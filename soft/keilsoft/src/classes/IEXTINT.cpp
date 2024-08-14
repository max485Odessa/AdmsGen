#include "IEXTINT.h"
#include "stm32f4xx_hal_cortex.h"



TEXTINT_ISR *TEXTINT_ISR::isr_this[C_MAXGPIOPININTERRUPT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//IEXTINT_ISR *IEXTINT_ISR::isr_this[C_MAXGPIOPININTERRUPT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const uint8_t isrnumbarr[C_MAXGPIOPININTERRUPT] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, \
EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn};



#ifdef __cplusplus
 extern "C" {
#endif 

	 
void EXTI0_IRQHandler ()
{
	TEXTINT_ISR::isr_this[0]->gpio_isr (GPIO_PIN_0);
}



void EXTI1_IRQHandler ()
{
	TEXTINT_ISR::isr_this[1]->gpio_isr (GPIO_PIN_1);

}



void EXTI2_IRQHandler ()
{
	TEXTINT_ISR::isr_this[2]->gpio_isr (GPIO_PIN_2);
	
}



void EXTI3_IRQHandler ()
{
	TEXTINT_ISR::isr_this[3]->gpio_isr (GPIO_PIN_3);
}



void EXTI4_IRQHandler ()
{
	TEXTINT_ISR::isr_this[4]->gpio_isr (GPIO_PIN_4);
}



void EXTI9_5_IRQHandler ()
{
	uint16_t start_mask = GPIO_PIN_5, ix = 5;
	uint32_t mask = EXTI->PR;
	while (ix <= 9)
		{
		if (mask & start_mask) TEXTINT_ISR::isr_this[ix]->gpio_isr (start_mask);
		start_mask <<= 1;
		ix++;
		}
}



void EXTI15_10_IRQHandler ()
{
	uint16_t start_mask = GPIO_PIN_10, ix = 10;
	uint32_t mask = EXTI->PR;
	while (ix <= 15)
		{
		if (mask & start_mask) TEXTINT_ISR::isr_this[ix]->gpio_isr (start_mask);
		start_mask <<= 1;
		ix++;
		}
}

	 
	 
#ifdef __cplusplus
}
#endif



void TEXTINT_ISR::set_cb (IEXTISRCB *cb)
{
	cb_exec = cb;
}



void TEXTINT_ISR::gpio_isr (uint16_t pinn)
{
	GPIO_PinState stt = HAL_GPIO_ReadPin(c_pin_in->port, c_pin_in->pin);
	if (cb_exec) cb_exec->isr_gpio_cb_isr (c_isr_nmbr, stt);
	__HAL_GPIO_EXTI_CLEAR_IT (pinn);
}



TEXTINT_ISR::TEXTINT_ISR (S_GPIOPIN *p, EGPINTMOD md)
{
	cb_exec = 0;
	c_pin_in = p;
	long ix = gpio_pin_ix_from_mask (p->pin);
	if (ix != -1) 
		{
		c_isr_nmbr = ix;
		isr_this[ix] = this;
		_pin_low_init_int (c_pin_in, 1, md, EHRTGPIOSPEED_HI);
		enable_extint_isr (true);
		}
}



void TEXTINT_ISR::enable_extint_isr (bool st)
{
	IRQn_Type tp = (IRQn_Type)(isrnumbarr[c_isr_nmbr]);
	if (st)
		{
		HAL_NVIC_EnableIRQ (tp);
		}
	else
		{
		HAL_NVIC_DisableIRQ (tp);
		}
}



long TEXTINT_ISR::gpio_pin_ix_from_mask (uint16_t msk)
{
	long rv = -1, ix = 0;
	while (ix < C_MAXGPIOPININTERRUPT)
		{
		if (msk & 1)
			{
			rv = ix;
			break;
			}
		msk >>= 1;
		ix++;
		}
	return rv;
}


