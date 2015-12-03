#ifndef __FONTS_H__
#define __FONTS_H__

typedef struct {
	unsigned char** root;
	unsigned char width, height;
} font;

typedef struct {
	unsigned int n;
	unsigned char** s;
} text;
	
typedef struct {
	font Ubuntu;
	font Terminus;
	font Small;
	font Big;
	font _8x8;
	font Basic_8x8;
} FontList;

const FontList* fonts(void);
text font_get(const font*, const char*);

#endif
