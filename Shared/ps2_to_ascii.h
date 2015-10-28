// This is a look up table for basic key strokes. 
// This translates from ps/2 to ascii. 
// The table starts at 0x15, so you should shift 
// the table by 0x15 when you reference it. 
// the table ends at 0x4D (meaning last index is 0x38)
unsigned char ps2_to_ascii[] = {
	'q','1',0x00,0x00,0x00,'z','s','a','w','2',0x00,
	0x00,'c','x','d','e','4','3',0x00,0x00,' ','v','f','t','r','5',0x00,0x00,
	'n','b','h','g','y','6',0x00,0x00,0x00,'m','j','u','7','8',0x00,
	0x00,',','k','i','o','0','9',0x00,0x00,'.',0x00,'l','p'};