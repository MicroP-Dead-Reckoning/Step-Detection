/**
  ******************************************************************************
  * @file    stm32f4_discovery_LIS3DSH.c
  * @author  Ashraf Suyyagh based on the MCD Application Team implementation of the LIS3DSH driver
  * @version V1.0.0
  * @date    12-February-2014
  * @brief   This file provides a set of functions needed to manage the LIS3DSH
  *          MEMS accelerometer available on STM32F4-Discovery Kit.
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "lis3dsh.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY_LIS3DSH
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Private_Defines
  * @{
  */
__IO uint32_t  LIS3DSHTimeout = LIS3DSH_FLAG_TIMEOUT;   

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

#define USE_DEFAULT_TIMEOUT_CALLBACK

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Private_Macros
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Private_Variables
  * @{
  */ 

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Private_FunctionPrototypes
  * @{
  */
static uint8_t LIS3DSH_SendByte(uint8_t byte);
static void LIS3DSH_LowLevel_Init(void);
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LIS3DSH_Private_Functions
  * @{
  */


/**
  * @brief  Set LIS3DSH Initialization.
  * @param  LIS3DSH_Config_Struct: pointer to a LIS3DSH_Config_TypeDef structure 
  *         that contains the configuration setting for the LIS3DSH.
  * @retval None
  */
void LIS3DSH_Init(LIS3DSH_InitTypeDef *LIS3DSH_InitStruct)
{
  uint8_t ctrl = 0x00;
 
  /* Configure the low level interface ---------------------------------------*/
  LIS3DSH_LowLevel_Init();
  
  /* Configure MEMS: data rate, update mode and axes */
  ctrl = (uint8_t) (LIS3DSH_InitStruct->Power_Mode_Output_DataRate | \
										LIS3DSH_InitStruct->Continous_Update           | \
										LIS3DSH_InitStruct->Axes_Enable);
                    
  
  /* Write value to MEMS CTRL_REG4 regsister */
  LIS3DSH_Write(&ctrl, LIS3DSH_CTRL_REG4, 1);
	
	/* Configure MEMS: Anti-aliasing filter, full scale, self test  */
	ctrl = (uint8_t) (LIS3DSH_InitStruct->AA_Filter_BW | \
										LIS3DSH_InitStruct->Full_Scale   | \
										LIS3DSH_InitStruct->Self_Test);
	
	/* Write value to MEMS CTRL_REG5 regsister */
	LIS3DSH_Write(&ctrl, LIS3DSH_CTRL_REG5, 1);
}

/**
  * @brief Set LIS3DSH Interrupt configuration
  * @param  LIS3DSH_InterruptConfig_TypeDef: pointer to a LIS3DSH_InterruptConfig_TypeDef 
  *         structure that contains the configuration setting for the LIS3DSH Interrupt.
  * @retval None
  */
void LIS3DSH_DataReadyInterruptConfig(LIS3DSH_DRYInterruptConfigTypeDef *LIS3DSH_IntConfigStruct)
{
  uint8_t ctrl = 0x00;
  
  /* Read CLICK_CFG register */
  LIS3DSH_Read(&ctrl, LIS3DSH_CTRL_REG3, 1);
  
  /* Configure latch Interrupt request, click interrupts and double click interrupts */                   
  ctrl = (uint8_t)(LIS3DSH_IntConfigStruct->Dataready_Interrupt| \
                   LIS3DSH_IntConfigStruct->Interrupt_signal | \
                   LIS3DSH_IntConfigStruct->Interrupt_type);
  
  /* Write value to MEMS CLICK_CFG register */
  LIS3DSH_Write(&ctrl, LIS3DSH_CTRL_REG3, 1);
}

/**
  * @brief  Change to lowpower mode for LIS3DSH
  * @retval None
  */
void LIS3DSH_LowpowerCmd(void)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG1 register */
  LIS3DSH_Read(&tmpreg, LIS3DSH_CTRL_REG4, 1);
  
  /* Set new low power mode configuration */
  tmpreg &= (uint8_t)0x0F;
  tmpreg |= LIS3DSH_PWRDWN;
  
  /* Write value to MEMS CTRL_REG1 regsister */
  LIS3DSH_Write(&tmpreg, LIS3DSH_CTRL_REG4, 1);
}

/**
  * @brief  Data Rate command 
  * @param  DataRateValue: Data rate value
  *   This parameter can be one of the following values:
  *     @arg LIS3DSH_DATARATE_3_125	: 3.125 Hz output data rate 
  *     @arg LIS3DSH_DATARATE_6_25	: 6.25 	Hz output data rate
  *     @arg LIS3DSH_DATARATE_12_5	: 12.5	Hz output data rate
  *     @arg LIS3DSH_DATARATE_25		: 25 		Hz output data rate
  *     @arg LIS3DSH_DATARATE_50		: 50 		Hz output data rate
  *     @arg LIS3DSH_DATARATE_100		: 100 	Hz output data rate
  *     @arg LIS3DSH_DATARATE_400		: 400 	Hz output data rate
  *     @arg LIS3DSH_DATARATE_800		: 800 	Hz output data rate
  *     @arg LIS3DSH_DATARATE_1600	: 1600 	Hz output data rate


  * @retval None
  */
void LIS3DSH_DataRateCmd(uint8_t DataRateValue)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG1 register */
  LIS3DSH_Read(&tmpreg, LIS3DSH_CTRL_REG4, 1);
  
  /* Set new Data rate configuration */
  tmpreg &= (uint8_t)0x0F;
  tmpreg |= DataRateValue;
  
  /* Write value to MEMS CTRL_REG1 regsister */
  LIS3DSH_Write(&tmpreg, LIS3DSH_CTRL_REG4, 1);
}

/**
  * @brief  Change the Full Scale of LIS3DSH
  * @param  FS_value: new full scale value. 
  *   This parameter can be one of the following values:
  *     @arg LIS3DSH_FULLSCALE_2	: +-2g
  *     @arg LIS3DSH_FULLSCALE_4	: +-4g
  *     @arg LIS3DSH_FULLSCALE_6	: +-6g
  *     @arg LIS3DSH_FULLSCALE_8	: +-8g
  *     @arg LIS3DSH_FULLSCALE_16	: +-16g
  * @retval None
  */
void LIS3DSH_FullScaleCmd(uint8_t FS_value)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG1 register */
  LIS3DSH_Read(&tmpreg, LIS3DSH_CTRL_REG5, 1);
  
  /* Set new full scale configuration */
  tmpreg &= (uint8_t)0xC7;
  tmpreg |= FS_value;
  
  /* Write value to MEMS CTRL_REG1 regsister */
  LIS3DSH_Write(&tmpreg, LIS3DSH_CTRL_REG5, 1);
}

