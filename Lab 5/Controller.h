#ifndef __TM4C123GH6PM_H__
#define __TM4C123GH6PM_H__

// PD: 0x42000000 + 32*0x73FC = 0x420E7F80
#define LCD_CS (*((volatile unsigned int *)0x420E7F88)) // PD2 = 0x420E7F88
#define DCX (*((volatile unsigned int *)0x420E7F8C)) // PD3 = 0x420E7F8C
#define WRX (*((volatile unsigned int *)0x420E7F98)) // PD6 = 0x420E7F98
#define RDX (*((volatile unsigned int *)0x420E7F9C)) // PD7 = 0x420E7F9C

#endif