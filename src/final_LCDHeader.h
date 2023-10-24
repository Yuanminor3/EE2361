/* 
 * File:   final_LCDHeader.h
 * Author: yuan
 *
 * Created on April 15, 2022, 2:15 PM
 */


#ifndef FINAL_LCDHEADER_H
#define	FINAL_LCDHEADER_H

#ifdef	__cplusplus
extern "C" {
#endif
void delay_ms(unsigned int ms);
void lcd_cmd(char Package);
void lcd_init();
void lcd_setCursor(char x, char y);
void lcd_printChar(char Package);   
void lcd_printStr(const char *s);

#ifdef	__cplusplus
}
#endif

#endif	/* JIAN0655_LCDHEADERLAB6_V001_H */



