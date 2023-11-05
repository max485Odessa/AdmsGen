#include "TGlobalISR.h"


uint32_t TGLOBISR::disisr_cnt = 0;


void TGLOBISR::enable ()
{
if (disisr_cnt) disisr_cnt--;
if (!disisr_cnt) __enable_irq ();
}



void TGLOBISR::disable ()
{
	disisr_cnt++;
	__disable_irq ();
	//if (!disisr_cnt) __disable_irq ();
	
}