/**
  * @brief  Writes one byte to the LIS3DSH.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the LIS3DSH.
  * @param  WriteAddr : LIS3DSH's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void LIS3DSH_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  LIS3DSH_CS_LOW();
  
  /* Send the Address of the indexed register */
  LIS3DSH_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    LIS3DSH_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  LIS3DSH_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the LIS3DSH.
  * @param  pBuffer : pointer to the buffer that receives the data read from the LIS3DSH.
  * @param  ReadAddr : LIS3DSH's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the LIS3DSH.
  * @retval None
  */
void LIS3DSH_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  LIS3DSH_CS_LOW();
  
  /* Send the Address of the indexed register */
  LIS3DSH_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS3DSH (Slave device) */
    *pBuffer = LIS3DSH_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  LIS3DSH_CS_HIGH();
}

/**
  * @brief  Read LIS3DSH output register, and calculate the acceleration 
  *         ACC[mg]=SENSITIVITY* (out_h*256+out_l)/16 (12 bit rappresentation)
  * @param  s16 buffer to store data
  * @retval None
  */
void LIS3DSH_ReadACC(float* out)
{
  uint8_t buffer[6];
  uint8_t ctrl, i = 0x00;
	uint8_t offsetX, offsetY, offsetZ;
	int16_t aggregateResult = 0;
  
  LIS3DSH_Read(&offsetX, LIS3DSH_OFF_X, 1);
  LIS3DSH_Read(&offsetY, LIS3DSH_OFF_Y, 1);
  LIS3DSH_Read(&offsetZ, LIS3DSH_OFF_Z, 1);
	
  LIS3DSH_Read(&ctrl, LIS3DSH_CTRL_REG5, 1);  
	LIS3DSH_Read(&buffer[0], LIS3DSH_OUT_X_L, 1);
	LIS3DSH_Read(&buffer[1], LIS3DSH_OUT_X_H, 1);
	LIS3DSH_Read(&buffer[2], LIS3DSH_OUT_Y_L, 1);
	LIS3DSH_Read(&buffer[3], LIS3DSH_OUT_Y_H, 1);
	LIS3DSH_Read(&buffer[4], LIS3DSH_OUT_Z_L, 1);
	LIS3DSH_Read(&buffer[5], LIS3DSH_OUT_Z_H, 1);
  
	ctrl = (ctrl & 0x38) >> 3;
	
  switch(ctrl)
    {
    /* FS bits = 000 ==> Sensitivity typical value = 0.061 milligals/digit*/ 
    case 0x00:
      for(i=0; i<0x06; i=i+2)
      {
				aggregateResult = (int32_t)(buffer[i] | buffer[i+1] << 8);
        *out =(float)(LIS3DSH_SENSITIVITY_2G * (float)aggregateResult);
        out++;
      }
      break;
			
    /* FS bit = 001 ==> Sensitivity typical value = 0.122 milligals/digit*/ 
    case 0x01:
      for(i=0; i<0x06; i=i+2)
      {
				aggregateResult = (int32_t)(buffer[i] | buffer[i+1] << 8);
        *out =(float)(LIS3DSH_SENSITIVITY_4G * (float)aggregateResult);
        out++;
      }         
      break;
			
		/* FS bit = 010 ==> Sensitivity typical value = 0.183 milligals/digit*/ 
    case 0x02:
      for(i=0; i<0x06; i=i+2)
      {
				aggregateResult = (int32_t)(buffer[i] | buffer[i+1] << 8);
        *out =(float)(LIS3DSH_SENSITIVITY_6G * (float)aggregateResult);
        out++;
      }         
      break;
			
		 /* FS bit = 011 ==> Sensitivity typical value = 0.244 milligals/digit*/ 
    case 0x03:
      for(i=0; i<0x06; i=i+2)
      {
				aggregateResult = (int32_t)(buffer[i] | buffer[i+1] << 8);
        *out =(float)(LIS3DSH_SENSITIVITY_8G * (float)aggregateResult);
        out++;
      }         
      break;
			
		/* FS bit = 100 ==> Sensitivity typical value = 0.488 milligals/digit*/ 
    case 0x04:
      for(i=0; i<0x06; i=i+2)
      {
				aggregateResult = (int32_t)(buffer[i] | buffer[i+1] << 8);
        *out =(float)(LIS3DSH_SENSITIVITY_16G * (float)aggregateResult);
        out++;
      }         
      break;
			
    default:
      break;
    }
 }

