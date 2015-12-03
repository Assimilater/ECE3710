#include "../fonts.h"
#include "Ubuntu.h"
#include "TerminusFont.h"
#include "SmallFont.h"
#include "font8x8_basic.h"
#include "font8x8.h"
#include "BigFont.h"
#include <string.h> // strlen
//#include <stdlib.h> // malloc

//---------------------------------------------------------------------------------------+
// My implementation of the singleton for accessing fonts                                |
//---------------------------------------------------------------------------------------+
const FontList* fonts() {
	static FontList list;
	static char init = 0;
	if (!init) {
		// Ubuntu
		list.Ubuntu.root = (unsigned char**) Ubuntu;
		list.Ubuntu.width = 3; // (24 x 32) / 8 bit char
		list.Ubuntu.height = 32;
		
		// Terminus
		list.Terminus.root = (unsigned char**) TerminusFont;
		list.Terminus.width = 1; // (8 x 12) / 8 bit char
		list.Terminus.height = 12;
		
		// Small
		list.Small.root = (unsigned char**) SmallFont;
		list.Small.width = 1; // (8 x 12) / 8 bit char
		list.Small.height = 12;
		
		// Big
		list.Big.root = (unsigned char**) BigFont;
		list.Big.width = 2; // (16 x 16) / 8 bit char
		list.Big.height = 16;
		
		// 8x8
		list.Basic_8x8.root = (unsigned char**) font8x8;
		list.Basic_8x8.width = 1; // (8 x 8) / 8 bit char
		list.Basic_8x8.height = 1;
		
		// 8x8_basic
		list._8x8.root = (unsigned char**) font8x8_basic;
		list._8x8.width = 1; // (8 x 8) / 8 bit char
		list._8x8.height = 1;
		
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
	
	d.n = strlen(val);
  if (d.n > 100) { d.n = 100; } // Rather than use dynamic memory, max out the size to 100
	//d.s = malloc(sizeof(unsigned char*) * d.n);
	for (i = 0; i < d.n; ++i) {
		c = val[i];
		d.s[i] = (c < ' ' || c > '~')
			? font->root[0] // use space for unknown characters (ie extended ascii)
			: font->root[c - ' '];
	}
	return d;
}
