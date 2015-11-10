#include "LCD.h"

void LCD_WriteCmd(unsigned char cmd) {
	GPIO.PortD->DATA.bit2 = 0; // CSX "LCD, pay attention!"
	GPIO.PortD->DATA.bit3 = 0; // DCX cmd
	GPIO.PortD->DATA.bit6 = 0; // WRX
	GPIO.PortB->DATA.byte[0] = cmd;
	GPIO.PortD->DATA.bit6 = 1; // WRX read on +edge	
}
void LCD_WriteData(unsigned char* data, int len) {
	int i;
	GPIO.PortD->DATA.bit3 = 1; // DCX Data
	for (i = 0; i < len; ++i) {
		// LCD_WriteData(data[i]);
		GPIO.PortD->DATA.bit6 = 0; // WRX
		GPIO.PortB->DATA.byte[0] = data[i];
		GPIO.PortD->DATA.bit6 = 1; // WRX read on +edge
	}
	GPIO.PortD->DATA.bit2 = 0; // CSX "LCD, pay attention!"

}
void LCD_WriteBlock(unsigned char* data, int len, int n) {
	int i, j;
	for (j = 0; j < n; ++j) {
		for (i = 0; i < len; ++i) {
			// LCD_WriteData(data[i]);
		}
	}
}

void LCD_Test() {
	/*
	int i;
	LCD_WriteCmd(0xCB); //Power Control A
	LCD_WriteData({0x39, 0x2C, 0x00, 0x34, 0x02}, 5);

	LCD_WriteCmd(0xCF); //Power Control B
	LCD_WriteData(0x00);
	LCD_WriteData(0XC1); //Driver specification has 0x81
	LCD_WriteData(0X30);

	LCD_WriteCmd(0xE8); //Driver timing control A
	LCD_WriteData(0x85);
	LCD_WriteData(0x00);
	LCD_WriteData(0x78);

	LCD_WriteCmd(0xEA); //Driver timing control B
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xED); //Power on sequence control
	LCD_WriteData(0x64);
	LCD_WriteData(0x03);
	LCD_WriteData(0X12);
	LCD_WriteData(0X81);

	LCD_WriteCmd(0xF7); //Pump ratio control
	LCD_WriteData(0x20);

	LCD_WriteCmd(0xC0);    //Power control 1
	LCD_WriteData(0x23);   //VRH[5:0]

	LCD_WriteCmd(0xC1);    //Power control 2
	LCD_WriteData(0x10);   //SAP[2:0];BT[3:0]

	LCD_WriteCmd(0xC5);    //VCM control
	LCD_WriteData(0x3e); //�Աȶȵ���
	LCD_WriteData(0x28);

	LCD_WriteCmd(0xC7);    //VCM control2
	LCD_WriteData(0x86);  //--

	LCD_WriteCmd(0x36);    // Memory Access Control
	LCD_WriteData(0x48); //C8       //48 68����//28 E8 ����

	LCD_WriteCmd(0x3A); //Pixel format Set
	LCD_WriteData(0x55);

	LCD_WriteCmd(0xB1); //Frame Control
	LCD_WriteData(0x00);
	LCD_WriteData(0x18);

	LCD_WriteCmd(0xB6);    // Display Function Control
	LCD_WriteData(0x08);
	LCD_WriteData(0x82);
	LCD_WriteData(0x27);

	LCD_WriteCmd(0xF2);    // 3Gamma Function Disable
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x26);    //Gamma curve selected
	LCD_WriteData(0x01);

	LCD_WriteCmd(0xE0);    //Set Gamma
	LCD_WriteData(0x0F);
	LCD_WriteData(0x31);
	LCD_WriteData(0x2B);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x08);
	LCD_WriteData(0x4E);
	LCD_WriteData(0xF1);
	LCD_WriteData(0x37);
	LCD_WriteData(0x07);
	LCD_WriteData(0x10);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x09);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0XE1);    //Set Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x14);
	LCD_WriteData(0x03);
	LCD_WriteData(0x11);
	LCD_WriteData(0x07);
	LCD_WriteData(0x31);
	LCD_WriteData(0xC1);
	LCD_WriteData(0x48);
	LCD_WriteData(0x08);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x31);
	LCD_WriteData(0x36);
	LCD_WriteData(0x0F);

	LCD_WriteCmd(0x11);    //Exit Sleep
		for( i = 0; i < 20000; i++) { i++;}

	LCD_WriteCmd(0x29);    //Display on
	//LCD_WriteCmd(0x2c);
	*/
}
