#ifndef _H_EXTERNAL_INTERRUPT_CLASS_H_
#define _H_EXTERNAL_INTERRUPT_CLASS_H_



#include "stm32f4xx_hal.h"
#include "hard_rut.h"



#define C_MAXGPIOPININTERRUPT 16



class IEXTINT_ISR {
	protected:
		uint8_t c_isr_nmbr;
		S_GPIOPIN *c_pin_in;
		virtual void isr_gpio_cb_int (uint8_t isr_n, bool pinstate) = 0;
		IEXTINT_ISR (S_GPIOPIN *p, EGPINTMOD md);
	
	public:
		static long gpio_pin_ix_from_mask (uint16_t msk);
		void gpio_isr (uint16_t pinn);
		void enable_extint_isr (bool st);
		static IEXTINT_ISR *isr_this[C_MAXGPIOPININTERRUPT];
};




#endif
