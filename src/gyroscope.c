#include "gyroscope.h"

#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "lsm9ds1.h"
#include "moving_average.h"
#include "osObjects.h"

#define NUM_CALIBRATION 100
#define X_OFFSET 50
#define Y_OFFSET 10 //-50.665001

#define Z_OFFSET 10
#define G_SCALER 1.841f
#define PI 3.14159265358979323846
#define buffer_size 40
#define DT 0.010000

float G_YAW_OFFSET;
float G_PITCH_OFFSET;
float G_ROLL_OFFSET;

int32_t xl_out[10], g_out[10];
int32_t past_g_out_pitch[3], past_g_out_roll[3], past_g_out_yaw[3];
float roll, pitch;

float g_pitch_sum = 0;
float g_roll_sum = 0;
float g_yaw_sum = 0;

float combined_roll;
float combined_pitch;

typedef struct {
	float data[buffer_size];
	int index;
	float buffer_avg;
	int count;
}filter_state;


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

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);

	EXTI_InitTypeDef exti_init;

	exti_init.EXTI_Line = EXTI_Line0; //EXTI Line 0
	exti_init.EXTI_LineCmd = ENABLE; //Enable
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt; //Use for interupts
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&exti_init);

	/* setup nvic */
	NVIC_InitTypeDef nvic_init;

	nvic_init.NVIC_IRQChannel = EXTI0_IRQn; //EXTIO
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic_init.NVIC_IRQChannelSubPriority = 0x01;

	NVIC_Init(&nvic_init);
	//calculate_gyro_offsets();
	
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
}

void calculate_gyro_offsets(void){
	
	for(int i=0;i<50;i++){
		LSM9DS1_ReadG(g_out);
		G_PITCH_OFFSET += (g_out[0] >> 11);
		G_ROLL_OFFSET += (g_out[1] >> 11);
		G_YAW_OFFSET += (g_out[2] >> 11);
//		printf("%d \n", i);
	}
	
		G_PITCH_OFFSET /= 50;
		G_ROLL_OFFSET /= 50;
		G_YAW_OFFSET /= 50;
	//	printf("Offset :G	 Pitch: %f Roll: %f	Yaw: %f\n", G_PITCH_OFFSET, G_ROLL_OFFSET, G_YAW_OFFSET);
}

void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
		osSignalSet(gyroscope_thread, SIGNAL_GYROSCOPE);
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
			EXTI_ClearITPendingBit(EXTI_Line0); 			/* Clear interrupt flag */
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
		
		//Scale output before output
		g_pitch = g_pitch * G_SCALER;
		g_roll = g_roll * G_SCALER;
		g_yaw = g_yaw* G_SCALER;
		
		g_pitch_sum = g_pitch_sum + (DT/6)*(past_g_out_pitch[0] + 2*past_g_out_pitch[1] + 2*past_g_out_pitch[2] + g_pitch);
		g_roll_sum = g_roll_sum + (DT/6)*(past_g_out_roll[0] + 2*past_g_out_roll[1] + 2*past_g_out_roll[2] + g_roll);
		g_yaw_sum = g_yaw_sum + (DT/6)*(past_g_out_yaw[0] + 2*past_g_out_yaw[1] + 2*past_g_out_yaw[2] + g_yaw);

		

		past_g_out_pitch[0] = past_g_out_pitch[1];
		past_g_out_pitch[1] = past_g_out_pitch[2];
		past_g_out_pitch[2] = g_pitch;
		
		past_g_out_roll[0] = past_g_out_roll[1];
		past_g_out_roll[1] = past_g_out_roll[2];
		past_g_out_roll[2] = g_roll;
		
		past_g_out_yaw[0] = past_g_out_yaw[1];
		past_g_out_yaw[1] = past_g_out_yaw[2];
		past_g_out_yaw[2] = g_yaw;

		combined_roll = 0.5f * g_roll_sum + 0.5f * roll;
		combined_pitch = 0.5f * g_pitch_sum + 0.5f * pitch;

		if(g_yaw_sum == 0){
			G_YAW_OFFSET = 0;
			G_PITCH_OFFSET = 0;
			G_ROLL_OFFSET = 0;
			calculate_gyro_offsets();
		}
		
		printf("Roll: %f\n", g_roll);
	
	}
}

