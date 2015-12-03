/**
  ******************************************************************************
  * @file    stm32f4_discovery_LSM9DS1.c
  * @author  Taylor Dotsikas, Wei-di Chang, inspired by MCD Application Team lis302dl driver
  * @version V1.1.0
  * @date    28-October-2011
  * @brief   This file provides a set of functions needed to manage the LSM9DS1
  *          MEMS accelerometer and gyroscope.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lsm9ds1.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY_LSM9DS1
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Private_Defines
  * @{
  */
__IO uint32_t  LSM9DS1Timeout = LSM9DS1_FLAG_TIMEOUT;   

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_LSM9DS1_Private_FunctionPrototypes
  * @{
  */
static uint8_t LSM9DS1_SendByte(uint8_t byte);
static void LSM9DS1_LowLevel_Init(void);
/**
  * @}
  */

/**
  * @brief  Set LSM9DS1 Initialization.
  * @param  LSM9DS1_Config_Struct: pointer to a LSM9DS1_Config_TypeDef structure 
  *         that contains the configuration setting for the LSM9DS1. Also enable interrupts
  * @retval None
  */
void LSM9DS1_Init(LSM9DS1_InitTypeDef *LSM9DS1_InitStruct)
{
  
  /* Configure the low level interface ---------------------------------------*/
  LSM9DS1_LowLevel_Init();
	
	uint8_t ctrl6 = 0x00;
  
  /* Configure Linear Acceleration Sensor Control Register 6 */
  ctrl6 = (uint8_t) (LSM9DS1_InitStruct->Power_Mode_Output_DataRate | \
                    LSM9DS1_InitStruct->Full_Scale | LSM9DS1_InitStruct->Bandwidth_Selection | \
                    LSM9DS1_InitStruct->Anti_Aliasing_Filter_BW);
  
  /* Write value to CTRL_REG6_XL regsister */
  LSM9DS1_Write(&ctrl6, LSM9DS1_CTRL_REG6_XL, 1);
	
	uint8_t ctrl1 = 0x00;
	ctrl1 = (uint8_t) (LSM9DS1_InitStruct->G_Power_Mode_Output_DataRate | LSM9DS1_InitStruct->G_Full_Scale);
	LSM9DS1_Write(&ctrl1, LSM9DS1_CTRL_REG1_G, 1);
	
	uint8_t ctrl5 = 0x00;
	
	/* Configure Linear Acceleration Sensor Control Register 5 */
	ctrl5 = (uint8_t)	(LSM9DS1_InitStruct->XL_Axes_Enable); 
	
	/* Write value to CTRL_REG5_XL regsister */
  LSM9DS1_Write(&ctrl5, LSM9DS1_CTRL_REG5_XL, 1);
	
	
	uint8_t ctrl4 = 0x00;
	
	/* Configure Gyroscope Sensor Control Register 4 */
	ctrl4 = (uint8_t) (LSM9DS1_InitStruct->G_Axes_Enable); 
	
	/* Write value to CTRL_REG4_G register */
	LSM9DS1_Write(&ctrl4, LSM9DS1_CTRL_REG4, 1);
	
  uint8_t tmpreg;
  tmpreg = (uint8_t)0x03;
	/* Enable Interrupts on INT1 for Accelerometer and Gyroscope */
	LSM9DS1_Write(&tmpreg, LSM9DS1_INT1_CTRL, 1);
}

/**
  * @brief  Writes one byte to the LSM9DS1.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the LSM9DS1.
  * @param  WriteAddr : LSM9DS1's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void LSM9DS1_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
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
  LSM9DS1_CS_LOW();
  
  /* Send the Address of the indexed register */
  LSM9DS1_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    LSM9DS1_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  LSM9DS1_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the LSM9DS1.
  * @param  pBuffer : pointer to the buffer that receives the data read from the LSM9DS1.
  * @param  ReadAddr : LSM9DS1's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the LSM9DS1.
  * @retval None
  */
void LSM9DS1_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
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
  LSM9DS1_CS_LOW();
  
  /* Send the Address of the indexed register */
  LSM9DS1_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LSM9DS1 (Slave device) */
    *pBuffer = LSM9DS1_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  LSM9DS1_CS_HIGH();
}

