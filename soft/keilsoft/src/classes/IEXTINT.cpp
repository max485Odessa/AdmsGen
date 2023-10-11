#include "IEXTINT.h"
#include "stm32f4xx_hal_cortex.h"


IEXTINT_ISR *IEXTINT_ISR::isr_this[C_MAXGPIOPININTERRUPT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//IEXTINT_ISR *IEXTINT_ISR::isr_this[C_MAXGPIOPININTERRUPT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const uint8_t isrnumbarr[C_MAXGPIOPININTERRUPT] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, \
EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn};

#ifdef __cplusplus
 extern "C" {
#endif 

	 
void EXTI0_IRQHandler ()
{
	IEXTINT_ISR::isr_this[0]->gpio_isr (GPIO_PIN_0);
}



void EXTI1_IRQHandler ()
{
	IEXTINT_ISR::isr_this[1]->gpio_isr (GPIO_PIN_1);

}



void EXTI2_IRQHandler ()
{
	IEXTINT_ISR::isr_this[2]->gpio_isr (GPIO_PIN_2);
	
}



void EXTI3_IRQHandler ()
{
	IEXTINT_ISR::isr_this[3]->gpio_isr (GPIO_PIN_3);
}



void EXTI4_IRQHandler ()
{
	IEXTINT_ISR::isr_this[4]->gpio_isr (GPIO_PIN_4);
}


void EXTI9_5_IRQHandler ()
{
	uint16_t start_mask = GPIO_PIN_5, ix = 5;
	uint32_t mask = EXTI->PR;
	while (ix <= 9)
		{
		if (mask & start_mask) IEXTINT_ISR::isr_this[ix]->gpio_isr (start_mask);
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
		if (mask & start_mask) IEXTINT_ISR::isr_this[ix]->gpio_isr (start_mask);
		start_mask <<= 1;
		ix++;
		}
}

	 
	 
#ifdef __cplusplus
}
#endif



void IEXTINT_ISR::gpio_isr (uint16_t pinn)
{
	__HAL_GPIO_EXTI_CLEAR_IT (pinn);
	GPIO_PinState stt = HAL_GPIO_ReadPin(c_pin_in->port, c_pin_in->pin);
	isr_gpio_cb_int (c_isr_nmbr, stt);
}



IEXTINT_ISR::IEXTINT_ISR (S_GPIOPIN *p, EGPINTMOD md)
{
	c_pin_in = p;
	long ix = gpio_pin_ix_from_mask (p->pin);
	if (ix != -1) 
		{
		c_isr_nmbr = ix;
		isr_this[ix] = this;
		_pin_low_init_int (c_pin_in, 1, md);
		enable_extint_isr (true);
		}
}



void IEXTINT_ISR::enable_extint_isr (bool st)
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



long IEXTINT_ISR::gpio_pin_ix_from_mask (uint16_t msk)
{
	long rv = -1, ix = 0;
	while (ix < C_MAXGPIOPININTERRUPT)
		{
		if (msk & 1)
			{
			rv = ix;
			break;
			}
		ix++;
		}
	return rv;
}


