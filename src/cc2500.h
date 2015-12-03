/**
  ******************************************************************************
  * @file    cc2500.h
  * @author  Jit Kanetkar, Stephen Carter / Based on the LIS3DSH driver by TA Ashraf
  * @version V1.0.0
  * @date    11th-November-2015
  * @brief   Used to communicate with the CC2500 wireless chip. Uses SPI2.
  ******************************************************************************
	**/
#ifndef _INCLUDE_CC2500_H_
#define _INCLUDE_CC2500_H_

/* includes */
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

/* data structures */

/* enums */
typedef enum {
	CC2500_PARTNAME = 0x30,
	CC2500_VERSION = 0x31,
	CC2500_FREQ_REG = 0x0D,
	CC2500_SRX_REG = 0x34,
	CC2500_STX_REG = 0x35,
	CC2500_SIDLE_REG = 0x36,
	CC2500_TXBYTES_REG = 0x3A,
	CC2500_RXBYTES_REG = 0x3B,
	CC2500_FIFO_REG = 0x3F,
} CC2500_REG;

typedef enum {
	CC2500_SPI_CLK = GPIO_Pin_13,
	CC2500_SPI_MISO = GPIO_Pin_14,
	CC2500_SPI_MOSI = GPIO_Pin_15,
	CC2500_SPI_CS = GPIO_Pin_12
}	CC2500_SPI_Pin;

typedef enum {
	CC2500_RECV,
	CC2500_TRANS,
}CC2500_TXRX_MODE;

/* defines */
#define CC2500_FLAG_TIMEOUT 						 ((uint32_t)0x1000)

#define CC2500_SPI                       SPI2
#define CC2500_SPI_CLK                   RCC_APB1Periph_SPI2

#define CC2500_SPI_SCK_PIN               GPIO_Pin_13                 /* PB.13 */
#define CC2500_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource13
#define CC2500_SPI_SCK_AF                GPIO_AF_SPI2

#define CC2500_SPI_MISO_PIN              GPIO_Pin_14                 /* PB.14 */
#define CC2500_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource14
#define CC2500_SPI_MISO_AF               GPIO_AF_SPI2

#define CC2500_SPI_MOSI_PIN              GPIO_Pin_15                 /* PB.15 */
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource15
#define CC2500_SPI_MOSI_AF               GPIO_AF_SPI2

#define CC2500_SPI_CS_PIN                GPIO_Pin_12                 /* PB.12 */
#define CC2500_SPI_CS_GPIO_PORT          GPIOB                       /* GPIOB */
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB

#define CC2500_SPI_INT1_PIN              GPIO_Pin_11                  /* PB.11 */
#define CC2500_SPI_INT1_GPIO_PORT        GPIOB                       /* GPIOB */
#define CC2500_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define CC2500_SPI_INT1_EXTI_LINE        EXTI_Line11
#define CC2500_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOB
#define CC2500_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource11
#define CC2500_SPI_INT1_EXTI_IRQn        EXTI15_10_IRQn 

#define CC2500_CARRIER_FREQ 2433.056 //MHz	

#define VAL_CC2500_IOCFG2 	0x29
#define VAL_CC2500_IOCFG1   0x2E
#define VAL_CC2500_IOCFG0 	0x06	

#define VAL_CC2500_FIFOTHR 	0x07

#define VAL_CC2500_PKTLEN 	0x01

#define VAL_CC2500_PKTCTRL1 0x08	
#define VAL_CC2500_PKTCTRL0 0x04	

#define VAL_CC2500_ADDR 		0x00	

#define VAL_CC2500_CHANNR 	0x00

#define VAL_CC2500_FSCTRL1 	0x0C	
#define VAL_CC2500_FSCTRL0 	0x00	

// freq set to 2.433056 GHz, note f_xosc = 26MHz
#define VAL_CC2500_FREQ2 		0x5D	
#define VAL_CC2500_FREQ1 		0x94	
#define VAL_CC2500_FREQ0 		0x3E	

#define VAL_CC2500_MDMCFG4 	0x0E
#define VAL_CC2500_MDMCFG3 	0x3B
#define VAL_CC2500_MDMCFG2 	0x73	
#define VAL_CC2500_MDMCFG1 	0x42	
#define VAL_CC2500_MDMCFG0 	0xF8	

#define VAL_CC2500_DEVIATN 	0x00	


#define VAL_CC2500_MCSM1 		0x0E	
#define VAL_CC2500_MCSM0 		0x18	

#define VAL_CC2500_FOCCFG 	0x1D	
#define VAL_CC2500_BSCFG 		0x1C	

#define VAL_CC2500_AGCTRL2 	0xC7	 
#define VAL_CC2500_AGCTRL1 	0x40	
#define VAL_CC2500_AGCTRL0 	0xB0

#define VAL_CC2500_FREND1 	0xB6	 
#define VAL_CC2500_FREND0 	0x10	

#define VAL_CC2500_FSCAL3 	0xEA	 
#define VAL_CC2500_FSCAL2 	0x0A	
#define VAL_CC2500_FSCAL1 	0x00	 
#define VAL_CC2500_FSCAL0 	0x19	

#define VAL_CC2500_FSTEST 	0x59

#define VAL_CC2500_TEST2 		0x88	
#define VAL_CC2500_TEST1 		0x31	
#define VAL_CC2500_TEST0 		0x0B	


/* functions */
int CC2500_SPI_INIT(CC2500_TXRX_MODE mode);
void CC2500_INT_INIT(void);
void CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void CC2500_Read_SR(uint8_t* pBuffer, uint8_t ReadAddr);
void CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void CC2500_Read_RX(uint8_t* pBuffer, uint16_t NumByteToRead);
#endif // _INCLUDE_CC2500_H_
