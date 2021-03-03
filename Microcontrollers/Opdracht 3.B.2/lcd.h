
/*
 * lcd.h
 *
 * Created: 21/02/2021 14:11:09
 *  Author: fabia
 */ 

#ifndef LCD_H
#define LCD_H

void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void lcd_clear(void);
void set_cursor(int position);
#endif
