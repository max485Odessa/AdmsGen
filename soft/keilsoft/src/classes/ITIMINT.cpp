#include "ITIMINT.h"



static const uint8_t isrnumbarr[ESYSTIM_ENDENUM] = {TIM2_IRQn, TIM5_IRQn};
static const TIM_TypeDef *artims[ESYSTIM_ENDENUM] = {TIM2, TIM5};
static ITIM_ISR *isr_this[ESYSTIM_ENDENUM] = {0,0};
static const uint32_t chanarr[4] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};



#ifdef __cplusplus
 extern "C" {
#endif 

	 

void TimIsrFreqMeter (ITIM_ISR *o)
{
	ITIM_ISR &obj = *o;
	obj.isr_tim ();
}	


	 
void TIM2_IRQHandler ()
{
	TimIsrFreqMeter (isr_this[ESYSTIM_TIM2]);
}



void TIM5_IRQHandler ()
{
	TimIsrFreqMeter (isr_this[ESYSTIM_TIM5]);
}



#ifdef __cplusplus
}
#endif



ITIM_ISR::ITIM_ISR (ESYSTIM t)
{
	TimHandle.Instance = get_tim (t);
	isr_this[t] = this;
	tim_ix = t;
	uint8_t ix = 0;
	while (ix < EPWMCHNL_ENDENUM)
		{
		f_active_isr[ix] = false;
		a_pwmvalue[ix] = 0;
		ix++;
		}
}



void ITIM_ISR::clr_tim ()
{
	TimHandle.Instance->CNT = 0;
}



uint32_t ITIM_ISR::get_timer_counter ()
{
	return TimHandle.Instance->CNT;
}



// isr context executed
void ITIM_ISR::isr_tim ()
{
	if(__HAL_TIM_GET_IT_SOURCE (&TimHandle, TIM_IT_CC1) !=RESET)
    {
			tim_comp_cb_isr (tim_ix, EPWMCHNL_PWM1);
		 __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC1);
		}
	if(__HAL_TIM_GET_IT_SOURCE (&TimHandle, TIM_IT_CC2) !=RESET)
    {
			tim_comp_cb_isr (tim_ix, EPWMCHNL_PWM2);
		 __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC2);
		}
	if(__HAL_TIM_GET_IT_SOURCE (&TimHandle, TIM_IT_CC3) !=RESET)
    {
			tim_comp_cb_isr (tim_ix, EPWMCHNL_PWM3);
		 __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC3);
		}
	if(__HAL_TIM_GET_IT_SOURCE (&TimHandle, TIM_IT_CC4) !=RESET)
    {
			tim_comp_cb_isr (tim_ix, EPWMCHNL_PWM4);
		 __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC4);
		}
	if(__HAL_TIM_GET_IT_SOURCE (&TimHandle, TIM_IT_UPDATE) !=RESET)
    {
			tim_comp_cb_isr (tim_ix, EPWMCHNL_UPDATE);
		 __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_UPDATE);
		}
}



TIM_TypeDef *ITIM_ISR::get_tim (ESYSTIM t)
{
	TIM_TypeDef *rv = 0;
	if (t < ESYSTIM_ENDENUM) rv = const_cast<TIM_TypeDef*>(artims[t]);
	return rv;
}



void ITIM_ISR::timer_init (uint32_t period, uint32_t hz)
{
TIM_OC_InitTypeDef   sConfig;
uint32_t uwPrescalerValue = (uint32_t) ((SystemCoreClock / hz) - 1);

  
  /* Initialize TIMx peripheral as follow:
       + Period = 10000 - 1
       + Prescaler = ((SystemCoreClock/2)/hz) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period = period - 1;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&TimHandle);
	HAL_TIM_Base_Start (&TimHandle);
}



void ITIM_ISR::enable_timer_isr (bool st)
{
	IRQn_Type tp = (IRQn_Type)(isrnumbarr[tim_ix]);
	if (st)
		{
		HAL_NVIC_EnableIRQ (tp);
		}
	else
		{
		HAL_NVIC_DisableIRQ (tp);
		}
}



void ITIM_ISR::set_timer_oc_value (EPWMCHNL c, uint32_t v)
{
	if (c < EPWMCHNL_ENDENUM) a_pwmvalue[c] = v;
}



void ITIM_ISR::enable_timer_oc (EPWMCHNL c, bool state)
{
	if (c < EPWMCHNL_ENDENUM)
		{
		TIM_OC_InitTypeDef sConfig;

		if (state)
			{
			sConfig.OCMode  = TIM_OCMODE_ACTIVE;
			sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

			sConfig.Pulse = a_pwmvalue[c];  
			HAL_TIM_OC_ConfigChannel (&TimHandle, &sConfig, chanarr[c]);
			HAL_TIM_OC_Start_IT (&TimHandle, chanarr[c]);
			}
		else
			{
			HAL_TIM_OC_Stop_IT (&TimHandle, chanarr[c]);
			}
		f_active_isr[c] = state;
		}
}





