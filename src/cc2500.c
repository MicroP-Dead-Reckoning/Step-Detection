#include "cc2500.h"
#include <stdio.h>
/* defines */
/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/* macros */
#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)

/* helper functions */
static uint8_t CC2500_SendByte(uint8_t byte);
void CC2500_REG_INIT(void);
uint8_t CC2500_Read_SRX(void);
uint8_t CC2500_Read_STX(void);

/* source */
int CC2500_SPI_INIT(CC2500_TXRX_MODE mode) {
	GPIO_InitTypeDef gpio_init_s;
  SPI_InitTypeDef  spi_init_s;
	
	/* Enable SPI */
	//Enable the SPI periph
  RCC_APB1PeriphClockCmd(CC2500_SPI_CLK, ENABLE);

  // Enable SCK, MOSI and MISO GPIO clocks
  RCC_AHB1PeriphClockCmd(CC2500_SPI_SCK_GPIO_CLK | CC2500_SPI_MISO_GPIO_CLK | CC2500_SPI_MOSI_GPIO_CLK, ENABLE);

  // Enable CS GPIO clock
  RCC_AHB1PeriphClockCmd(CC2500_SPI_CS_GPIO_CLK, ENABLE);
	
	/* GPIO Config */
	GPIO_PinAFConfig(CC2500_SPI_SCK_GPIO_PORT, CC2500_SPI_SCK_SOURCE, CC2500_SPI_SCK_AF);
  GPIO_PinAFConfig(CC2500_SPI_MISO_GPIO_PORT, CC2500_SPI_MISO_SOURCE, CC2500_SPI_MISO_AF);
  GPIO_PinAFConfig(CC2500_SPI_MOSI_GPIO_PORT, CC2500_SPI_MOSI_SOURCE, CC2500_SPI_MOSI_AF);

	gpio_init_s.GPIO_Mode =  GPIO_Mode_AF;
  gpio_init_s.GPIO_OType = GPIO_OType_PP;
  gpio_init_s.GPIO_PuPd  = GPIO_PuPd_DOWN;
  gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* SPI SCK pin configuration */
  gpio_init_s.GPIO_Pin = CC2500_SPI_SCK_PIN;
  GPIO_Init(CC2500_SPI_SCK_GPIO_PORT, &gpio_init_s);

  /* SPI  MOSI pin configuration */
  gpio_init_s.GPIO_Pin =  CC2500_SPI_MOSI_PIN;
  GPIO_Init(CC2500_SPI_MOSI_GPIO_PORT, &gpio_init_s);

  /* SPI MISO pin configuration */
  gpio_init_s.GPIO_Pin = CC2500_SPI_MISO_PIN;
  GPIO_Init(CC2500_SPI_MISO_GPIO_PORT, &gpio_init_s);
	
	/* SPI configuration */
  SPI_I2S_DeInit(CC2500_SPI);
  spi_init_s.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi_init_s.SPI_DataSize = SPI_DataSize_8b;
  spi_init_s.SPI_CPOL = SPI_CPOL_Low;
  spi_init_s.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init_s.SPI_NSS = SPI_NSS_Soft;
  spi_init_s.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;	// Reduces GPIO clock to 5.25 MHz
  spi_init_s.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init_s.SPI_CRCPolynomial = 7;
  spi_init_s.SPI_Mode = SPI_Mode_Master;
  SPI_Init(CC2500_SPI, &spi_init_s);
	
	/* Enable SPI */
	SPI_Cmd(CC2500_SPI, ENABLE);
	
	/* Configure GPIO PIN for Lis Chip select */
  gpio_init_s.GPIO_Pin = CC2500_SPI_CS_PIN;
  gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init_s.GPIO_OType = GPIO_OType_PP;
  gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &gpio_init_s);
	
	/* Deselect : Chip Select high */
  GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN);
	
	/* setup registers */
	CC2500_REG_INIT();
	if(mode == CC2500_RECV){
		CC2500_Read_SRX();
	}else if(mode == CC2500_TRANS){
		CC2500_Read_STX();
	}
	return 0;
}

/*!
	Sets up the register of the CC2500 with te required values
 */
