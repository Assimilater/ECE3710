#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "LCD.h"

//---------------------------------------------------------------------------------------+
// Uses SPI to interact witht the touchscreen chip and retrieve coordinates              |
//---------------------------------------------------------------------------------------+
bool LCD_GetXY(SAMPLE_MODE mode, coord* val) {
	// Vars used for all modes
	static const unsigned short TOUCH_MAX_COL = 0xEDF;
	static const unsigned short TOUCH_MAX_ROW = 0xFFF;
	static const unsigned int SAMPLE_SIZE = 100;
	static coord data[SAMPLE_SIZE];
	
	// Vars used for poll
	unsigned short read0, read1, read2, read3, read4;
	coord* poll;
	
	// Vars used for get (averaging)
	static unsigned int sample = 0;
	unsigned int i, n, col, page;
	
	if (mode == TOUCH_RESET) {
		sample = 0; // Resets the sampling
	} else if (mode == TOUCH_GET) {
		//---------------------------------------------------------------------------------------+
		// Calculates the average from the sample and returns scaled coord based on LCD size     |
		//---------------------------------------------------------------------------------------+
		n = sample < SAMPLE_SIZE ? sample : SAMPLE_SIZE;
		sample = 0; // Reset the count now that we've grabbed a value
		if (!n) { return false; } // Report there is no data to average
		
		// Perform the average
		col = page = 0;
		for (i = 0; i < n; ++i) {
			col += data[i].col;
			page += data[i].page;
		}
		col /= n;
		page /= n;
		
		// Scale to range in pixels
		col = (col * LCD_COLS) / TOUCH_MAX_COL;
		page = (page * LCD_ROWS) / TOUCH_MAX_ROW;
		
		// Update the caller with the proper coordinate
		val->col = col;
		val->page = page;
	} else if (mode == TOUCH_POLL) {
		//---------------------------------------------------------------------------------------+
		// Uses SPI to interact with the touchscreen chip and retrieve coordinates               |
		//---------------------------------------------------------------------------------------+
		while (SSI0->SR & 0x4) { read0 = SSI0->DR; } // Clear the buffer, just in case
		poll = &data[sample++ % SAMPLE_SIZE]; // 
		TP_CSX = 0;
		
		while(!(SSI0->SR & 0x1)); // Wait for TFE = 1
		SSI0->DR = 0xD0; // X
		SSI0->DR = 0; // Give the TFT time to write both byes before issuing new command
		SSI0->DR = 0x90; // Y
		SSI0->DR = 0;
		SSI0->DR = 0; // Second byte of null so we can receive both bytes pertaining to y
		
		while(SSI0->SR & 0x10); // Wait for BSY == 0
		read0 = SSI0->DR; // Read null byte
		read1 = SSI0->DR; // Read first x-data byte
		read2 = SSI0->DR; // Read second x-data byte
		read3 = SSI0->DR; // Read first y-data byte
		read4 = SSI0->DR; // Read second y-data byte
		
		// Read = X C[11:0] X[2:0] (where X is don't care)
		poll->col  = ((read1 << 8 | read2) >> 3) & 0xFFF;
		poll->page = ((read3 << 8 | read4) >> 3) & 0xFFF;
		
		TP_CSX = 1;
	} else { return false; } // Unrecognized command
	return true; // Inidicate success
}

//---------------------------------------------------------------------------------------+
// A busy wait that should provide the LCD adequate time to respond to commands          |
//---------------------------------------------------------------------------------------+
void LCD_WaitChip() {
	const static int delay = 100000;
	int i = 0;
	for (; i < delay; ++i);
}

//---------------------------------------------------------------------------------------+
// Writes a command code to the LCD                                                      |
//---------------------------------------------------------------------------------------+
void LCD_WriteCmd(const unsigned char cmd) {
	LCD_CSX = 0; // CSX "LCD, pay attention!"
	LCD_DCX = 0; // DCX cmd
	LCD_WRX = 0; // WRX
	LCD_RDX = 1; // Set Read high
	GPIO.PortB->DATA.byte[0] = cmd;
	LCD_WRX = 1; // WRX read on +edge
	LCD_CSX = 1; // "We're done, LCD"
}

//---------------------------------------------------------------------------------------+
// Writes a data stream of arbitrary size to the LCD (high screen refresh speeds)        |
//---------------------------------------------------------------------------------------+
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

