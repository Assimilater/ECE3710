#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "../Shared/bool.h"
#include "LCD.h"



void exec() {
	LCD_SetColumn(40,200);
	LCD_SetPage(20,110);
	LCD_WriteBlock(LCD_COLOR_RED, 2, 160*90);
	LCD_SetPage(125,215);
	LCD_WriteBlock(LCD_COLOR_GREEN, 2, 160*90);
	LCD_SetPage(230,320);
	LCD_WriteBlock(LCD_COLOR_YELLOW, 2, 160*90);
	
	LCD_SetColumn(50,190);
	LCD_SetPage(30,100);
	LCD_WriteBlock(LCD_COLOR_BLACK, 2, 140*70);
	LCD_SetPage(135,205);
	LCD_WriteBlock(LCD_COLOR_BLACK, 2, 140*70);
	LCD_SetPage(240,310);
	LCD_WriteBlock(LCD_COLOR_BLACK, 2, 140*70);
	
}

void init() {
	
}

int main() {
	init();
	exec();
	return 0;
}