void CC2500_REG_INIT(void) {
	// burst write data
	uint8_t FREQ[3] = {0x5D, 0x94, 0x02};
	uint8_t to_write_array[6] = {VAL_CC2500_MDMCFG4, VAL_CC2500_MDMCFG3,VAL_CC2500_MDMCFG2,
															 VAL_CC2500_MDMCFG1, VAL_CC2500_MDMCFG0, VAL_CC2500_DEVIATN};
	uint8_t to_write_array_7[7] = {VAL_CC2500_MCSM1,VAL_CC2500_MCSM0,VAL_CC2500_FOCCFG,
															   VAL_CC2500_BSCFG,VAL_CC2500_AGCTRL2,VAL_CC2500_AGCTRL1,VAL_CC2500_AGCTRL0};
	uint8_t frend[2] = {VAL_CC2500_FREND1, VAL_CC2500_FREND0};
	uint8_t fscal[4] = {VAL_CC2500_FSCAL3, VAL_CC2500_FSCAL2, VAL_CC2500_FSCAL1, VAL_CC2500_FSCAL0};
	uint8_t test[3] = {VAL_CC2500_TEST2, VAL_CC2500_TEST1, VAL_CC2500_TEST0};\
	
	/* reset chip */
	uint8_t tmp = DUMMY_BYTE;
	CC2500_Read(&tmp, CC2500_PARTNAME, 1);
	for (int i=0; i < 1000000; i++);
	
	/* set registers */
	uint8_t to_write = VAL_CC2500_IOCFG2;
	CC2500_Write(&to_write,0x00, 1);
	to_write = VAL_CC2500_IOCFG1;
	CC2500_Write(&to_write, 0x01, 1);
	to_write = VAL_CC2500_IOCFG0;
	CC2500_Write(&to_write, 0x02, 1);
	to_write = VAL_CC2500_FIFOTHR;
	CC2500_Write(&to_write, 0x03, 1);
	to_write = VAL_CC2500_PKTLEN;
	CC2500_Write(&to_write, 0x06, 1);
	to_write = VAL_CC2500_PKTCTRL1;
	CC2500_Write(&to_write, 0x07, 1);
	to_write = VAL_CC2500_PKTCTRL0;
	CC2500_Write(&to_write, 0x08, 1);
	to_write = VAL_CC2500_ADDR;
	CC2500_Write(&to_write, 0x09, 1);
	to_write = VAL_CC2500_CHANNR;
	CC2500_Write(&to_write, 0x0A, 1);
	to_write = VAL_CC2500_FSCTRL1;
	CC2500_Write(&to_write, 0x0B, 1);
	to_write = VAL_CC2500_FSCTRL0;
	CC2500_Write(&to_write, 0x0C, 1);
	CC2500_Write(FREQ, CC2500_FREQ_REG, 3);

	CC2500_Write(to_write_array, 0x10, 6);

	CC2500_Write(to_write_array_7, 0x17, 7);
	
	CC2500_Write(frend, 0x21, 2);
	
	CC2500_Write(fscal, 0x23, 4);
													
	CC2500_Write(test, 0x2C, 3);
	to_write = VAL_CC2500_FSTEST;
	CC2500_Write(&to_write, 0x29, 1);
}


void CC2500_INT_INIT(void) {
	
		GPIO_InitTypeDef gpio_init_s;
		EXTI_InitTypeDef exti_init_s;
		NVIC_InitTypeDef nvic_init_s;

		//enable GPIOE
		RCC_AHB1PeriphClockCmd(CC2500_SPI_INT1_GPIO_CLK, ENABLE);
		//enable clock for SYSCFG
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


		gpio_init_s.GPIO_Mode = GPIO_Mode_IN;
		gpio_init_s.GPIO_Pin = CC2500_SPI_INT1_PIN;
		gpio_init_s.GPIO_PuPd = GPIO_PuPd_UP; // Force low from start
		gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(CC2500_SPI_INT1_GPIO_PORT, &gpio_init_s);

		SYSCFG_EXTILineConfig(CC2500_SPI_INT1_EXTI_PORT_SOURCE, CC2500_SPI_INT1_EXTI_PIN_SOURCE); 
		// setup external interupts
		exti_init_s.EXTI_Line = CC2500_SPI_INT1_EXTI_LINE;
		exti_init_s.EXTI_Mode = EXTI_Mode_Interrupt;
		exti_init_s.EXTI_Trigger = EXTI_Trigger_Falling;
		exti_init_s.EXTI_LineCmd = ENABLE;

		EXTI_Init(&exti_init_s);

		// setup NVIC for accelerometer
		nvic_init_s.NVIC_IRQChannel = CC2500_SPI_INT1_EXTI_IRQn; //
		nvic_init_s.NVIC_IRQChannelPreemptionPriority = 0x00;
		nvic_init_s.NVIC_IRQChannelSubPriority = 0x00;
		nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic_init_s);
}

static uint8_t CC2500_SendByte(uint8_t byte)
{
	uint32_t  CC2500Timeout;
	
  /* Loop while DR register in not emplty */
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((CC2500Timeout--) == 0) return 69;	// TODO
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(CC2500_SPI, byte);
  
  /* Wait to receive a Byte */
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((CC2500Timeout--) == 0) return 69;
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(CC2500_SPI);
}

void CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t) READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  CC2500_CS_LOW();
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS3DSH (Slave device) */
    *pBuffer = CC2500_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}

void CC2500_Read_SR(uint8_t* pBuffer, uint8_t ReadAddr)
{  
	ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  
	/* Set chip select Low at the start of the transmission */
  CC2500_CS_LOW();
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  /* Send dummy byte (0x00) to generate the SPI clock to LIS3DSH (Slave device) */
	*pBuffer = CC2500_SendByte(DUMMY_BYTE);
  
	/* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}

void CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
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
  CC2500_CS_LOW();
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    CC2500_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}

inline void CC2500_Read_RX(uint8_t* pBuffer, uint16_t NumByteToRead) {
	//CC2500_Read_SRX();
	CC2500_Read(pBuffer, CC2500_FIFO_REG, NumByteToRead);
}

inline uint8_t CC2500_Read_SRX(void) {
	uint8_t tmp = DUMMY_BYTE;
	CC2500_Read(&tmp, CC2500_SRX_REG, 1);
	return tmp;
}

inline uint8_t CC2500_Read_STX(void) {
	uint8_t tmp = DUMMY_BYTE;
	CC2500_Read(&tmp, CC2500_STX_REG, 1);
	return tmp;
}
