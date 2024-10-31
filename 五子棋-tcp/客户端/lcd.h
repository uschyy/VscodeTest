#ifndef   __LCD_H__
#define   __LCD_H__
#include "macro.h"

void lcd_init(void);

void lcd_draw_point(int x0, int y0, int color);

void lcd_uninit(void);

void lcd_draw_bmp(const char *bmpname, int x0, int y0);



void clear_screen(int x0, int y0, int w, int h, int color);

void Fixed_botton_printing();

#endif 

