#include "stm32f103xb.h"
#include "gpio_pin_cfg.h"
#include "delay.h"

void spi_init(uint8_t spix){

	if(spix == 1){
		gpio_pin_cfg(GPIOA, PA5, gpio_mode_alternate_PP_50MHz); /* SCK */
		gpio_pin_cfg(GPIOA, PA6, gpio_mode_input_floating); /* MISO */
		gpio_pin_cfg(GPIOA, PA7, gpio_mode_alternate_PP_50MHz); /* MOSI */

		SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_1;
		delay_ms(10);
		SPI1->CR1 |= SPI_CR1_SPE;
	}
	if(spix == 2){
		gpio_pin_cfg(GPIOA, PB13, gpio_mode_alternate_PP_50MHz); /* SCK */
		gpio_pin_cfg(GPIOA, PB14, gpio_mode_input_floating); /* MISO */
		gpio_pin_cfg(GPIOA, PB15, gpio_mode_alternate_PP_50MHz); /* MOSI */

		SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_1;
		delay_ms(10);
		SPI2->CR1 |= SPI_CR1_SPE;
	}
}

uint16_t spi_sendrecv(uint8_t spix, uint8_t data){
	if(spix == 1){
		while( !(SPI1->SR & SPI_SR_TXE) );
		SPI1->DR = data;
		while( !(SPI1->SR & SPI_SR_RXNE) );
		data = SPI1->DR;
		return data;
	}
	if(spix == 2){
		while( !(SPI2->SR & SPI_SR_TXE) );
		SPI2->DR = data;
		while( !(SPI2->SR & SPI_SR_RXNE) );
		data = SPI2->DR;
		return data;
	}
	return data;
}
