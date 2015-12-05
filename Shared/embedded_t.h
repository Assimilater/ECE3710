#ifndef __EMBEDDED_T_H__
#define __EMBEDDED_T_H__
#pragma anon_unions

//---------------------------------------------------------------------------------------+
// Type defentions that should be used for better, more descriptive code                 |
//---------------------------------------------------------------------------------------+
typedef enum { false = 0, true = !false } bool;
typedef unsigned char byte;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef uint32 uint;
typedef byte uint8;

typedef union {
	struct {
		unsigned
			bit0:1,  bit1:1,  bit2:1,  bit3:1,  bit4:1,  bit5:1,  bit6:1,  bit7:1,
			bit8:1,  bit9:1,  bit10:1, bit11:1, bit12:1, bit13:1, bit14:1, bit15:1,
			bit16:1, bit17:1, bit18:1, bit19:1, bit20:1, bit21:1, bit22:1, bit23:1,
			bit24:1, bit25:1, bit26:1, bit27:1, bit28:1, bit29:1, bit30:1, bit31:1;
	};
	struct {
		unsigned
			nibble0:4, nibble1:4,
			nibble2:4, nibble3:4,
			nibble4:4, nibble5:4,
			nibble6:4, nibble7:4;
	};
	byte byte[4];
	uint16 half[2];
	uint word;
} WORD;

//access register via bit, byte, halfword, and word
typedef union {
	struct {
		volatile unsigned
			bit0:1,  bit1:1,  bit2:1,  bit3:1,  bit4:1,  bit5:1,  bit6:1,  bit7:1,
			bit8:1,  bit9:1,  bit10:1, bit11:1, bit12:1, bit13:1, bit14:1, bit15:1,
			bit16:1, bit17:1, bit18:1, bit19:1, bit20:1, bit21:1, bit22:1, bit23:1,
			bit24:1, bit25:1, bit26:1, bit27:1, bit28:1, bit29:1, bit30:1, bit31:1;
	};
	struct {
		volatile unsigned
			nibble0:4, nibble1:4,
			nibble2:4, nibble3:4,
			nibble4:4, nibble5:4,
			nibble6:4, nibble7:4;
	};
	volatile unsigned char byte[4];
	volatile unsigned short half[2];
	volatile unsigned int word;
} REG;

#endif
