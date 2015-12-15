#ifndef __FONTS_H__
#define __FONTS_H__

#include "embedded_t.h"
typedef struct {
	byte* root;
	byte width, height;
} font;

typedef struct {
	uint n;
	byte* s[25];
} text;
	
typedef struct {
	font Ubuntu;
//	font Terminus;
//	font Small;
	font Big;
//	font _8x8;
//	font Basic_8x8;
} FontList;

const FontList* fonts(void);
text font_get(const font*, const char*);

#endif