/**
  * @brief  Initializes the low level interface used to drive the LIS3DSH
  * @param  None
  * @retval None
  */
static void LIS3DSH_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB2PeriphClockCmd(LIS3DSH_SPI_CLK, ENABLE);

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_AHB1PeriphClockCmd(LIS3DSH_SPI_SCK_GPIO_CLK | LIS3DSH_SPI_MISO_GPIO_CLK | LIS3DSH_SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable CS  GPIO clock */
  RCC_AHB1PeriphClockCmd(LIS3DSH_SPI_CS_GPIO_CLK, ENABLE);
  
  /* Enable INT1 GPIO clock */
  RCC_AHB1PeriphClockCmd(LIS3DSH_SPI_INT1_GPIO_CLK, ENABLE);
  
  /* Enable INT2 GPIO clock */
  RCC_AHB1PeriphClockCmd(LIS3DSH_SPI_INT2_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(LIS3DSH_SPI_SCK_GPIO_PORT, LIS3DSH_SPI_SCK_SOURCE, LIS3DSH_SPI_SCK_AF);
  GPIO_PinAFConfig(LIS3DSH_SPI_MISO_GPIO_PORT, LIS3DSH_SPI_MISO_SOURCE, LIS3DSH_SPI_MISO_AF);
  GPIO_PinAFConfig(LIS3DSH_SPI_MOSI_GPIO_PORT, LIS3DSH_SPI_MOSI_SOURCE, LIS3DSH_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = LIS3DSH_SPI_SCK_PIN;
  GPIO_Init(LIS3DSH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  LIS3DSH_SPI_MOSI_PIN;
  GPIO_Init(LIS3DSH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = LIS3DSH_SPI_MISO_PIN;
  GPIO_Init(LIS3DSH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(LIS3DSH_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(LIS3DSH_SPI, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(LIS3DSH_SPI, ENABLE);

  /* Configure GPIO PIN for Lis Chip select */
  GPIO_InitStructure.GPIO_Pin = LIS3DSH_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LIS3DSH_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GPIO_SetBits(LIS3DSH_SPI_CS_GPIO_PORT, LIS3DSH_SPI_CS_PIN);
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.GPIO_Pin = LIS3DSH_SPI_INT1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LIS3DSH_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = LIS3DSH_SPI_INT2_PIN;
  GPIO_Init(LIS3DSH_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t LIS3DSH_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  LIS3DSHTimeout = LIS3DSH_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(LIS3DSH_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((LIS3DSHTimeout--) == 0) return LIS3DSH_TIMEOUT_UserCallback();
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(LIS3DSH_SPI, byte);
  
  /* Wait to receive a Byte */
  LIS3DSHTimeout = LIS3DSH_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(LIS3DSH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((LIS3DSHTimeout--) == 0) return LIS3DSH_TIMEOUT_UserCallback();
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(LIS3DSH_SPI);
}


#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t LIS3DSH_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
//  while (1)
 // {   
  //}
	return 0;
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

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

