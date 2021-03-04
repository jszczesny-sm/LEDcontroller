#include "stm32f103xb.h"

#include "gpio_pin_cfg.h"
#include "delay.h"
#include "lcd.h"
#include "spi_conf.h"

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define LCD_DC			PC1
#define LCD_CE			PC2
#define LCD_RST			PC3


void send_char(char c){
	while (!(USART2->SR & USART_SR_TXE));

	USART2->DR = c;

}

void send_string(const char* s){
	while (*s)
	send_char(*s++);
}


int main(void){

	SysTick_Config(8000);

	RCC->APB2ENR = RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_SPI1EN;

	spi_init(1);

	//********** PINY **********
	gpio_pin_cfg(GPIOB, PB6, gpio_mode_output_PP_2MHz); // podświetlenie

	gpio_pin_cfg(GPIOC, LCD_DC, gpio_mode_output_PP_10MHz);
	gpio_pin_cfg(GPIOC, LCD_CE, gpio_mode_output_PP_10MHz);
	gpio_pin_cfg(GPIOC, LCD_RST, gpio_mode_output_PP_10MHz);


	//lcd setup
	BB(GPIOC->ODR, LCD_RST) = 1;
	delay_ms(10);
	BB(GPIOC->ODR, LCD_RST) = 0;
	delay_ms(10);
	BB(GPIOC->ODR, LCD_RST) = 1;
	BB(GPIOC->ODR, LCD_CE) = 1;
	BB(GPIOB->ODR, PB6) = 1;
	
	
	int i,j;
	lcd_setup();
	for(i=0; i<48; i++){
		for(j=0; j<84; j++){
			lcd_draw_pixel(j,i);
		}
			lcd_copy();
	}
	lcd_copy();
	
	//UART Setup
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	gpio_pin_cfg(GPIOA, PA2, gpio_mode_alternate_PP_2MHz);
	gpio_pin_cfg(GPIOA, PA3, gpio_mode_input_floating);

	gpio_pin_cfg(GPIOC, PC0, gpio_mode_output_PP_2MHz);
	gpio_pin_cfg(GPIOC, PC1, gpio_mode_output_PP_2MHz);
	gpio_pin_cfg(GPIOC, PC2, gpio_mode_output_PP_2MHz);

	USART2->BRR = 8000000/9600;
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	
	int size = 3;
	unsigned int index = 0;
	char *my_buffer;
	my_buffer = malloc(sizeof(char) * size);
	memset(my_buffer, 0, sizeof(char) * size);
	
	
	// turning a line on LCD
	while (1) {
		lcd_clear();
		lcd_draw_line(0, 0, LCD_WIDTH - 1, 0);
		lcd_draw_line(LCD_WIDTH - 1, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
		lcd_draw_line(LCD_WIDTH - 1, LCD_HEIGHT - 1, 0, LCD_HEIGHT - 1);
		lcd_draw_line(0, LCD_HEIGHT - 1, 0, 0);

		angle += 5;
		int x = 41.0 * sin(angle * M_PI / 180.0f);
		int y = 23.0 * cos(angle * M_PI / 180.0f);
		lcd_draw_line(LCD_WIDTH / 2 + x, LCD_HEIGHT / 2 - y, LCD_WIDTH / 2 - x, LCD_HEIGHT / 2 + y);

		lcd_copy();

		// UART controll bt console		
		if (USART2->SR & USART_SR_RXNE)	{
			USART2->SR &= ~USART_SR_RXNE;
			char c = USART2->DR;
			my_buffer[index++] = c;

			if(c == '\n' || c == '\r')
			{
				if(!strncmp(my_buffer, "on\r", 3) || !strncmp(my_buffer, "on\n", 3))
				{
					send_string("Power ON\n\r");
					BB(GPIOC->ODR, PC0) = 1;
				}
				else if(!strncmp(my_buffer, "off\r", 4) || !strncmp(my_buffer, "off\n", 4))
				{
					send_string("Power OFF\n\r");
					BB(GPIOC->ODR, PC0) = 0;
				}
				else {
					send_string("Blad\n\r");
				}
				memset(my_buffer, 0, sizeof(char) * size);
				index=0;
			}
		}
		delay_ms(100);
	}


	while (1) {

	}


} /* main */

