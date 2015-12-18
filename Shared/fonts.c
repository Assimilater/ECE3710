#include "fonts.h"
#include "fonts/Ubuntu.h"
#include "fonts/TerminusFont.h"
#include "fonts/SmallFont.h"
#include "fonts/font8x8_basic.h"
#include "fonts/font8x8.h"
#include "fonts/BigFont.h"
#include <string.h> // strlen
//#include <stdlib.h> // malloc

//---------------------------------------------------------------------------------------+
// My implementation of the singleton for accessing fonts                                |
//---------------------------------------------------------------------------------------+
const FontList* fonts() {
	static FontList list;
	static byte init = 0;
	if (!init) {
		// Ubuntu
		list.Ubuntu.root = (byte*) Ubuntu;
		list.Ubuntu.width = 3; // (24 x 32) / 8 bit char
		list.Ubuntu.height = 32;
		
		// Terminus
		list.Terminus.root = (byte*) TerminusFont;
		list.Terminus.width = 1; // (8 x 12) / 8 bit char
		list.Terminus.height = 12;
		
		// Small
		list.Small.root = (byte*) SmallFont;
		list.Small.width = 1; // (8 x 12) / 8 bit char
		list.Small.height = 12;
		
		// Big
		list.Big.root = (byte*) BigFont;
		list.Big.width = 2; // (16 x 16) / 8 bit char
		list.Big.height = 16;
		
		// 8x8
		list.Basic_8x8.root = (byte*) font8x8_basic;
		list.Basic_8x8.width = 1; // (8 x 8) / 8 bit char
		list.Basic_8x8.height = 8;
		
		// 8x8_basic
		list._8x8.root = (byte*) font8x8;
		list._8x8.width = 1; // (8 x 8) / 8 bit char
		list._8x8.height = 8;
		
		init = 1;
	}
	
	return &list;
}

//---------------------------------------------------------------------------------------+
// Transform c-string into array of pointers to font characters                          |
//---------------------------------------------------------------------------------------+
text font_get(const font* font, const char* val) {
	int i;
	char c;
	text d;
	
	// Rather than use dynamic memory, max out the size to 25
	//d.s = malloc(sizeof(byte*) * d.n);
	d.n = strlen(val);
	if (d.n > 25) { d.n = 25; }
	for (i = 0; i < d.n; ++i) {
		c = val[i];
		c = (c < ' ' || c > '~')
			? 0 // use space for unknown characters (ie extended ascii)
			: c - ' ';
		
		d.s[i] = font->root + (c * font->width * font->height);
	}
	return d;
}
