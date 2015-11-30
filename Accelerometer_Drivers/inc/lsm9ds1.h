/**
  ******************************************************************************
  * @file    stm32f4_discovery_LSM9DS1.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    28-October-2011
  * @brief   This file contains all the functions prototypes for the stm32f4_discovery_LSM9DS1.c
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_LSM9DS1_H
#define __STM32F4_DISCOVERY_LSM9DS1_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"
 #include "stm32f4xx_conf.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32F4_DISCOVERY
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY_LSM9DS1
  * @{
  */
  

/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Types
  * @{
  */
   
/* LSM9DS1 struct */
typedef struct
{
  uint8_t Power_Mode_Output_DataRate;         /* Power-down/Active Mode */
  uint8_t XL_Axes_Enable;                        /* Axes enable */
	uint8_t G_Axes_Enable;
  uint8_t Full_Scale;                         /* Full scale */
  uint8_t Bandwidth_Selection;                /* Self test */
	uint8_t Anti_Aliasing_Filter_BW;
	uint8_t G_Full_Scale;
	uint8_t G_Power_Mode_Output_DataRate;
}LSM9DS1_InitTypeDef;


/**
  * @}
  */
  
/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Constants
  * @{
  */

/* Uncomment the following line to use the default LSM9DS1_TIMEOUT_UserCallback() 
   function implemented in stm32f4_discovery_LSM9DS1.c file.
   LSM9DS1_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)). */   
/* #define USE_DEFAULT_TIMEOUT_CALLBACK */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define LSM9DS1_FLAG_TIMEOUT         ((uint32_t)0x1000)

/**
  * @brief  LSM9DS1 SPI Interface pins
  */
#define LSM9DS1_SPI                       SPI3
#define LSM9DS1_SPI_CLK                   RCC_APB1Periph_SPI3

#define LSM9DS1_SPI_SCK_PIN               GPIO_Pin_10                  /* PC.10, SCL */
#define LSM9DS1_SPI_SCK_GPIO_PORT         GPIOC                       /* GPIOC */
#define LSM9DS1_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define LSM9DS1_SPI_SCK_SOURCE            GPIO_PinSource10
#define LSM9DS1_SPI_SCK_AF                GPIO_AF_SPI3

#define LSM9DS1_SPI_MISO_PIN              GPIO_Pin_4                  /* PB.4 SDO AG */
#define LSM9DS1_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define LSM9DS1_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define LSM9DS1_SPI_MISO_SOURCE           GPIO_PinSource4
#define LSM9DS1_SPI_MISO_AF               GPIO_AF_SPI3

#define LSM9DS1_SPI_MOSI_PIN              GPIO_Pin_5                  /* PB.5, SDA */
#define LSM9DS1_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define LSM9DS1_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define LSM9DS1_SPI_MOSI_SOURCE           GPIO_PinSource5
#define LSM9DS1_SPI_MOSI_AF               GPIO_AF_SPI3

#define LSM9DS1_SPI_CS_PIN                GPIO_Pin_15                  /* PA.15 */
#define LSM9DS1_SPI_CS_GPIO_PORT          GPIOA                       /* GPIOA */
#define LSM9DS1_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOA

#define LSM9DS1_SPI_INT1_PIN              GPIO_Pin_0                  /* PD.00 */
#define LSM9DS1_SPI_INT1_GPIO_PORT        GPIOD                       /* GPIOD */
#define LSM9DS1_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define LSM9DS1_SPI_INT1_EXTI_LINE        EXTI_Line0									
#define LSM9DS1_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOD
#define LSM9DS1_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
#define LSM9DS1_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

#define LSM9DS1_SPI_INT2_PIN              GPIO_Pin_6                  /* PD.06 */
#define LSM9DS1_SPI_INT2_GPIO_PORT        GPIOD                       /* GPIOD */
#define LSM9DS1_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define LSM9DS1_SPI_INT2_EXTI_LINE        EXTI_Line6
#define LSM9DS1_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOD
#define LSM9DS1_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource6
#define LSM9DS1_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 


/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/*******************************************************************************
*  WHO_AM_I Register: Device Identification Register
*  Read only register
*  Default value: 0x3B
*******************************************************************************/
#define LSM9DS1_WHO_AM_I_ADDR                  0x0F

#define LSM9DS1_ACT_THS												0x04								
#define LSM9DS1_ACT_DUR												0x05								
#define LSM9DS1_INT_GEN_CFG_XL								0x06
#define LSM9DS1_INT_THS_X_XL									0x07							
#define LSM9DS1_INT_THS_Y_XL									0x08							
#define LSM9DS1_INT_THS_Z_XL									0x09							
#define LSM9DS1_INT_GEN_DUR_XL								0x0A							
#define LSM9DS1_REFERENCE_G										0x0B
#define LSM9DS1_INT1_CTRL											((uint8_t)0x0C)								
#define LSM9DS1_INT2_CTRL											0x0D	

#define LSM9DS1_CTRL_REG1_G										0x10
#define LSM9DS1_CTRL_REG2_G										0x11
#define LSM9DS1_CTRL_REG3_G										0x12
#define LSM9DS1_ORIENT_CFG_G									0x13
#define LSM9DS1_INT_GEN_SRC_G									0x14
#define LSM9DS1_OUT_TEMP_L										0x15
#define LSM9DS1_OUT_TEMP_H										0x16
#define LSM9DS1_STATUS_REG										0x17

#define LSM9DS1_OUT_X_L_G											0x18
#define LSM9DS1_OUT_X_H_G											0x19

#define LSM9DS1_OUT_Y_L_G											0x1A
#define LSM9DS1_OUT_Y_H_G											0x1B

#define LSM9DS1_OUT_Z_L_G											0x1C
#define LSM9DS1_OUT_Z_H_G											0x1D

#define LSM9DS1_CTRL_REG4											0x1E
#define LSM9DS1_CTRL_REG5_XL									0x1F 	//Linear Acceleration Sensor Control
#define LSM9DS1_CTRL_REG6_XL									((uint8_t)0x20)	//Linear Acceleration Sensor Control (Gyro accel switch)
#define LSM9DS1_CTRL_REG7_XL									0x21

#define LSM9DS1_CTRL_REG8											0x22
#define LSM9DS1_CTRL_REG9											0x23
#define LSM9DS1_CTRL_REG10										0x24

#define LSM9DS1_INT_GEN_SRC_XL								0x26
#define LSM9DS1_STATUS_REG2										0x27

#define LSM9DS1_OUT_X_L_XL										0x28
#define LSM9DS1_OUT_X_H_XL										0x29
#define LSM9DS1_OUT_Y_L_XL										0x2A
#define LSM9DS1_OUT_Y_H_XL										0x2B
#define LSM9DS1_OUT_Z_L_XL										0x2C
#define LSM9DS1_OUT_Z_H_XL										0x2D

#define LSM9DS1_FIFO_CTRL											0x2E
#define LSM9DS1_FIFO_SRC											0x2F

#define LSM9DS1_INT_GEN_CFG_G									0x30
#define LSM9DS1_INT_THS_XH_XL									0x31							
#define LSM9DS1_INT_THS_XL_XL									0x32							

#define LSM9DS1_INT_THS_YH_XL									0x33							
#define LSM9DS1_INT_THS_YL_XL									0x34							

#define LSM9DS1_INT_THS_ZH_XL									0x35							
#define LSM9DS1_INT_THS_ZL_XL									0x36							

#define LSM9DS1_INT_GEN_DUR_G									0x37							



/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/

#define LSM9DS1_SENSITIVITY_2G                         0.061	//mg/LSB
#define LSM9DS1_SENSITIVITY_4G                         0.122	//mg/LSB
#define LSM9DS1_SENSITIVITY_8G                         0.244	//mg/LSB
#define LSM9DS1_SENSITIVITY_16G                        0.732	//mg/LSB


#define LSM9DS1_G_SENSITIVITY_245                         8.75	//mdps
#define LSM9DS1_G_SENSITIVITY_500                         17.5	//mdps
#define LSM9DS1_G_SENSITIVITY_2000                        70	//mdps


/** @defgroup Data_Rate_selection                 
  * @{
  */
	
#define LSM9DS1_DATARATE_OFF								((uint8_t)0x00)
#define LSM9DS1_DATARATE_10									((uint8_t)0x20)
#define LSM9DS1_DATARATE_50									((uint8_t)0x40)
#define LSM9DS1_DATARATE_119								((uint8_t)0x60)
#define LSM9DS1_DATARATE_238								((uint8_t)0x80)
#define LSM9DS1_DATARATE_476								((uint8_t)0xA0)
#define LSM9DS1_DATARATE_952								((uint8_t)0xC0)
#define LSM9DS1_DATARATE_NA									((uint8_t)0xE0)

//Gyroscope
#define LSM9DS1_G_DATARATE_OFF								((uint8_t)0x00)
#define LSM9DS1_G_DATARATE_14									((uint8_t)0x20)
#define LSM9DS1_G_DATARATE_59									((uint8_t)0x40)
#define LSM9DS1_G_DATARATE_119								((uint8_t)0x60)
#define LSM9DS1_G_DATARATE_238								((uint8_t)0x80)
#define LSM9DS1_G_DATARATE_476								((uint8_t)0xA0)
#define LSM9DS1_G_DATARATE_952								((uint8_t)0xC0)
#define LSM9DS1_G_DATARATE_NA									((uint8_t)0xE0)

/**
  * @}
  */

/** @defgroup Full_Scale_selection 
  * @{
  */
#define LSM9DS1_FULLSCALE_2G                ((uint8_t)0x00)
#define LSM9DS1_FULLSCALE_4G                ((uint8_t)0x10)
#define LSM9DS1_FULLSCALE_16G               ((uint8_t)0x08)
#define LSM9DS1_FULLSCALE_8G                ((uint8_t)0x18)

#define LSM9DS1_FS_G_245										((uint8_t)0x00)
#define LSM9DS1_FS_G_500										((uint8_t)0x08)
#define LSM9DS1_FS_G_2000										((uint8_t)0x18)
/**
  * @}
  */

/** @defgroup BandWidth_selection 
  * @{
  */
#define LSM9DS1_BANDWIDTH_ODR	             ((uint8_t)0x00)
#define LSM9DS1_BANDWIDTH_AA	             ((uint8_t)0x04)

/**
  * @}
  */

/** @defgroup AA_FILTER
  * @{
  */
#define LSM9DS1_AA_FILTER_408                 ((uint8_t)0x00)
#define LSM9DS1_AA_FILTER_211                 ((uint8_t)0x01)
#define LSM9DS1_AA_FILTER_105                 ((uint8_t)0x02)
#define LSM9DS1_AA_FILTER_50                	((uint8_t)0x03)
/**
  * @}
  */
//============================BOOKMARK=================================

/** @defgroup Activity Threshold
  * @{
  */
#define LSM9DS1_INACT_G_POWER_DOWN                       ((uint8_t)0x00)
#define LSM9DS1_INACT_G_SLEEP			                       ((uint8_t)0x80)
#define LSM9DS1_INIT_DEFAULT_THRESH                       ((uint8_t)0x00)
/**
  * @}
  */  


/** @defgroup Self_Test_selection 
  * @{
  */
#define LSM9DS1_SELFTEST_NORMAL                          ((uint8_t)0x00)
#define LSM9DS1_SELFTEST_P                               ((uint8_t)0x10)
#define LSM9DS1_SELFTEST_M                               ((uint8_t)0x08)
/**
  * @}
  */  

//Modified for lsm9ds1
/** @defgroup Direction_XYZ_selection 
  * @{
  */
#define LSM9DS1_X_ENABLE                                 ((uint8_t)0x08)
#define LSM9DS1_Y_ENABLE                                 ((uint8_t)0x10)
#define LSM9DS1_Z_ENABLE                                 ((uint8_t)0x20)
#define LSM9DS1_XYZ_ENABLE                               ((uint8_t)0x07)
/**
  * @}
  */
	
	
/** @defgroup Gyroscope_Direction_XYZ_selection 
  * @{
  */	
#define LSM9DS1_G_X_ENABLE                               ((uint8_t)0x08)
#define LSM9DS1_G_Y_ENABLE                               ((uint8_t)0x10)
#define LSM9DS1_G_Z_ENABLE                               ((uint8_t)0x20)	
	
 
 /** @defgroup SPI_Serial_Interface_Mode_selection 
  * @{
  */
#define LSM9DS1_SERIALINTERFACE_4WIRE                    ((uint8_t)0x00)
#define LSM9DS1_SERIALINTERFACE_3WIRE                    ((uint8_t)0x80)
/**
  * @}
  */ 

 /** @defgroup Boot_Mode_selection 
  * @{
  */
#define LSM9DS1_BOOT_NORMALMODE                          ((uint8_t)0x00)
#define LSM9DS1_BOOT_REBOOTMEMORY                        ((uint8_t)0x40)
/**
  * @}
  */   

 /** @defgroup Filtered_Data_Selection_Mode_selection 
  * @{
  */
#define LSM9DS1_FILTEREDDATASELECTION_BYPASSED           ((uint8_t)0x00)
#define LSM9DS1_FILTEREDDATASELECTION_OUTPUTREGISTER     ((uint8_t)0x20)
/**
  * @}
  */ 
  
 /** @defgroup High_Pass_Filter_Interrupt_selection 
  * @{
  */  
#define LSM9DS1_HIGHPASSFILTERINTERRUPT_OFF              ((uint8_t)0x00)
#define LSM9DS1_HIGHPASSFILTERINTERRUPT_1                ((uint8_t)0x04)
#define LSM9DS1_HIGHPASSFILTERINTERRUPT_2                ((uint8_t)0x08)
#define LSM9DS1_HIGHPASSFILTERINTERRUPT_1_2              ((uint8_t)0x0C)
/**
  * @}
  */ 
  
 /** @defgroup High_Pass_Filter_selection 
  * @{
  */
#define LSM9DS1_HIGHPASSFILTER_LEVEL_0                   ((uint8_t)0x00)
#define LSM9DS1_HIGHPASSFILTER_LEVEL_1                   ((uint8_t)0x01)
#define LSM9DS1_HIGHPASSFILTER_LEVEL_2                   ((uint8_t)0x02)
#define LSM9DS1_HIGHPASSFILTER_LEVEL_3                   ((uint8_t)0x03)
/**
  * @}
  */


/** @defgroup latch_Interrupt_Request_selection 
  * @{
  */
#define LSM9DS1_INTERRUPTREQUEST_NOTLATCHED              ((uint8_t)0x00)
#define LSM9DS1_INTERRUPTREQUEST_LATCHED                 ((uint8_t)0x40)
/**
  * @}
  */

/** @defgroup Click_Interrupt_XYZ_selection 
  * @{
  */
#define LSM9DS1_CLICKINTERRUPT_XYZ_DISABLE               ((uint8_t)0x00)
#define LSM9DS1_CLICKINTERRUPT_X_ENABLE                  ((uint8_t)0x01)
#define LSM9DS1_CLICKINTERRUPT_Y_ENABLE                  ((uint8_t)0x04)
#define LSM9DS1_CLICKINTERRUPT_Z_ENABLE                  ((uint8_t)0x10)
#define LSM9DS1_CLICKINTERRUPT_XYZ_ENABLE                ((uint8_t)0x15)
/**
  * @}
  */

/** @defgroup Double_Click_Interrupt_XYZ_selection 
  * @{
  */
#define LSM9DS1_DOUBLECLICKINTERRUPT_XYZ_DISABLE         ((uint8_t)0x00)
#define LSM9DS1_DOUBLECLICKINTERRUPT_X_ENABLE            ((uint8_t)0x02)
#define LSM9DS1_DOUBLECLICKINTERRUPT_Y_ENABLE            ((uint8_t)0x08)
#define LSM9DS1_DOUBLECLICKINTERRUPT_Z_ENABLE            ((uint8_t)0x20)
#define LSM9DS1_DOUBLECLICKINTERRUPT_XYZ_ENABLE          ((uint8_t)0x2A)
/**
  * @}
  */
/**
  * @}
  */ 
  
/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Macros
  * @{
  */
#define LSM9DS1_CS_LOW()       GPIO_ResetBits(LSM9DS1_SPI_CS_GPIO_PORT, LSM9DS1_SPI_CS_PIN)
#define LSM9DS1_CS_HIGH()      GPIO_SetBits(LSM9DS1_SPI_CS_GPIO_PORT, LSM9DS1_SPI_CS_PIN)
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Exported_Functions
  * @{
  */ 
void LSM9DS1_Init(LSM9DS1_InitTypeDef *LSM9DS1_InitStruct);
//void LSM9DS1_InterruptConfig(LSM9DS1_InterruptConfigTypeDef *LSM9DS1_InterruptConfigStruct);
//void LSM9DS1_FilterConfig(LSM9DS1_FilterConfigTypeDef *LSM9DS1_FilterConfigStruct);
void LSM9DS1_LowpowerCmd(uint8_t LowPowerMode);
void LSM9DS1_FullScaleCmd(uint8_t FS_value);
void LSM9DS1_DataRateCmd(uint8_t DataRateValue);
void LSM9DS1_RebootCmd(void);
void LSM9DS1_ReadXL(int32_t* out);
void LSM9DS1_ReadG(int32_t* out);
void LSM9DS1_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void LSM9DS1_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* USER Callbacks: This is function for which prototype only is declared in
   MEMS accelerometre driver and that should be implemented into user applicaiton. */  
/* LSM9DS1_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_LSM9DS1.h file.
   Typically the user implementation of this callback should reset MEMS peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t LSM9DS1_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_LSM9DS1_H */
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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
