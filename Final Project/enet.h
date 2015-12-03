#ifndef __WIZ550_H__
#define __WIZ550_H__

//---------------------------------------------------------------------------------------+
// Register definitions                                                                  |
//---------------------------------------------------------------------------------------+

#define NET_PC_CS BAND_GPIO_PE0
#define NET_ISP_CS BAND_GPIO_PE1

void NET_WriteCmd(short, char);
void NET_READDATA(void);
void NET_PARSEDATA(void);
void NET_WRITEDATA(void);

void NET_Init(void);

#endif
