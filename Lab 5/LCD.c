#include "LCD.h"
//C:\Users\John\Source\School\ECE 3710\Lab 5

void LCD_Init()
{
	int i;
    writeCmd(0xCB); //Power Control A
    writeDat(0x39);
    writeDat(0x2C);
    writeDat(0x00);
    writeDat(0x34);
    writeDat(0x02);

    writeCmd(0xCF); //Power Control B
    writeDat(0x00);
    writeDat(0XC1); //**Driver specification has 0x81
    writeDat(0X30);

    writeCmd(0xE8); //Driver timing control A
    writeDat(0x85);
    writeDat(0x00);
    writeDat(0x78);

    writeCmd(0xEA); //Driver timing control B
    writeDat(0x00);
    writeDat(0x00);

    writeCmd(0xED); //Power on sequence control
    writeDat(0x64);
    writeDat(0x03);
    writeDat(0X12);
    writeDat(0X81);

    writeCmd(0xF7); //Pump ratio control
    writeDat(0x20);

    writeCmd(0xC0);    //Power control 1
    writeDat(0x23);   //VRH[5:0]

    writeCmd(0xC1);    //Power control 2
    writeDat(0x10);   //SAP[2:0];BT[3:0]

    writeCmd(0xC5);    //VCM control
    writeDat(0x3e); //对比度调节
    writeDat(0x28);

    writeCmd(0xC7);    //VCM control2
    writeDat(0x86);  //--

    writeCmd(0x36);    // Memory Access Control
    writeDat(0x48); //C8       //48 68竖屏//28 E8 横屏

    writeCmd(0x3A); //Pixel format Set
    writeDat(0x55);

    writeCmd(0xB1); //Frame Control
    writeDat(0x00);
    writeDat(0x18);

    writeCmd(0xB6);    // Display Function Control
    writeDat(0x08);
    writeDat(0x82);
    writeDat(0x27);

    writeCmd(0xF2);    // 3Gamma Function Disable
    writeDat(0x00);

    writeCmd(0x26);    //Gamma curve selected
    writeDat(0x01);

    writeCmd(0xE0);    //Set Gamma
    writeDat(0x0F);
    writeDat(0x31);
    writeDat(0x2B);
    writeDat(0x0C);
    writeDat(0x0E);
    writeDat(0x08);
    writeDat(0x4E);
    writeDat(0xF1);
    writeDat(0x37);
    writeDat(0x07);
    writeDat(0x10);
    writeDat(0x03);
    writeDat(0x0E);
    writeDat(0x09);
    writeDat(0x00);

    writeCmd(0XE1);    //Set Gamma
    writeDat(0x00);
    writeDat(0x0E);
    writeDat(0x14);
    writeDat(0x03);
    writeDat(0x11);
    writeDat(0x07);
    writeDat(0x31);
    writeDat(0xC1);
    writeDat(0x48);
    writeDat(0x08);
    writeDat(0x0F);
    writeDat(0x0C);
    writeDat(0x31);
    writeDat(0x36);
    writeDat(0x0F);

    writeCmd(0x11);    //Exit Sleep
		for( i = 0; i < 20000; i++) { i++;}

    writeCmd(0x29);    //Display on
    //writeCmd(0x2c);
}