/**
  * @brief  Read LSM9DS1 output register, and calculate the acceleration 
  * @param  s16 buffer to store data
  * @retval None
  */
void LSM9DS1_ReadXL(int32_t* out)
{
  uint8_t buffer[6];
  uint8_t crtl, i = 0x00;
	
  LSM9DS1_Read(&crtl, LSM9DS1_CTRL_REG6_XL, 1);  //ctrl will recieve the data from the reg
	
	//Read axis data
	LSM9DS1_Read(buffer, LSM9DS1_OUT_X_H_XL, 1);
	LSM9DS1_Read(buffer+1, LSM9DS1_OUT_X_L_XL, 1);
	
	LSM9DS1_Read(buffer+2, LSM9DS1_OUT_Y_H_XL, 1);
	LSM9DS1_Read(buffer+3, LSM9DS1_OUT_Y_L_XL, 1);
	
	LSM9DS1_Read(buffer+4, LSM9DS1_OUT_Z_H_XL, 1);
	LSM9DS1_Read(buffer+5, LSM9DS1_OUT_Z_L_XL, 1);	
	
	//buffer contains (x,y,z) coordinates 2bytes each. 
  //And the ctrl reg with 0x18 to get the Full Scale Selection code
	//See what sensitivity the accelerometer is configured at and alter the buffer contents
	
  switch(crtl & 0x18) 
    {
   
			case LSM9DS1_FULLSCALE_2G:
      for(i=0; i<6; i+=2)
      {
        *out =(int32_t)(LSM9DS1_SENSITIVITY_2G *  (int16_t)(buffer[i+1]+(buffer[i]<<8)));
        out++;
      }
      break;
			
			case LSM9DS1_FULLSCALE_4G:
      for(i=0; i<6; i+=2)
      {
        *out =(int32_t)(LSM9DS1_SENSITIVITY_4G *  (int16_t)(buffer[i+1]+(buffer[i]<<8)));
        out++;
      }
      break;
			
			case LSM9DS1_FULLSCALE_8G:
      for(i=0; i<6; i+=2)
      {
        *out =(int32_t)(LSM9DS1_SENSITIVITY_8G *  (int16_t)(buffer[i+1]+(buffer[i]<<8)));
        out++;
      }
      break;
			
			case LSM9DS1_FULLSCALE_16G:
      for(i=0; i<6; i+=2)
      {
        *out =(int32_t)(LSM9DS1_SENSITIVITY_16G *  (int16_t)(buffer[i+1]+(buffer[i]<<8)));
        out++;
      }
      break;
			


    default:
      break;
    }
 }

void LSM9DS1_ReadG(int32_t* out)
{
  uint8_t buffer[6];
  uint8_t crtl, i = 0x00;
   
  LSM9DS1_Read(&crtl, LSM9DS1_CTRL_REG1_G, 1);  //ctrl will recieve the data from the reg
	
	//Read axis data
	LSM9DS1_Read(buffer, LSM9DS1_OUT_X_H_G, 1);
	LSM9DS1_Read(buffer+1, LSM9DS1_OUT_X_L_G, 1);
	
	LSM9DS1_Read(buffer+2, LSM9DS1_OUT_Y_H_G, 1);
	LSM9DS1_Read(buffer+3, LSM9DS1_OUT_Y_L_G, 1);
	
	LSM9DS1_Read(buffer+4, LSM9DS1_OUT_Z_H_G, 1);
	LSM9DS1_Read(buffer+5, LSM9DS1_OUT_Z_L_G, 1);	
	
	//buffer contains (x,y,z) coordinates 2bytes each. 
  //And the ctrl reg with 0x18 to get the Full Scale Selection code
	//See what sensitivity the accelerometer is configured at and alter the buffer contents
	
  switch(crtl & 0x18) 
    {
   
			case LSM9DS1_FS_G_245:
      for(i=0; i<6; i+=2)
      {
        *out =(int32_t)(LSM9DS1_G_SENSITIVITY_245  *  (int16_t)(buffer[i+1]+(buffer[i]<<8)));
        out++;
      }
      break;
			
			case LSM9DS1_FS_G_500:
      for(i=0; i<6; i+=2)
      {
        *out =(int32_t)(LSM9DS1_G_SENSITIVITY_500  *  (int16_t)(buffer[i+1]+(buffer[i]<<8)));
        out++;
      }
      break;
			
			case LSM9DS1_FS_G_2000:
      for(i=0; i<6; i+=2)
      {
        *out =(int32_t)(LSM9DS1_G_SENSITIVITY_2000  *  (int16_t)(buffer[i+1]+(buffer[i]<<8)));
        out++;
      }
      break;
			


    default:
      break;
    }
 } 
 