//---------------------------------------------------------------------------------------+
// Primarily used by FillRegion when the same data stream needs repeated sends           |
//---------------------------------------------------------------------------------------+
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

//---------------------------------------------------------------------------------------+
// Sets column bounds on the LCD                                                         |
//---------------------------------------------------------------------------------------+
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

//---------------------------------------------------------------------------------------+
// Sets page bounds on the LCD                                                           |
//---------------------------------------------------------------------------------------+
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

//---------------------------------------------------------------------------------------+
// Fills an area on the LCD to a solid color                                             |
//---------------------------------------------------------------------------------------+
void LCD_FillRegion(const Region r) {
	LCD_SetColumn(r.ColumnStart, r.ColumnEnd);
	LCD_SetPage(r.PageStart, r.PageEnd);
	LCD_WaitChip(); // give the controller time to configure the page
	
	LCD_WriteBlock(r.Color, SIZE_COLOR, (r.ColumnEnd - r.ColumnStart) * (r.PageEnd - r.PageStart));
}

//---------------------------------------------------------------------------------------+
// In this case it is best to just inline LCD_WriteData                                  |
//---------------------------------------------------------------------------------------+
void LCD_WriteText(const TextRegion r) {
  char temp;
	int row, letter, col, bitmask;
	text t = font_get(r.Font, r.Text);
	
	// We're writing horizontally (x is page-wise)
	LCD_SetColumn(r.y, r.y + r.Font->height * 8);
	LCD_SetPage(r.x, r.x + t.n * r.Font->width * 8);
	LCD_WaitChip(); // give the controller time to configure the page
	
	// Inline from LCD_WriteData
	LCD_WriteCmd(0x2C); // See LCD_WriteBlock
//	LCD_CSX = 0; // CSX "LCD, pay attention!"
//	LCD_DCX = 1; // DCX Data
//	LCD_RDX = 1; // Set Read high
	
	for (row = 0; row < r.Font->height; ++row) {
		for (letter = 0; letter < t.n; ++letter) {
			for (col = 0; col < r.Font->width; ++col) {
				// The font rows are inline in memory so this reads like assembly array access
				temp = t.s[letter][row * r.Font->width + col];
				for (bitmask = 1; bitmask < 0x100; bitmask = bitmask << 1) {
					if (temp & bitmask) {
						LCD_WriteData(r.Color, SIZE_COLOR);
						// Inline from LCD_WriteData
//						LCD_WRX = 0; // WRX
//						GPIO.PortB->DATA.byte[0] = r.Color[0];
//						LCD_WRX = 1; // WRX read on +edge
//						
//						// Making up for the absence of loop structures
//						__nop(); __nop(); __nop();  __nop();
//						
//						// Inline from LCD_WriteData
//						LCD_WRX = 0; // WRX
//						GPIO.PortB->DATA.byte[0] = r.Color[1];
//						LCD_WRX = 1; // WRX read on +edge
					} else {
						LCD_WriteData(r.BackColor, SIZE_COLOR);
						// Inline from LCD_WriteData
//						LCD_WRX = 0; // WRX
//						GPIO.PortB->DATA.byte[0] = r.BackColor[0];
//						LCD_WRX = 1; // WRX read on +edge
//						
//						// Making up for the absence of loop structures
//						__nop(); __nop(); __nop();  __nop();
//						
//						// Inline from LCD_WriteData
//						LCD_WRX = 0; // WRX
//						GPIO.PortB->DATA.byte[0] = r.BackColor[1];
//						LCD_WRX = 1; // WRX read on +edge
					}
				}
			}
		}
	}
	
	// Inline from LCD_WriteData
//	LCD_CSX = 1; // "We're done, LCD"
}

//---------------------------------------------------------------------------------------+
// Initialization codes provided by lecture notes, then start the screen all black       |
//---------------------------------------------------------------------------------------+
void LCD_Init() {
	Region r = {
		0, LCD_COLS,
		0, LCD_ROWS,
		LCD_COLOR_BLACK
	};
	
	LCD_RST = 0;
	LCD_WaitChip(); // Give the LCD time to reset
	LCD_RST = 1;
  LCD_WaitChip(); // Give the LCD time to reset
	
	TP_CSX = 1;
	
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
