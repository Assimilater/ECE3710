#include "LCD.h"

void LCD_WriteCmd(const unsigned char cmd) {
	LCD_CSX = 0; // CSX "LCD, pay attention!"
	LCD_DCX = 0; // DCX cmd
	LCD_WRX = 0; // WRX
	LCD_RDX = 1; // Set Read high
	GPIO.PortB->DATA.byte[0] = cmd;
	LCD_WRX = 1; // WRX read on +edge
}
void LCD_WriteData(const unsigned char* data, const int len) {
	int i;
	LCD_CSX = 0; // CSX "LCD, pay attention!"
	LCD_DCX = 1; // DCX Data
	LCD_RDX = 1; // Set Read high
	for (i = 0; i < len; ++i) {
		LCD_WRX = 0; // WRX
		GPIO.PortB->DATA.byte[0] = data[i];
		LCD_WRX = 1; // WRX read on +edge
	}

}
void LCD_WriteBlock(const unsigned char* data, const int len, const int n) {
	int i, j;
	LCD_WriteCmd(0x2C);
	LCD_CSX = 0; // CSX "LCD, pay attention!"
	LCD_DCX = 1; // DCX Data
	LCD_RDX = 1; // Set Read high
	for (j = 0; j < n; ++j) {
		for (i = 0; i < len; ++i) {
			LCD_WRX = 0; // WRX
			GPIO.PortB->DATA.byte[0] = data[i];
			LCD_WRX = 1; // WRX read on +edge
		}
	}
}

void LCD_SetColumn(const unsigned short Start, const unsigned short End) {
	LCD_WriteCmd(0x2A);
	LCD_WriteData((unsigned char*)&Start, 2);
	LCD_WriteData((unsigned char*)&End, 2);
}

void LCD_SetPage(const unsigned short Start, const unsigned short End) {
	LCD_WriteCmd(0x2B);
	LCD_WriteData((unsigned char*)&Start, 2);
	LCD_WriteData((unsigned char*)&End, 2);
}

void LCD_Test() {
	int i;
	
	LCD_WriteCmd(LCD_CODE_PWRA[0]); // Power Control A
	LCD_WriteData(LCD_CODE_PWRA + 1, SIZE_CODE_PWRA - 1);
	
	LCD_WriteCmd(LCD_CODE_PWRB[0]); // Power Control B
	LCD_WriteData(LCD_CODE_PWRB + 1, SIZE_CODE_PWRB - 1);
	
	LCD_WriteCmd(LCD_CODE_DTCA[0]); // Driver timing control A
	LCD_WriteData(LCD_CODE_DTCA + 1, SIZE_CODE_DTCA - 1);
	
	LCD_WriteCmd(LCD_CODE_DTCB[0]); // Driver timing control B
	LCD_WriteData(LCD_CODE_DTCB + 1, SIZE_CODE_DTCB - 1);
	
	LCD_WriteCmd(LCD_CODE_PSQC[0]); // Power on sequence control
	LCD_WriteData(LCD_CODE_PSQC + 1, SIZE_CODE_PSQC - 1);
	
	LCD_WriteCmd(LCD_CODE_PMRC[0]); // Pump ratio control
	LCD_WriteData(LCD_CODE_PMRC + 1, SIZE_CODE_PMRC - 1);
	
	LCD_WriteCmd(LCD_CODE_PCL1[0]); // Power control 1
	LCD_WriteData(LCD_CODE_PCL1 + 1, SIZE_CODE_PCL1 - 1);
	
	LCD_WriteCmd(LCD_CODE_PCL2[0]); // Power control 2
	LCD_WriteData(LCD_CODE_PCL2 + 1, SIZE_CODE_PCL2 - 1);
	
	LCD_WriteCmd(LCD_CODE_VCM1[0]); // VCM control
	LCD_WriteData(LCD_CODE_VCM1 + 1, SIZE_CODE_VCM1 - 1);
	
	LCD_WriteCmd(LCD_CODE_VCM2[0]); // VCM control2
	LCD_WriteData(LCD_CODE_VCM2 + 1, SIZE_CODE_VCM2 - 1);
	
	LCD_WriteCmd(LCD_CODE_MACL[0]); // Memory Access Control
	LCD_WriteData(LCD_CODE_MACL + 1, SIZE_CODE_MACL - 1);
	
	LCD_WriteCmd(LCD_CODE_PXFS[0]); // Pixel format Set
	LCD_WriteData(LCD_CODE_PXFS + 1, SIZE_CODE_PXFS - 1);
	
	LCD_WriteCmd(LCD_CODE_FMCL[0]); // Frame Control
	LCD_WriteData(LCD_CODE_FMCL + 1, SIZE_CODE_FMCL - 1);
	
	LCD_WriteCmd(LCD_CODE_DFCL[0]); // Display Function Control
	LCD_WriteData(LCD_CODE_DFCL + 1, SIZE_CODE_DFCL - 1);
	
	LCD_WriteCmd(LCD_CODE_3GFD[0]); // 3Gamma Function Disable
	LCD_WriteData(LCD_CODE_3GFD + 1, SIZE_CODE_3GFD - 1);
	
	LCD_WriteCmd(LCD_CODE_GCSL[0]); // Gamma curve selected
	LCD_WriteData(LCD_CODE_GCSL + 1, SIZE_CODE_GCSL - 1);
	
	LCD_WriteCmd(LCD_CODE_SGM0[0]); // Set Gamma 0
	LCD_WriteData(LCD_CODE_SGM0 + 1, SIZE_CODE_SGM0 - 1);
	
	LCD_WriteCmd(LCD_CODE_SGM1[0]); // Set Gamma 1
	LCD_WriteData(LCD_CODE_SGM1 + 1, SIZE_CODE_SGM1 - 1);
	
	LCD_WriteCmd(0x11); // Exit Sleep
	for(i = 0; i < 20000; i++) { i++; }
	LCD_WriteCmd(0x29); // Display on
}