/**
  * @brief  Initializes the low level interface used to drive the LSM9DS1
  * @param  None
  * @retval None
  */
static void LSM9DS1_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB1PeriphClockCmd(LSM9DS1_SPI_CLK, ENABLE);

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_AHB1PeriphClockCmd(LSM9DS1_SPI_SCK_GPIO_CLK | LSM9DS1_SPI_MISO_GPIO_CLK | LSM9DS1_SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable CS  GPIO clock */
  RCC_AHB1PeriphClockCmd(LSM9DS1_SPI_CS_GPIO_CLK, ENABLE);
  
  /* Enable INT1 GPIO clock */
  RCC_AHB1PeriphClockCmd(LSM9DS1_SPI_INT1_GPIO_CLK, ENABLE);
  
  /* Enable INT2 GPIO clock */
  RCC_AHB1PeriphClockCmd(LSM9DS1_SPI_INT2_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(LSM9DS1_SPI_SCK_GPIO_PORT, LSM9DS1_SPI_SCK_SOURCE, LSM9DS1_SPI_SCK_AF);
  GPIO_PinAFConfig(LSM9DS1_SPI_MISO_GPIO_PORT, LSM9DS1_SPI_MISO_SOURCE, LSM9DS1_SPI_MISO_AF);
  GPIO_PinAFConfig(LSM9DS1_SPI_MOSI_GPIO_PORT, LSM9DS1_SPI_MOSI_SOURCE, LSM9DS1_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = LSM9DS1_SPI_SCK_PIN;
  GPIO_Init(LSM9DS1_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  LSM9DS1_SPI_MOSI_PIN;
  GPIO_Init(LSM9DS1_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = LSM9DS1_SPI_MISO_PIN;
  GPIO_Init(LSM9DS1_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(LSM9DS1_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(LSM9DS1_SPI, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(LSM9DS1_SPI, ENABLE);

  /* Configure GPIO PIN for Lis Chip select */
  GPIO_InitStructure.GPIO_Pin = LSM9DS1_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LSM9DS1_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GPIO_SetBits(LSM9DS1_SPI_CS_GPIO_PORT, LSM9DS1_SPI_CS_PIN);
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.GPIO_Pin = LSM9DS1_SPI_INT1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LSM9DS1_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = LSM9DS1_SPI_INT2_PIN;
  GPIO_Init(LSM9DS1_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t LSM9DS1_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  LSM9DS1Timeout = LSM9DS1_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(LSM9DS1_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((LSM9DS1Timeout--) == 0) return LSM9DS1_TIMEOUT_UserCallback();
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(LSM9DS1_SPI, byte);
  
  /* Wait to receive a Byte */
  LSM9DS1Timeout = LSM9DS1_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(LSM9DS1_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((LSM9DS1Timeout--) == 0) return LSM9DS1_TIMEOUT_UserCallback();
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(LSM9DS1_SPI);
}

//#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t LSM9DS1_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  while (1)
  {   
  }
}
//#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

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
