#ifndef _LCD_H_
#define _LCD_H_

#include <p18f4620.h>

#define LCD_RS			2 		//define MCU pin connected to LCD RS
//#define LCD_RW			 	//define MCU pin connected to LCD R/W
#define LCD_E			3		//define MCU pin connected to LCD E
#define LCD_D4			4		//define MCU pin connected to LCD D3
#define LCD_D5			5		//define MCU pin connected to LCD D4
#define LCD_D6			6		//define MCU pin connected to LCD D5
#define LCD_D7			7		//define MCU pin connected to LCD D6

#define LCD_LINE0		0x00
#define LCD_LINE1		0x40

#define CLR     0
#define SET     1
#define DAT 1	// data mode
#define CMD 0	// command mode
#define READ 1	// read mode
#define WRITE 0	// write mode

#define LCD_PORT 	PORTB
#define LCD_DDR 	TRISB

//code danh cho 4 bit cao
#define RS(x)			( (x) ? ( LCD_PORT |= (1<<LCD_RS) ) : ( LCD_PORT &= ~(1<<LCD_RS) ) )
#define EN(x)			( (x) ? ( LCD_PORT |= (1<<LCD_E) ) : ( LCD_PORT &= ~(1<<LCD_E) ) )

#define LCD_DATA_OUT(x)         LCD_PORT &= 0x0F; LCD_PORT |= (x);
#define LCD_DATA_IN             LCD_PORT & 0x0F;
#define LCD_DIR_IN              LCD_PORT |= 0x0F;

void lcd_delay(unsigned char time);
void init_lcd();
void lcd_write_4bits(unsigned char dat);
void lcd_write_cmd(unsigned char cmd);
void lcd_write_data(unsigned char data);
void lcd_print_char(char c);
void lcd_putchar (char c);
void lcd_print_str(const rom unsigned char* string);
void lcd_print_num(long num);
void lcd_set_cursor(unsigned char row, unsigned char column);
void lcd_clear();
void LcdPrintNum(unsigned char x, unsigned char y, long num);
void LcdPrintString(unsigned char x, unsigned char y, unsigned char* string);

void lcd_print_charS(unsigned char c);
void LcdPrintCharS(unsigned char x, unsigned char y,unsigned char c);
void LcdClearS();
void DisplayLcdScreen();
void DisplayLcdScreen2();
void LcdPrintNumS(unsigned char x, unsigned char y, long num);
void LcdPrintStringS(unsigned char x, unsigned char y, const rom  unsigned char* string);

#endif 
