#ifndef _H_STM32HALL_TAWAVE_CLASS_H_
#define _H_STM32HALL_TAWAVE_CLASS_H_

#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"

enum EPHASEIX {EPHASEIX_A = 0, EPHASEIX_B = 1, EPHASEIX_C = 2, EPHASEIX_ENDENUM = 3};


typedef struct {
	uint16_t phases[EPHASEIX_ENDENUM];
} S_AWAVEELEMNT_T;


class TAWAVE {
		void init ();
		__IO uint16_t *ADCxConvertedValue;
		uint32_t c_full_buf_size;
	
	public:
		TAWAVE (uint16_t elm_cnt);
		uint32_t get_scan_time ();	// время сканирования элемента
};

#endif
