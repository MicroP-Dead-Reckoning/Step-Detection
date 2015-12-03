#include "gyroscope.h"

#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "lsm9ds1.h"
#include "moving_average.h"
#include "osObjects.h"
#include "update_pos.h"
//#include "viterbi.h"

#define NUM_CALIBRATION 20
#define X_OFFSET 50
#define Y_OFFSET 10 //-50.665001
#define Z_OFFSET 10

#define G_SCALER 2.0553f
#define PI 3.14159265358979323846

#define buffer_size 40
#define DT 0.0084033613


#define STEP_DELAY 30
#define RISING_STEP_THRESHOLD 15
#define FALLING_STEP_THRESHOLD -10

float G_YAW_OFFSET;
float G_PITCH_OFFSET;
float G_ROLL_OFFSET;

int32_t xl_out[3], g_out[3];
int32_t past_g_out_pitch[3], past_g_out_roll[3], past_g_out_yaw[3];
float roll, pitch;

float g_pitch_sum = 0;
float g_roll_sum = 0;
float g_yaw_sum = 0;

float combined_roll;
float combined_pitch;

int state = 0;
uint8_t heading_state = 0;
int counter = 0;

typedef struct {
	float data[buffer_size];
	int index;
	float buffer_avg;
	int count;
}filter_state;

typedef enum {
	STANDING,
	STEPPING
} STEP;

STEP step;
char* PRINT_STATE[] = {"STANDING", "STEPPING"};
int waiting;

FilterBuffer x_buffer;
float x_data[buffer_size];

FilterBuffer y_buffer;
float y_data[buffer_size];

FilterBuffer z_buffer;
float z_data[buffer_size];

FilterBuffer g_pitch_buffer;
float pitch_data[buffer_size];

FilterBuffer g_roll_buffer;
float roll_data[buffer_size];

FilterBuffer g_yaw_buffer;
float yaw_data[buffer_size];

//#define NUM_POSITION_READINGS 100
//#define initial_heading 0
//#define initial_position 0
//#define step_distance 1
//char heading[NUM_POSITION_READINGS];
//char distance[NUM_POSITION_READINGS];
//int position_reading = 1;


/* Helper Functions */
void calculate_gyro_offsets(void);


void init_gyroscope() {
	/* initialize gyroscope */
	LSM9DS1_InitTypeDef xl_g_init;

	xl_g_init.G_Axes_Enable  = LSM9DS1_G_X_ENABLE | LSM9DS1_G_Y_ENABLE | LSM9DS1_G_Z_ENABLE;
	xl_g_init.G_Full_Scale = LSM9DS1_FS_G_500;
	xl_g_init.G_Power_Mode_Output_DataRate = LSM9DS1_G_DATARATE_119;

	xl_g_init.XL_Axes_Enable = LSM9DS1_X_ENABLE | LSM9DS1_Y_ENABLE | LSM9DS1_Z_ENABLE;
	xl_g_init.Full_Scale = LSM9DS1_FULLSCALE_2G;
	xl_g_init.Power_Mode_Output_DataRate = LSM9DS1_DATARATE_119;
	xl_g_init.Bandwidth_Selection = LSM9DS1_BANDWIDTH_ODR;
	xl_g_init.Anti_Aliasing_Filter_BW = LSM9DS1_AA_FILTER_408; //Default value anyway

	LSM9DS1_Init(&xl_g_init);

	/* setup exti */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //CHANGE?
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // Enable

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);

	EXTI_InitTypeDef exti_init;

	exti_init.EXTI_Line = EXTI_Line1; //EXTI Line 0
	exti_init.EXTI_LineCmd = ENABLE; //Enable
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt; //Use for interupts
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&exti_init);

	/* setup nvic */
	NVIC_InitTypeDef nvic_init;

	nvic_init.NVIC_IRQChannel = EXTI1_IRQn; //EXTIO
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic_init.NVIC_IRQChannelSubPriority = 0x01;

	NVIC_Init(&nvic_init);
	calculate_gyro_offsets();

	/* setup filters */
	x_buffer.buffer = x_data;
	y_buffer.buffer = y_data;
	z_buffer.buffer = z_data;

	x_buffer.size = buffer_size;
	y_buffer.size = buffer_size;
	z_buffer.size = buffer_size;

	g_yaw_buffer.buffer = yaw_data;
	g_roll_buffer.buffer = roll_data;
	g_pitch_buffer.buffer = pitch_data;

	g_yaw_buffer.size = buffer_size;
	g_roll_buffer.size = buffer_size;
	g_pitch_buffer.size = buffer_size;
	
	step = STANDING;
	waiting = 0;
	
