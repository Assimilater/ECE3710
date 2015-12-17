#ifndef __FILTER_H__
#define __FILTER_H__

#include "../Shared/embedded_t.h"
#include "../Shared/Controller.h"

void Filter_Reset(uint);
bool Filter_IP(uint);
bool Filter_DNS(uint);
bool Filter_Deep(uint);


#endif
