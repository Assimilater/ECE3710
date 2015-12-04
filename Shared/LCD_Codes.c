#include "LCD_Codes.h"
#include "embedded_t.h"

//---------------------------------------------------------------------------------------+
// Command Codes For The LCD                                                             |
//---------------------------------------------------------------------------------------+
const byte LCD_CODE_PWRA[SIZE_CODE_PWRA] = {0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02};
const byte LCD_CODE_PWRB[SIZE_CODE_PWRB] = {0xCF, 0x00, 0xC1, 0x30}; //Driver spec. has 0x81 (not 0xC1)
const byte LCD_CODE_DTCA[SIZE_CODE_DTCA] = {0xE8, 0x85, 0x00, 0x78};
const byte LCD_CODE_DTCB[SIZE_CODE_DTCB] = {0xEA, 0x00, 0x00};
const byte LCD_CODE_PSQC[SIZE_CODE_PSQC] = {0xED, 0x64, 0x03, 0x12, 0x81};
const byte LCD_CODE_PMRC[SIZE_CODE_PMRC] = {0xF7, 0x20};
const byte LCD_CODE_PCL1[SIZE_CODE_PCL1] = {0xC0, 0x23}; //VRH[5:0]
const byte LCD_CODE_PCL2[SIZE_CODE_PCL2] = {0xC1, 0x10}; //SAP[2:0]; BT[3:0]
const byte LCD_CODE_VCM1[SIZE_CODE_VCM1] = {0xC5, 0x3e, 0x28};
const byte LCD_CODE_VCM2[SIZE_CODE_VCM2] = {0xC7, 0x86};
const byte LCD_CODE_MACL[SIZE_CODE_MACL] = {0x36, 0x48};
const byte LCD_CODE_PXFS[SIZE_CODE_PXFS] = {0x3A, 0x55};
const byte LCD_CODE_FMCL[SIZE_CODE_FMCL] = {0xB1, 0x00, 0x18};
const byte LCD_CODE_DFCL[SIZE_CODE_DFCL] = {0xB6, 0x08, 0x82, 0x27};
const byte LCD_CODE_3GFD[SIZE_CODE_3GFD] = {0xF2, 0x00};
const byte LCD_CODE_GCSL[SIZE_CODE_GCSL] = {0x26, 0x01};

const byte LCD_CODE_SGM0[SIZE_CODE_SGM0] = {0xE0, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
const byte LCD_CODE_SGM1[SIZE_CODE_SGM1] = {0xE1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};

//---------------------------------------------------------------------------------------+
// Color Codes For The LCD                                                               |
//---------------------------------------------------------------------------------------+
const byte LCD_COLOR_WHITE		[SIZE_COLOR] = {0xFF, 0xFF};
const byte LCD_COLOR_BLACK		[SIZE_COLOR] = {0x00, 0x01};
const byte LCD_COLOR_GREY		[SIZE_COLOR] = {0xF7, 0xDE};
const byte LCD_COLOR_BLUE		[SIZE_COLOR] = {0x00, 0x1F};
const byte LCD_COLOR_RED		[SIZE_COLOR] = {0xF8, 0x00};
const byte LCD_COLOR_MAGENTA	[SIZE_COLOR] = {0xF8, 0x1F};
const byte LCD_COLOR_GREEN		[SIZE_COLOR] = {0x07, 0xE0};
const byte LCD_COLOR_CYAN		[SIZE_COLOR] = {0x7F, 0xFF};
const byte LCD_COLOR_YELLOW		[SIZE_COLOR] = {0xFF, 0xE0};
