#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "LCD.h"

#define TOUCH_MAX_COL (uint16)0xEDF;
#define TOUCH_MAX_ROW (uint16)0xFFF;

//---------------------------------------------------------------------------------------+
// Uses SPI to interact witht the touchscreen chip and retrieve coordinates              |
//---------------------------------------------------------------------------------------+
bool LCD_GetXY(SAMPLE_MODE mode, coord* val) {
	const uint SAMPLE_SIZE = 100;
	static coord data[SAMPLE_SIZE];
	
	// Vars used for poll
	byte miso[5], mosi[5] = {0xD0, 0, 0x90, 0, 0};
	SPI_Frame frame;
	coord* poll;
	
	// Vars used for get (averaging)
	static uint8 sample = 0;
	uint i, n, col, page;
	
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
		frame.route.CS = &TP_CSX;
		frame.route.SSI = SSI1;
		frame.bus.MOSI = mosi;
		frame.bus.MISO = miso;
		frame.bus.N = 5;
		SPI_Transfer(&frame);
		
		// Read = X C[11:0] X[2:0] (where X is don't care)
		poll = &data[sample++ % SAMPLE_SIZE];
		poll->col  = ((miso[1] << 8 | miso[2]) >> 3) & 0xFFF;
		poll->page = ((miso[3] << 8 | miso[4]) >> 3) & 0xFFF;
		
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
void LCD_WriteCmd(const byte cmd) {
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
void LCD_WriteData(const byte* data, const int len) {
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
void LCD_WriteBlock(const byte* data, const int len, const int n) {
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
	byte bStream[2];
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
	byte bStream[2];
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
// Writes text horizontally, with x-y referring the bottom left corner of the text       |
//---------------------------------------------------------------------------------------+
void LCD_WriteText(const TextRegion r) {
	byte temp;
	int row, letter, col, bitmask;
	text t = font_get(r.Font, r.Text);
	
	// We're writing horizontally (x is page-wise)
	LCD_SetColumn(r.y, r.y + r.Font->height - 1);
	LCD_SetPage(r.x, r.x + t.n * r.Font->width * 8);
	LCD_WaitChip(); // give the controller time to configure the page
	LCD_WriteCmd(0x2C); // See LCD_WriteBlock
	
	for (letter = 0; letter < t.n; ++letter) {
		for (col = 0; col < r.Font->width; ++col) {
			for (bitmask = 0x80; bitmask > 0; bitmask = bitmask >> 1) {
				for (row = r.Font->height - 1; row >= 0; --row) {
					// The font rows are inline in memory so this reads like assembly array access
					temp = t.s[letter][row * r.Font->width + col];
					if (temp & bitmask) {
						LCD_WriteData(r.Color, SIZE_COLOR);
					} else {
						LCD_WriteData(r.BackColor, SIZE_COLOR);
					}
				}
			}
		}
	}
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
