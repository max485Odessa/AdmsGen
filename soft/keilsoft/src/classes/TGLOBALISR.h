#ifndef _H_GLOBAL_ISR_H_
#define _H_GLOBAL_ISR_H_

#include "stm32f4xx_hal.h"


class TGLOBISR {
	static uint32_t disisr_cnt;
	public:
		static void enable ();
		static void disable ();
};


#endif