//	heading[0] = initial_heading;
//	distance[0] = initial_position;
}

void calculate_gyro_offsets(void){

	for(int i=0;i<NUM_CALIBRATION;i++){
		LSM9DS1_ReadG(g_out);
		G_PITCH_OFFSET += (g_out[0] >> 11);
		G_ROLL_OFFSET += (g_out[1] >> 11);
		G_YAW_OFFSET += (g_out[2] >> 11);
//		printf("%d \n", i);
	}

		G_PITCH_OFFSET /= NUM_CALIBRATION;
		G_ROLL_OFFSET /= NUM_CALIBRATION;
		G_YAW_OFFSET /= NUM_CALIBRATION;
	//	printf("Offset :G	 Pitch: %f Roll: %f	Yaw: %f\n", G_PITCH_OFFSET, G_ROLL_OFFSET, G_YAW_OFFSET);
}

void EXTI1_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
		osSignalSet(gyroscope_thread, SIGNAL_GYROSCOPE);
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
			EXTI_ClearITPendingBit(EXTI_Line1); 			/* Clear interrupt flag */
    }
}

void Gyroscope(void const *argument) {
	while(1) {
		osSignalWait(SIGNAL_GYROSCOPE, osWaitForever);


		int32_t result[3];

		LSM9DS1_ReadXL(xl_out);
		result[0] = xl_out[0];
		result[1] = xl_out[1];
		result[2] = xl_out[2];
		//printf("x: %d y:%d z:%d \n", result[0], result[1], result[2]);

		result[0] = result[0] + X_OFFSET;
		result[1] = result[1] + Y_OFFSET;
		result[2] = result[2] + Z_OFFSET;

		//printf(" Corrected x: %d y:%d z:%d \n", result[0], result[1], result[2]);

		float x_value = add_value(&x_buffer, result[0]);
		float y_value = add_value(&y_buffer, result[1]);
		float z_value = add_value(&z_buffer, result[2]);

		roll = (180/PI) * atan(x_value/sqrt(pow(y_value, 2) + pow(z_value, 2)));		//Uses pitch equation
		pitch = (-180/PI) * atan(y_value/sqrt(pow(x_value, 2) + pow(z_value, 2)));		//Uses roll equation, "-180" as it is opposite of printed axes
		//roll = roll + 90;	//Changes range from -90:90 to 0:180

		LSM9DS1_ReadG(g_out);

		g_out[0] = (g_out[0] >> 11);
		g_out[1] = (g_out[1] >> 11);
		g_out[2] = (g_out[2] >> 11);

		float g_pitch = g_out[0];
		float g_roll = g_out[1];
		float g_yaw = g_out[2];

		g_pitch = add_value(&g_pitch_buffer, g_out[0]);
		g_roll = add_value(&g_roll_buffer, g_out[1]);
		g_yaw = add_value(&g_yaw_buffer, g_out[2]);


		g_pitch -= G_PITCH_OFFSET;
		g_roll -= G_ROLL_OFFSET;


		if(g_yaw > 0){
			g_yaw -= G_YAW_OFFSET;
		}
		else{
			g_yaw += G_YAW_OFFSET;
		}

		//Scales output to actual angles
		g_pitch = g_pitch * G_SCALER;
		g_roll = g_roll * G_SCALER;
		g_yaw = g_yaw* G_SCALER;

		//Runge-Kutta integration for extra precision
		g_pitch_sum = g_pitch_sum + (DT/6)*(past_g_out_pitch[0] + 2*past_g_out_pitch[1] + 2*past_g_out_pitch[2] + g_pitch);
		g_roll_sum = g_roll_sum + (DT/6)*(past_g_out_roll[0] + 2*past_g_out_roll[1] + 2*past_g_out_roll[2] + g_roll);
		g_yaw_sum = g_yaw_sum + (DT/6)*(past_g_out_yaw[0] + 2*past_g_out_yaw[1] + 2*past_g_out_yaw[2] + g_yaw);


		//Keep past 3 observations (Used for integration)
		if(counter<3)
		{
			past_g_out_pitch[counter] = g_pitch;
			past_g_out_roll[counter] = g_roll;
			past_g_out_yaw[counter] = g_yaw;
			counter++;
		}
		else{
			past_g_out_pitch[0] = past_g_out_pitch[1];
			past_g_out_pitch[1] = past_g_out_pitch[2];
			past_g_out_pitch[2] = g_pitch;

			past_g_out_roll[0] = past_g_out_roll[1];
			past_g_out_roll[1] = past_g_out_roll[2];
			past_g_out_roll[2] = g_roll;

			past_g_out_yaw[0] = past_g_out_yaw[1];
			past_g_out_yaw[1] = past_g_out_yaw[2];
			past_g_out_yaw[2] = g_yaw;
		}
		//
		//=========================		
		//	7	|
		//	-
		//	6	|
		//	-
		//	5	|
		//Increase the state if clockwise rotation is measured
		if(45<g_yaw_sum){
				state++;
				g_yaw_sum = 0;
			}
		//Decrease the state if anticlockwise rotation is measured
		else if(-45>g_yaw_sum){
			state--;
			g_yaw_sum = 0;
		}
		//Reset the state when reaching abs(state) = 8
		if(state == 8 || state == -8)
		{
			state = 0;
		}

		//Combine positive and negative states into one
		if(state == 1 || state == -7)
		{
			heading_state = 1;
		}
		else if(state == 2 || state == -6)
		{
			heading_state = 2;
		}
		else if(state == 3 || state == -5)
		{
			heading_state = 3;
		}
		else if(state == 4 || state == -4)
		{
			heading_state = 4;
		}
		else if(state == 5 || state == -3)
		{
			heading_state = 5;
		}
		else if(state == 6 || state == -2)
		{
			heading_state = 6;
		}
		else if(state == 7 || state == -1)
		{
			heading_state = 7;
		}
		else if(state == 0)
		{
			heading_state = 0;
		}
		//printf("Yaw: %f	State: %d \n",g_yaw_sum, heading_state);
		switch (step) {
			case STANDING:
				if (waiting == 0) {
					if (g_roll > RISING_STEP_THRESHOLD) {
							printf("stepping direction %d\n", heading_state);\
							send_pos(heading_state);
//						heading[position_reading] = heading_state;
//						distance[position_reading] = step_distance;
//						position_reading++; //increment index to store next reading
							step = STEPPING;
							waiting = STEP_DELAY;	
					}
				}
				else {
					waiting--;
				}
				break;
			case STEPPING:
				if (waiting == 0) {
					if (g_roll < FALLING_STEP_THRESHOLD) {
						step = STANDING;
						printf("standing %d\n", heading_state);
						waiting = STEP_DELAY;
					}
				}
				else {
					waiting--;
				}
				break;
		}
		//printf("\tRoll, %f,,\n", g_roll);
		//printf("%s: %d\n", PRINT_STATE[step], heading_state);
	}
}
