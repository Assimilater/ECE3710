#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "../Shared/bool.h"
#include "LCD.h"

void exec() {
	LCD_SetColumn(40,200);
	LCD_SetPage(20,110);
	//LCD_WriteBlock(red);
	LCD_SetPage(130,220);
	//LCD_WriteBlock(green);
	LCD_SetPage(230,320);
	//LCD_WriteBlockyellow);
	
}

void init() {
	
}

int main() {
	init();
	exec();
	return 0;
}
