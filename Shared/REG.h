#ifndef _REG_H_
#define _REG_H_
#pragma anon_unions

//access register via bit, byte, halfword, and word
typedef union {
	struct {
		volatile unsigned
			bit0:1,  bit1:1,  bit2:1,  bit3:1,  bit4:1,  bit5:1,  bit6:1,  bit7:1,
			bit8:1,  bit9:1,  bit10:1, bit11:1, bit12:1, bit13:1, bit14:1, bit15:1,
			bit16:1, bit17:1, bit18:1, bit19:1, bit20:1, bit21:1, bit22:1, bit23:1,
			bit24:1, bit25:1, bit26:1, bit27:1, bit28:1, bit29:1, bit30:1, bit31:1;
	};
	volatile unsigned char byte[4];
	volatile unsigned short half[2];
	volatile unsigned int word;
} REG;

#endif