#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "LCD.h"

void LCD_GetXY(coord* val) {
	// CB Format => S A[2:0] Mode 000
	unsigned short read;
	while (SSI0->SR & 0x4) { read = SSI0->DR; } // Clear the buffer, just in case
	TP_CSX = 0;
	
	while(!(SSI0->SR & 0x1)); // Wait for TFE = 1
	SSI0->DR = 0xD0; // X
	SSI0->DR = 0; // Give the TFT time to write both byes before issuing new command
	//SSI0->DR = 0;
	SSI0->DR = 0x90; // Y
	SSI0->DR = 0;
	SSI0->DR = 0; // Second byte of null so we can receive both bytes pertaining to y
	
	while(SSI0->SR & 0x10); // Wait for BSY == 0
	read = SSI0->DR; // Read null byte
	read = SSI0->DR; // Read first x-data byte
	read = read << 8;
	read |= SSI0->DR; // Read second x-data byte
	
	// Read = X C[11:0] X[2:0] (where X is don't care)
	read = read >> 3;
	read &= 0xFFF;
	val->col = read;
	
	read = SSI0->DR; // Read first y-data byte
	read = read << 8;
	read |= SSI0->DR; // Read second y-data byte
	
	// Read = X C[11:0] X[2:0] (where X is don't care)
	read = read >> 3;
	read &= 0xFFF;
	val->page = read;
	
	TP_CSX = 1;
}

void LCD_WaitChip() {
	const static int delay = 100000;
	int i = 0;
	for (; i < delay; ++i);
}

void LCD_WriteCmd(const unsigned char cmd) {
	LCD_CSX = 0; // CSX "LCD, pay attention!"
	LCD_DCX = 0; // DCX cmd
	LCD_WRX = 0; // WRX
	LCD_RDX = 1; // Set Read high
	GPIO.PortB->DATA.byte[0] = cmd;
	LCD_WRX = 1; // WRX read on +edge
	LCD_CSX = 1; // "We're done, LCD"
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
	LCD_CSX = 1; // "We're done, LCD"
}
void LCD_WriteBlock(const unsigned char* data, const int len, const int n) {
	int i, j;
	LCD_WriteCmd(0x2C);
	LCD_CSX = 0; // CSX "LCD, pay attention!"
	LCD_DCX = 1; // DCX Data
	LCD_RDX = 1; // Set Read high
	for (j = 1; j < n; ++j) {
		for (i = 0; i < len; ++i) {
			LCD_WRX = 0; // WRX
			GPIO.PortB->DATA.byte[0] = data[i];
			LCD_WRX = 1; // WRX read on +edge
		}
	}
	LCD_CSX = 1; // "We're done, LCD"
}

void LCD_SetColumn(const unsigned short Start, const unsigned short End) {
	unsigned char bStream[2];
	LCD_WriteCmd(0x2A);
	
	bStream[0] = (Start & 0xFF00) >> 8;
	bStream[1] = (Start & 0x00FF);
	LCD_WriteData(bStream, 2);
	
	bStream[0] = (End & 0xFF00) >> 8;
	bStream[1] = (End & 0x00FF);
	LCD_WriteData(bStream, 2);
}

void LCD_SetPage(const unsigned short Start, const unsigned short End) {
	unsigned char bStream[2];
	LCD_WriteCmd(0x2B);
	
	bStream[0] = (Start & 0xFF00) >> 8;
	bStream[1] = (Start & 0x00FF);
	LCD_WriteData(bStream, 2);
	
	bStream[0] = (End & 0xFF00) >> 8;
	bStream[1] = (End & 0x00FF);
	LCD_WriteData(bStream, 2);
}
void LCD_FillRegion(const Region r) {
	LCD_SetColumn(r.ColumnStart, r.ColumnEnd);
	LCD_SetPage(r.PageStart, r.PageEnd);
	
	LCD_WaitChip(); // give the controller time to configure the page
	LCD_WriteBlock(r.Color, SIZE_COLOR, (r.ColumnEnd - r.ColumnStart) * (r.PageEnd - r.PageStart));
}

void LCD_Init() {
	Region r = {
		0, LCD_COLS,
		0, LCD_ROWS,
		LCD_COLOR_BLACK
	};
	
	TP_CSX = 1;
	
	LCD_RST = 0;
	LCD_RST = 1;
	
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
	LCD_WaitChip();
	LCD_WriteCmd(0x29); // Display on
	
	LCD_FillRegion(r);
}
