#ifndef __FONTS_H__
#define __FONTS_H__

typedef struct {
	unsigned char** root;
	unsigned char width, height;
} font;

typedef struct {
	font Ubuntu;
	font Terminus;
	font Small;
	font Big;
	font _8x8;
	font Basic_8x8;
} FontList;

const FontList* fonts(void);
const unsigned char* font_get(const font*, char);

#endif
