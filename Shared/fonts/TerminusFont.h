unsigned char TerminusFont[][12] = {
  // #32
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // ' '
  {0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x20,0x00,0x00},  // '!'
  {0x00,0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '"'
  {0x00,0x00,0x50,0x50,0xF8,0x50,0x50,0xF8,0x50,0x50,0x00,0x00},  // '#'
  {0x00,0x00,0x20,0x70,0xA8,0xA0,0x70,0x28,0xA8,0x70,0x20,0x00},  // '$'
  {0x00,0x00,0x48,0xA8,0x50,0x10,0x20,0x28,0x54,0x48,0x00,0x00},  // '%'
  {0x00,0x00,0x20,0x50,0x50,0x20,0x68,0x90,0x90,0x68,0x00,0x00},  // '&'
  {0x00,0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '''
  {0x00,0x00,0x10,0x20,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00},  // '('
  {0x00,0x00,0x40,0x20,0x10,0x10,0x10,0x10,0x20,0x40,0x00,0x00},  // ')'
  {0x00,0x00,0x00,0x00,0x50,0x20,0xF8,0x20,0x50,0x00,0x00,0x00},  // '*'
  {0x00,0x00,0x00,0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00,0x00},  // '+'
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x40,0x00},  // ','
  {0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00},  // '-'
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x00,0x00},  // '.'
  {0x00,0x00,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,0x00,0x00},  // '/'
  {0x00,0x00,0x70,0x88,0x98,0xA8,0xC8,0x88,0x88,0x70,0x00,0x00},  // '0'
  {0x00,0x00,0x20,0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // '1'
  {0x00,0x00,0x70,0x88,0x88,0x10,0x20,0x40,0x80,0xF8,0x00,0x00},  // '2'
  {0x00,0x00,0x70,0x88,0x08,0x30,0x08,0x08,0x88,0x70,0x00,0x00},  // '3'
  {0x00,0x00,0x08,0x18,0x28,0x48,0x88,0xF8,0x08,0x08,0x00,0x00},  // '4'
  {0x00,0x00,0xF8,0x80,0x80,0xF0,0x08,0x08,0x88,0x70,0x00,0x00},  // '5'
  {0x00,0x00,0x70,0x80,0x80,0xF0,0x88,0x88,0x88,0x70,0x00,0x00},  // '6'
  {0x00,0x00,0xF8,0x08,0x08,0x10,0x10,0x20,0x20,0x20,0x00,0x00},  // '7'
  {0x00,0x00,0x70,0x88,0x88,0x70,0x88,0x88,0x88,0x70,0x00,0x00},  // '8'
  {0x00,0x00,0x70,0x88,0x88,0x88,0x78,0x08,0x08,0x70,0x00,0x00},  // '9'
  {0x00,0x00,0x00,0x00,0x20,0x20,0x00,0x00,0x20,0x20,0x00,0x00},  // ':'
  {0x00,0x00,0x00,0x00,0x20,0x20,0x00,0x00,0x20,0x20,0x40,0x00},  // ';'
  {0x00,0x00,0x00,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00,0x00},  // '<'
  {0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00,0x00,0x00,0x00},  // '='
  {0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00,0x00},  // '>'
  {0x00,0x00,0x70,0x88,0x88,0x10,0x20,0x00,0x20,0x20,0x00,0x00},  // '?'
  {0x00,0x00,0x70,0x88,0x98,0xA8,0xA8,0x98,0x80,0x78,0x00,0x00},  // '@'
  {0x00,0x00,0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0x00},  // 'A'
  {0x00,0x00,0xF0,0x88,0x88,0xF0,0x88,0x88,0x88,0xF0,0x00,0x00},  // 'B'
  {0x00,0x00,0x70,0x88,0x80,0x80,0x80,0x80,0x88,0x70,0x00,0x00},  // 'C' '\x43'
  {0x00,0x00,0xE0,0x90,0x88,0x88,0x88,0x88,0x90,0xE0,0x00,0x00},  // 'D'
  {0x00,0x00,0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0xF8,0x00,0x00},  // 'E'
  {0x00,0x00,0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x80,0x00,0x00},  // 'F' '\x46'
  {0x00,0x00,0x70,0x88,0x80,0x80,0xB8,0x88,0x88,0x70,0x00,0x00},  // 'G'
  {0x00,0x00,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x88,0x00,0x00},  // 'H'
  {0x00,0x00,0x70,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'I'
  {0x00,0x00,0x38,0x10,0x10,0x10,0x10,0x90,0x90,0x60,0x00,0x00},  // 'J'
  {0x00,0x00,0x88,0x90,0xA0,0xC0,0xC0,0xA0,0x90,0x88,0x00,0x00},  // 'K'
  {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00,0x00},  // 'L'
  {0x00,0x00,0x88,0xD8,0xA8,0xA8,0x88,0x88,0x88,0x88,0x00,0x00},  // 'M'
  {0x00,0x00,0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x88,0x00,0x00},  // 'N'
  {0x00,0x00,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'O'
  {0x00,0x00,0xF0,0x88,0x88,0x88,0xF0,0x80,0x80,0x80,0x00,0x00},  // 'P'
  {0x00,0x00,0x70,0x88,0x88,0x88,0x88,0x88,0xA8,0x70,0x08,0x00},  // 'Q'
  {0x00,0x00,0xF0,0x88,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00,0x00},  // 'R'
  {0x00,0x00,0x70,0x88,0x80,0x70,0x08,0x08,0x88,0x70,0x00,0x00},  // 'S'
  {0x00,0x00,0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00},  // 'T'
  {0x00,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'U'
  {0x00,0x00,0x88,0x88,0x88,0x50,0x50,0x50,0x20,0x20,0x00,0x00},  // 'V'
  {0x00,0x00,0x88,0x88,0x88,0x88,0xA8,0xA8,0xD8,0x88,0x00,0x00},  // 'W'
  {0x00,0x00,0x88,0x88,0x50,0x20,0x20,0x50,0x88,0x88,0x00,0x00},  // 'X'
  {0x00,0x00,0x88,0x88,0x50,0x50,0x20,0x20,0x20,0x20,0x00,0x00},  // 'Y'
  {0x00,0x00,0xF8,0x08,0x10,0x20,0x40,0x80,0x80,0xF8,0x00,0x00},  // 'Z'
  {0x00,0x00,0x70,0x40,0x40,0x40,0x40,0x40,0x40,0x70,0x00,0x00},  // '['
  {0x00,0x00,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x00,0x00},  // '\'
  {0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x70,0x00,0x00},  // ']'
  {0x00,0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '^'
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00},  // '_'
  {0x40,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '`'
  {0x00,0x00,0x00,0x00,0x70,0x08,0x78,0x88,0x88,0x78,0x00,0x00},  // 'a'
  {0x00,0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x88,0xF0,0x00,0x00},  // 'b'
  {0x00,0x00,0x00,0x00,0x70,0x88,0x80,0x80,0x88,0x70,0x00,0x00},  // 'c'
  {0x00,0x00,0x08,0x08,0x78,0x88,0x88,0x88,0x88,0x78,0x00,0x00},  // 'd'
  {0x00,0x00,0x00,0x00,0x70,0x88,0xF8,0x80,0x88,0x70,0x00,0x00},  // 'e'
  {0x00,0x00,0x18,0x20,0x70,0x20,0x20,0x20,0x20,0x20,0x00,0x00},  // 'f'
  {0x00,0x00,0x00,0x00,0x78,0x88,0x88,0x88,0x88,0x78,0x08,0x70},  // 'g'
  {0x00,0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x88,0x88,0x00,0x00},  // 'h'
  {0x00,0x20,0x20,0x00,0x60,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'i'
  {0x00,0x08,0x08,0x00,0x18,0x08,0x08,0x08,0x08,0x08,0x48,0x30},  // 'j'
  {0x00,0x00,0x40,0x40,0x48,0x50,0x60,0x60,0x50,0x48,0x00,0x00},  // 'k'
  {0x00,0x00,0x60,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'l'
  {0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0xA8,0x00,0x00},  // 'm'
  {0x00,0x00,0x00,0x00,0xF0,0x88,0x88,0x88,0x88,0x88,0x00,0x00},  // 'n'
  {0x00,0x00,0x00,0x00,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'o'
  {0x00,0x00,0x00,0x00,0xF0,0x88,0x88,0x88,0x88,0xF0,0x80,0x80},  // 'p'
  {0x00,0x00,0x00,0x00,0x78,0x88,0x88,0x88,0x88,0x78,0x08,0x08},  // 'q'
  {0x00,0x00,0x00,0x00,0xB8,0xC0,0x80,0x80,0x80,0x80,0x00,0x00},  // 'r'
  {0x00,0x00,0x00,0x00,0x78,0x80,0x70,0x08,0x08,0xF0,0x00,0x00},  // 's'
  {0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x20,0x18,0x00,0x00},  // 't'
  {0x00,0x00,0x00,0x00,0x88,0x88,0x88,0x88,0x88,0x78,0x00,0x00},  // 'u'
  {0x00,0x00,0x00,0x00,0x88,0x88,0x50,0x50,0x20,0x20,0x00,0x00},  // 'v'
  {0x00,0x00,0x00,0x00,0x88,0x88,0xA8,0xA8,0xA8,0x70,0x00,0x00},  // 'w'
  {0x00,0x00,0x00,0x00,0x88,0x50,0x20,0x20,0x50,0x88,0x00,0x00},  // 'x'
  {0x00,0x00,0x00,0x00,0x88,0x88,0x88,0x88,0x88,0x78,0x08,0x70},  // 'y'
  {0x00,0x00,0x00,0x00,0xF8,0x10,0x20,0x40,0x80,0xF8,0x00,0x00},  // 'z'
  {0x00,0x00,0x18,0x20,0x20,0x40,0x20,0x20,0x20,0x18,0x00,0x00},  // '{'
  {0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00},  // '|'
  {0x00,0x00,0x60,0x10,0x10,0x08,0x10,0x10,0x10,0x60,0x00,0x00},  // '}'
  // #126
  {0x00,0x48,0xA8,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '~'
  // #128
  {0x00,0x0C,0x16,0x20,0x60,0xFC,0x60,0xF8,0x10,0x0C,0x00,0x00},  // '€' '\x80'
  // #139
  {0x00,0x00,0x00,0x04,0x08,0x10,0x20,0x10,0x08,0x04,0x00,0x00},  // '‹' '\x8B'
  // #155
  {0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x08,0x10,0x20,0x00,0x00},  // '›' '\x9B'
  // #160
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '' (&nbsp;) '\xA0'
  {0x00,0x00,0x20,0x20,0x00,0x20,0x20,0x20,0x20,0x20,0x00,0x00},  // '¡' '\xA1'
  {0x00,0x00,0x00,0x20,0x70,0xA8,0xA0,0xA0,0xA8,0x70,0x20,0x00},  // '¢' '\xA2'
  {0x00,0x00,0x30,0x48,0x40,0xF0,0x40,0x40,0x48,0xF8,0x00,0x00},  // '£' '\xA3'
  {0x00,0x00,0x00,0x48,0x30,0x48,0x48,0x30,0x48,0x00,0x00,0x00},  // '¤' '\xA4'
  {0x00,0x00,0x88,0x88,0x50,0x20,0x70,0x20,0x70,0x20,0x00,0x00},  // '¥' '\xA5'
  {0x00,0x00,0x20,0x20,0x20,0x00,0x00,0x20,0x20,0x20,0x00,0x00},  // '¦' '\xA6'
  {0x00,0x30,0x48,0x20,0x50,0x48,0x28,0x10,0x48,0x30,0x00,0x00},  // '§' '\xA7'
  {0x50,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '¨' '\xA8'
  {0x00,0x00,0x78,0x84,0xB4,0xA4,0xA4,0xB4,0x84,0x78,0x00,0x00},  // '©' '\xA9'
  {0x30,0x08,0x38,0x48,0x38,0x00,0x78,0x00,0x00,0x00,0x00,0x00},  // 'ª' '\xAA'
  {0x00,0x00,0x00,0x14,0x28,0x50,0xA0,0x50,0x28,0x14,0x00,0x00},  // '«' '\xAB'
  {0x00,0x00,0x00,0x00,0xF8,0x08,0x08,0x08,0x00,0x00,0x00,0x00},  // '¬' '\xAC'
  {0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00},  // '­ ­­­­' '\xAD'
  {0x00,0x00,0x78,0x84,0xB4,0xAC,0xB4,0xAC,0x84,0x78,0x00,0x00},  // '®' '\xAE'
  {0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '¯' '\xAF'
  {0x00,0x20,0x50,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '°' '\xB0'
  {0x00,0x00,0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0xF8,0x00,0x00},  // '±' '\xB1'
  {0x00,0x30,0x48,0x10,0x20,0x78,0x00,0x00,0x00,0x00,0x00,0x00},  // '²' '\xB2'
  {0x00,0x70,0x08,0x30,0x08,0x70,0x00,0x00,0x00,0x00,0x00,0x00},  // '³' '\xB3'
  {0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '´' '\xB4'
  {0x00,0x00,0x00,0x00,0x88,0x88,0x88,0x88,0x98,0xE8,0x80,0x80},  // 'µ' '\xB5'
  {0x00,0x00,0x78,0xA8,0xA8,0xA8,0x68,0x28,0x28,0x28,0x00,0x00},  // '¶' '\xB6'
  {0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x00,0x00,0x00,0x00,0x00},  // '·' '\xB7'
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x40},  // '¸' '\xB8'
  {0x00,0x20,0x60,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // '¹' '\xB9'
  {0x30,0x48,0x48,0x48,0x30,0x00,0x78,0x00,0x00,0x00,0x00,0x00},  // 'º' '\xBA'
  {0x00,0x00,0x00,0xA0,0x50,0x28,0x14,0x28,0x50,0xA0,0x00,0x00},  // '»' '\xBB'
  {0x40,0xC0,0x40,0x48,0x10,0x20,0x48,0x98,0x28,0x78,0x08,0x08},  // '¼' '\xBC'
  {0x40,0xC0,0x44,0x48,0x10,0x20,0x40,0x98,0x24,0x08,0x10,0x3C},  // '½' '\xBD'
  {0xC0,0x20,0xC0,0x24,0xC8,0x10,0x24,0x4C,0x94,0x3C,0x04,0x04},  // '¾' '\xBE'
  {0x00,0x00,0x20,0x20,0x00,0x20,0x40,0x88,0x88,0x70,0x00,0x00},  // '¿' '\xBF'
  {0x40,0x20,0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0x00},  // 'À' '\xC0'
  {0x10,0x20,0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0x00},  // 'Á' '\xC1'
  {0x20,0x50,0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0x00},  // 'Â' '\xC2'
  {0x28,0x50,0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0x00},  // 'Ã' '\xC3'
  {0x50,0x50,0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0x00},  // 'Ä' '\xC4'
  {0x20,0x50,0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0x00},  // 'Å' '\xC5'
  {0x00,0x00,0x7C,0x90,0x90,0xFC,0x90,0x90,0x90,0x9C,0x00,0x00},  // 'Æ' '\xC6'
  {0x00,0x00,0x70,0x88,0x80,0x80,0x80,0x80,0x88,0x70,0x20,0x40},  // 'Ç' '\xC7'
  {0x40,0x20,0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0xF8,0x00,0x00},  // 'È' '\xC8'
  {0x10,0x20,0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0xF8,0x00,0x00},  // 'É' '\xC9'
  {0x20,0x50,0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0xF8,0x00,0x00},  // 'Ê' '\xCA'
  {0x50,0x50,0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0xF8,0x00,0x00},  // 'Ë' '\xCB'
  {0x40,0x20,0x70,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'Ì' '\xCC'
  {0x10,0x20,0x70,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'Í' '\xCD'
  {0x20,0x50,0x70,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'Î' '\xCE'
  {0x50,0x50,0x70,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'Ï' '\xCF'
  {0x00,0x00,0xE0,0x90,0x88,0xE8,0x88,0x88,0x90,0xE0,0x00,0x00},  // 'Ð' '\xD0'
  {0x28,0x50,0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x88,0x00,0x00},  // 'Ñ' '\xD1'
  {0x40,0x20,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Ò' '\xD2'
  {0x10,0x20,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Ó' '\xD3'
  {0x20,0x50,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Ô' '\xD4'
  {0x28,0x50,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Õ' '\xD5'
  {0x50,0x50,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Ö' '\xD6'
  {0x00,0x00,0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00,0x00,0x00},  // '×' '\xD7'
  {0x00,0x00,0x74,0x88,0x98,0xA8,0xC8,0x88,0x88,0x70,0x00,0x00},  // 'Ø' '\xD8'
  {0x40,0x20,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Ù' '\xD9'
  {0x10,0x20,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Ú' '\xDA'
  {0x20,0x50,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Û' '\xDB'
  {0x50,0x50,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'Ü' '\xDC'
  {0x10,0x20,0x88,0x88,0x50,0x50,0x20,0x20,0x20,0x20,0x00,0x00},  // 'Ý' '\xDD'
  {0x00,0x00,0x80,0xF0,0x88,0x88,0x88,0xF0,0x80,0x80,0x00,0x00},  // 'Þ' '\xDE'
  {0x00,0x00,0xE0,0x90,0x90,0xF0,0x88,0x88,0xC8,0xB0,0x00,0x00},  // 'ß' '\xDF'
  {0x00,0x00,0x40,0x20,0x70,0x08,0x78,0x88,0x88,0x78,0x00,0x00},  // 'à' '\xE0'
  {0x00,0x00,0x10,0x20,0x70,0x08,0x78,0x88,0x88,0x78,0x00,0x00},  // 'á' '\xE1'
  {0x00,0x00,0x20,0x50,0x70,0x08,0x78,0x88,0x88,0x78,0x00,0x00},  // 'â' '\xE2'
  {0x00,0x00,0x28,0x50,0x70,0x08,0x78,0x88,0x88,0x78,0x00,0x00},  // 'ã' '\xE3'
  {0x00,0x00,0x50,0x50,0x70,0x08,0x78,0x88,0x88,0x78,0x00,0x00},  // 'ä' '\xE4'
  {0x00,0x00,0x20,0x50,0x70,0x08,0x78,0x88,0x88,0x78,0x00,0x00},  // 'å' '\xE5'
  {0x00,0x00,0x00,0x00,0x70,0x28,0x68,0xB0,0xA0,0x78,0x00,0x00},  // 'æ' '\xE6'
  {0x00,0x00,0x00,0x00,0x70,0x88,0x80,0x80,0x88,0x70,0x20,0x40},  // 'ç' '\xE7'
  {0x00,0x00,0x40,0x20,0x70,0x88,0xF8,0x80,0x88,0x70,0x00,0x00},  // 'è' '\xE8'
  {0x00,0x00,0x10,0x20,0x70,0x88,0xF8,0x80,0x88,0x70,0x00,0x00},  // 'é' '\xE9'
  {0x00,0x00,0x20,0x50,0x70,0x88,0xF8,0x80,0x88,0x70,0x00,0x00},  // 'ê' '\xEA'
  {0x00,0x00,0x50,0x50,0x70,0x88,0xF8,0x80,0x88,0x70,0x00,0x00},  // 'ë' '\xEB'
  {0x00,0x00,0x40,0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'ì' '\xEC'
  {0x00,0x00,0x10,0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'í' '\xED'
  {0x00,0x00,0x20,0x50,0x60,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'î' '\xEE'
  {0x00,0x00,0x50,0x50,0x60,0x20,0x20,0x20,0x20,0x70,0x00,0x00},  // 'ï' '\xEF'
  {0x00,0xA0,0x40,0xA0,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'ð' '\xF0'
  {0x00,0x00,0x28,0x50,0xF0,0x88,0x88,0x88,0x88,0x88,0x00,0x00},  // 'ñ' '\xF1'
  {0x00,0x00,0x40,0x20,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'ò' '\xF2'
  {0x00,0x00,0x10,0x20,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'ó' '\xF3'
  {0x00,0x00,0x20,0x50,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'ô' '\xF4'
  {0x00,0x00,0x28,0x50,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'õ' '\xF5'
  {0x00,0x00,0x50,0x50,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00},  // 'ö' '\xF6'
  {0x00,0x00,0x00,0x20,0x20,0x00,0xF8,0x00,0x20,0x20,0x00,0x00},  // '÷' '\xF7'
  {0x00,0x00,0x00,0x00,0x34,0x48,0x58,0x68,0x48,0xB0,0x00,0x00},  // 'ø' '\xF8'
  {0x00,0x00,0x40,0x20,0x88,0x88,0x88,0x88,0x88,0x78,0x00,0x00},  // 'ù' '\xF9'
  {0x00,0x00,0x10,0x20,0x88,0x88,0x88,0x88,0x88,0x78,0x00,0x00},  // 'ú' '\xFA'
  {0x00,0x00,0x20,0x50,0x88,0x88,0x88,0x88,0x88,0x78,0x00,0x00},  // 'û' '\xFB'
  {0x00,0x00,0x50,0x50,0x88,0x88,0x88,0x88,0x88,0x78,0x00,0x00},  // 'ü' '\xFC'
  {0x00,0x00,0x10,0x20,0x88,0x88,0x88,0x88,0x88,0x78,0x08,0x70},  // 'ý' '\xFD'
  {0x00,0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x88,0xF0,0x80,0x80},  // 'þ' '\xFE'
  {0x00,0x00,0x50,0x50,0x88,0x88,0x88,0x88,0x88,0x78,0x08,0x70},  // 'ÿ' '\xFF'
};
