#ifndef LCD_H_
#define LCD_H_

#include <string.h>
#include "gpio_pin_cfg.h"
#include "font.h"
#include "delay.h"
#include "spi_conf.h"


#define LCD_WIDTH		84
#define LCD_HEIGHT		48

#define LCD_DC			PC1
#define LCD_CE			PC2
#define LCD_RST			PC3

/*
  	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_SPI1EN;
 	spi_init(1);
*/
void lcd_init(void);
void lcd_setup(void);

void lcd_clear(void);
void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_pixel(int x, int y);
void lcd_draw_text(int row, int col, const char* text);
void lcd_draw_line(int x1, int y1, int x2, int y2);

void lcd_copy(void);

#endif /* LCD_H_ */
