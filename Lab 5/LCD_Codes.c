#include "LCD_Codes.h"

//---------------------------------------------------------------------------------------+
// Command Codes For The LCD                                                             |
//---------------------------------------------------------------------------------------+
code LCD_CODE_PWRA[SIZE_CODE_PWRA] = {0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02};
code LCD_CODE_PWRB[SIZE_CODE_PWRB] = {0xCF, 0x00, 0xC1, 0x30}; //Driver spec. has 0x81 (not 0xC1)
code LCD_CODE_DTCA[SIZE_CODE_DTCA] = {0xE8, 0x85, 0x00, 0x78};
