#ifndef SPI_CONF_H_
#define SPI_CONF_H_

/*
 * Remember to enable clock: RCC->APB2ENR |= RCC_APB2ENR_AFIOEN
  		SPI1 --> RCC->APB2ENR |= RCC_APB2ENR_SPI1EN
 		SPI2 --> RCC->APB1ENR |= RCC_APB2ENR_SPI2EN

 **** SPI_1 *****
 	PA4 --> NSS
	PA5 --> SCK
	PA6 --> MISO
	PA7 --> MOSI

 **** SPI_2 *****
 	PB12 --> NSS
	PB13 --> SCK
	PB14 --> MISO
	PB15 --> MOSI
*/

void spi_init(uint8_t spix);
uint16_t spi_sendrecv(uint8_t spix, uint8_t data);

#endif /* SPI_CONF_H_ */
