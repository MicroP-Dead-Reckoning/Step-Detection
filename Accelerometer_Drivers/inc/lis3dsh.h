/**
  ******************************************************************************
  * @file    stm32f4_discovery_lis3dsh.h
  * @author  Ashraf Suyyagh / Based on the LIS3DSH driver by the MCD Application Team
  * @version V1.0.0
  * @date    11th-February-2014
  * @brief   This file contains all the functions prototypes for the stm32f4_discovery_lis3dsh.c
  *          firmware driver.
  ******************************************************************************
	**/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIS3DSH_H
#define __LIS3DSH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"
 #include "stm32f4xx_gpio.h"
 #include "stm32f4xx_rcc.h"
 #include "stm32f4xx_spi.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32F4_DISCOVERY
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY_LIS3DSH
  * @{
  */
  

/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Exported_Types
  * @{
  */
   
/* LIS3DSH struct */
typedef struct
{
  uint8_t Power_Mode_Output_DataRate;         /* Ppower down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  uint8_t Axes_Enable;                        /* Axes enable */
  uint8_t Continous_Update;					 				  /* Block or update Low/High registers of data until all data is read */
	uint8_t AA_Filter_BW;												/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  uint8_t Full_Scale;                         /* Full scale 2 / 4 / 6 / 8 / 16 g */
  uint8_t Self_Test;                          /* Self test */
}LIS3DSH_InitTypeDef;
 

/* LIS3DSH Data ready interrupt struct */
typedef struct
{
  uint8_t Dataready_Interrupt;                /* Enable/Disable data ready interrupt */
  uint8_t Interrupt_signal;                   /* Interrupt Signal Active Low / Active High */
  uint8_t Interrupt_type;                     /* Interrupt type as latched or pulsed */ 
}LIS3DSH_DRYInterruptConfigTypeDef;  

/**
  * @}
  */
  
/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Exported_Constants
  * @{
  */
  
  /* Uncomment the following line to use the default LIS3DSH_TIMEOUT_UserCallback() 
   function implemented in stm32f4_discovery_LIS3DSH.c file.
   LIS3DSH_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)). */   
/* #define USE_DEFAULT_TIMEOUT_CALLBACK */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
   
#define LIS3DSH_FLAG_TIMEOUT         ((uint32_t)0x1000)

/**
  * @brief  LIS3DSH SPI Interface pins
  */
#define LIS3DSH_SPI                       SPI1
#define LIS3DSH_SPI_CLK                   RCC_APB2Periph_SPI1

#define LIS3DSH_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define LIS3DSH_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define LIS3DSH_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define LIS3DSH_SPI_SCK_SOURCE            GPIO_PinSource5
#define LIS3DSH_SPI_SCK_AF                GPIO_AF_SPI1

#define LIS3DSH_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.6 */
#define LIS3DSH_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define LIS3DSH_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define LIS3DSH_SPI_MISO_SOURCE           GPIO_PinSource6
#define LIS3DSH_SPI_MISO_AF               GPIO_AF_SPI1

#define LIS3DSH_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.7 */
#define LIS3DSH_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define LIS3DSH_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define LIS3DSH_SPI_MOSI_SOURCE           GPIO_PinSource7
#define LIS3DSH_SPI_MOSI_AF               GPIO_AF_SPI1

#define LIS3DSH_SPI_CS_PIN                GPIO_Pin_3                  /* PE.03 */
#define LIS3DSH_SPI_CS_GPIO_PORT          GPIOE                       /* GPIOE */
#define LIS3DSH_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE

#define LIS3DSH_SPI_INT1_PIN              GPIO_Pin_0                  /* PE.00 */
#define LIS3DSH_SPI_INT1_GPIO_PORT        GPIOE                       /* GPIOE */
#define LIS3DSH_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define LIS3DSH_SPI_INT1_EXTI_LINE        EXTI_Line0
#define LIS3DSH_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define LIS3DSH_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
#define LIS3DSH_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

#define LIS3DSH_SPI_INT2_PIN              GPIO_Pin_1                  /* PE.01 */
#define LIS3DSH_SPI_INT2_GPIO_PORT        GPIOE                       /* GPIOE */
#define LIS3DSH_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define LIS3DSH_SPI_INT2_EXTI_LINE        EXTI_Line1
#define LIS3DSH_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define LIS3DSH_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
#define LIS3DSH_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 


/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/*******************************************************************************
*  OUT_T : Temperature sensor
*  Read only register
*  Default value: 0x00 corresponds to 25 degrees Celsius
*******************************************************************************/
#define LIS3DSH_OUT_T              0x0C
/******************************************************************************/

/*******************************************************************************
*  INFO1: Device Identification Register
*  Read only register
*  Default value: 0x21
*******************************************************************************/
#define LIS3DSH_INFO1              0x0D
/*******************************************************************************
*  INFO2: Device Identification Register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_INFO2              0x0E

/*******************************************************************************
*  WHO_AM_I Register: Device Identification Register
*  Read only register
*  Default value: 0x3B
*******************************************************************************/
#define LIS3DSH_WHO_AM_I_ADDR      0x0F

/*******************************************************************************
*  OFF_X Register: Offset compensation register X
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_OFF_X       		   0x10

/*******************************************************************************
*  OFF_Y Register: Offset compensation register Y
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_OFF_Y      			   0x11

/*******************************************************************************
*  OFF_Z Register: Offset compensation register Z
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_OFF_Z        		   0x12

/*******************************************************************************
*  STAT: STATUS register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_STAT       			   0x18

/*******************************************************************************
*  CTRL_REG4: Control Register 4 register
*  Read/Write  register
*  Default value: 0x07
*******************************************************************************/
#define LIS3DSH_CTRL_REG4       	 0x20

/*******************************************************************************
*  CTRL_REG1: Control Register 1 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_CTRL_REG1       	 0x21

/*******************************************************************************
*  CTRL_REG2: Control Register 2 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_CTRL_REG2       	 0x22

/*******************************************************************************
*  CTRL_REG3: Control Register 3 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_CTRL_REG3       	 0x23

/*******************************************************************************
*  CTRL_REG5: Control Register 5 register
*  Read/Write  register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_CTRL_REG5       	 0x24

/*******************************************************************************
*  STATUS: STATUS register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define LIS3DSH_STATUS       	 	 	 0x27

/*******************************************************************************
*  Output registers: X, Y, Z axes 8 bit MSB/LSB registers (combined 16 bits result)
*  Read only register
*  Default value: 0x00 corresponds to 0g acceleration
*******************************************************************************/
#define LIS3DSH_OUT_X_L       	 	 0x28
#define LIS3DSH_OUT_X_H       	 	 0x29
#define LIS3DSH_OUT_Y_L       	 	 0x2A
#define LIS3DSH_OUT_Y_H       	 	 0x2B
#define LIS3DSH_OUT_Z_L       	 	 0x2C
#define LIS3DSH_OUT_Z_H       	 	 0x2D

/******************************************************************************/
/*************************** END REGISTER MAPPING  ****************************/
/******************************************************************************/

/******************************************************************************/
/****************************** START BIT MAPPING  ****************************/
/******************************************************************************/


#define LIS3DSH_DOR									((uint8_t)0x02)
#define LIS3DSH_DRDY								((uint8_t)0x01)
#define LIS3DSH_ZYXOR								((uint8_t)0x80)
#define LIS3DSH_ZOR									((uint8_t)0x40)
#define LIS3DSH_YOR									((uint8_t)0x20)
#define LIS3DSH_XOR									((uint8_t)0x10)
#define LIS3DSH_ZYXDA								((uint8_t)0x08)
#define LIS3DSH_ZDA									((uint8_t)0x04)
#define LIS3DSH_YDA									((uint8_t)0x02)
#define LIS3DSH_XDA									((uint8_t)0x01)

/******************************************************************************/
/******************************* END BIT MAPPING  *****************************/
/******************************************************************************/

/** @defgroup Data_Rate_selection                 
  * @{
  */
#define LIS3DSH_PWRDWN							((uint8_t)0x00)
#define LIS3DSH_DATARATE_3_125			((uint8_t)0x10)
#define LIS3DSH_DATARATE_6_25				((uint8_t)0x20)
#define LIS3DSH_DATARATE_12_5				((uint8_t)0x30)
#define LIS3DSH_DATARATE_25					((uint8_t)0x40)
#define LIS3DSH_DATARATE_50					((uint8_t)0x50)
#define LIS3DSH_DATARATE_100				((uint8_t)0x60)
#define LIS3DSH_DATARATE_400				((uint8_t)0x70)
#define LIS3DSH_DATARATE_800				((uint8_t)0x80)
#define LIS3DSH_DATARATE_1600				((uint8_t)0x90)

/**
  * @}
  */
  
  /** @defgroup Full_Scale_selection 
  * @{
  */
#define LIS3DSH_FULLSCALE_2					((uint8_t)0x00)
#define LIS3DSH_FULLSCALE_4					((uint8_t)0x08)
#define LIS3DSH_FULLSCALE_6					((uint8_t)0x10)
#define LIS3DSH_FULLSCALE_8					((uint8_t)0x18)
#define LIS3DSH_FULLSCALE_16				((uint8_t)0x20)
/**
  * @}
  */
  
 /** @defgroup Antialiasing_Filter_BW 
  * @{
  */
#define LIS3DSH_AA_BW_800						((uint8_t)0x00)
#define LIS3DSH_AA_BW_400						((uint8_t)0x40)
#define LIS3DSH_AA_BW_200						((uint8_t)0x80)
#define LIS3DSH_AA_BW_50						((uint8_t)0xc0)
/**
  * @}
  */ 
  
  /** @defgroup Self_Test_selection 
  * @{
  */
#define LIS3DSH_SELFTEST_NORMAL    	((uint8_t)0x00)
#define LIS3DSH_SELFTEST_P          ((uint8_t)0x02)
#define LIS3DSH_SELFTEST_M          ((uint8_t)0x04)
/**
  * @}
  */  

/** @defgroup Direction_XYZ_selection 
  * @{
  */
#define LIS3DSH_X_ENABLE            ((uint8_t)0x01)
#define LIS3DSH_Y_ENABLE            ((uint8_t)0x02)
#define LIS3DSH_Z_ENABLE            ((uint8_t)0x04)
/**
  * @}
  */
  
 /** @defgroup Output_Register_Update 
 * @{
 */
  #define LIS3DSH_ContinousUpdate_Enabled							  ((uint8_t)0x08)
	#define LIS3DSH_ContinousUpdate_Disabled						  ((uint8_t)0x00)
	
 /**
 * @}
 */
 
 /** @defgroup SPI_Serial_Interface_Mode_selection 
  * @{
  */
#define LIS3DSH_SERIALINTERFACE_4WIRE    	((uint8_t)0x00)
#define LIS3DSH_SERIALINTERFACE_3WIRE     ((uint8_t)0x01)
/**
  * @}
  */ 
  
  /** @defgroup Data_Ready_Interrupt_Setup 
  * @{
  */
 #define LIS3DSH_DATA_READY_INTERRUPT_DISABLED     		 ((uint8_t)0x00)     
 #define LIS3DSH_DATA_READY_INTERRUPT_ENABLED					 ((uint8_t)0x88)
 #define LIS3DSH_ACTIVE_LOW_INTERRUPT_SIGNAL				   ((uint8_t)0x00)
 #define LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL			   	 ((uint8_t)0x40)
 #define LIS3DSH_INTERRUPT_REQUEST_PULSED              ((uint8_t)0x20)
 #define LIS3DSH_INTERRUPT_REQUEST_LATCHED             ((uint8_t)0x00)
  /**
  * @}
  */
  
	 /** @defgroup Sensitivity 
  * @{
  */
 #define LIS3DSH_SENSITIVITY_2G    	0.061 		      
 #define LIS3DSH_SENSITIVITY_4G		  0.122			 
 #define LIS3DSH_SENSITIVITY_6G			0.183	   
 #define LIS3DSH_SENSITIVITY_8G		  0.244	   	 
 #define LIS3DSH_SENSITIVITY_16G    0.488      
  /**
  * @}
  */
	
  /** @defgroup STM32F4_DISCOVERY_LIS3DSH_Exported_Macros
  * @{
  */
#define LIS3DSH_CS_LOW()       GPIO_ResetBits(LIS3DSH_SPI_CS_GPIO_PORT, LIS3DSH_SPI_CS_PIN)
#define LIS3DSH_CS_HIGH()      GPIO_SetBits(LIS3DSH_SPI_CS_GPIO_PORT, LIS3DSH_SPI_CS_PIN)
/**
  * @}
  */ 
	
/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Exported_Functions
  * @{
  */ 
void LIS3DSH_Init(LIS3DSH_InitTypeDef *LIS3DSHStruct);
void LIS3DSH_DataReadyInterruptConfig(LIS3DSH_DRYInterruptConfigTypeDef *LIS3DSH_InterruptConfigStruct);
void LIS3DSH_LowpowerCmd(void);
void LIS3DSH_FullScaleCmd(uint8_t FS_value);
void LIS3DSH_DataRateCmd(uint8_t DataRateValue);
void LIS3DSH_RebootCmd(void);
void LIS3DSH_ReadACC(float* out);
void LIS3DSH_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void LIS3DSH_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* USER Callbacks: This is function for which prototype only is declared in
   MEMS accelerometre driver and that should be implemented into user applicaiton. */  
/* LIS3DSH_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_lis302dl.h file.
   Typically the user implementation of this callback should reset MEMS peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t LIS3DSH_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIS3DSH_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 
