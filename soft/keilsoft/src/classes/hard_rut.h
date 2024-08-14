#ifndef _H_HARDWARE_RUTINES_H_
#define _H_HARDWARE_RUTINES_H_

// HRDCPU = 1  (stm32f1xx)
// HRDCPU = 2  (stm32f2xx)
// HRDCPU = 3  (stm32f3xx)
// HRDCPU = 4  (stm32f4xx)

#if (HRDCPU == 1)
	#include "stm32f1xx_hal.h"
#endif
#if (HRDCPU == 2)
	#include "stm32f2xx_hal.h"
#endif
#if (HRDCPU == 3)
	#include "stm32f3xx_hal.h"
#endif
#if (HRDCPU == 4)
	#include "stm32f4xx_hal.h"
#endif



#if (HRDCPU == 1)
	#define C_HRDGPIOSPEED_LOW GPIO_SPEED_LOW
	#define C_HRDGPIOSPEED_MID GPIO_SPEED_MEDIUM
	#define C_HRDGPIOSPEED_HI GPIO_SPEED_HIGH
	#define C_HRDGPIO_NOPULL GPIO_NOPULL
	#define C_HRDGPIO_PULLUP GPIO_PULLUP
	#define C_HRDGPIO_PULLDW GPIO_PULLDOWN
#endif

#if (HRDCPU == 2)
	#define C_HRDGPIOSPEED_LOW
	#define C_HRDGPIOSPEED_MID
	#define C_HRDGPIOSPEED_HI
#endif

#if (HRDCPU == 3)
	#define C_HRDGPIOSPEED_LOW
	#define C_HRDGPIOSPEED_MID
	#define C_HRDGPIOSPEED_HI
#endif

#if (HRDCPU == 4)
	#define C_HRDGPIOSPEED_LOW GPIO_SPEED_LOW
	#define C_HRDGPIOSPEED_MID GPIO_SPEED_MEDIUM
	#define C_HRDGPIOSPEED_HI GPIO_SPEED_HIGH
	#define C_HRDGPIO_NOPULL GPIO_NOPULL
	#define C_HRDGPIO_PULLUP GPIO_PULLUP
	#define C_HRDGPIO_PULLDW GPIO_PULLDOWN
#endif


enum EHRTGPIOPULL {EHRTGPIOPULL_NO = C_HRDGPIO_NOPULL, EHRTGPIOPULL_UP = C_HRDGPIO_PULLUP, EHRTGPIOPULL_DOWN = C_HRDGPIO_PULLDW};
enum EHRTGPIOSPEED {EHRTGPIOSPEED_LOW = C_HRDGPIOSPEED_LOW, EHRTGPIOSPEED_MID = C_HRDGPIOSPEED_MID, EHRTGPIOSPEED_HI = C_HRDGPIOSPEED_HI};
enum ESYSTIM {ESYSTIM_TIM1 = 0, ESYSTIM_TIM2, ESYSTIM_TIM3, ESYSTIM_TIM4, ESYSTIM_TIM5, ESYSTIM_TIM6, ESYSTIM_TIM7, ESYSTIM_TIM8, ESYSTIM_TIM9, ESYSTIM_TIM10, ESYSTIM_TIM11, ESYSTIM_TIM12, ESYSTIM_ENDENUM};
enum EGPMD {EGPMD_OD, EGPMD_PP, EGPMD_IN, EGPMD_AIN, EGPMD_ENDENUM};
enum EGPINTMOD {EGPINTMOD_RISING = 0, EGPINTMOD_FALLING = 1, EGPINTMOD_RISING_FALLING = 2, EGPINTMOD_ENDENUM};



typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} S_GPIOPIN;



typedef struct {
	S_GPIOPIN pin;
	EGPMD mod;
	bool out_mod_def_set;		// only for output modes: EGPMD_OD, EGPMD_PP
} S_GPMD_PIN_T;


typedef struct {
	S_GPIOPIN mosi;
	S_GPIOPIN miso;
	S_GPIOPIN sck;
	S_GPIOPIN cs;
} S_SPIGPIO_T;


#if (HRDCPU == 1)
void _pin_low_init_out_pp_af (S_GPIOPIN *lp_pin , EHRTGPIOSPEED sp);
void _pin_low_init_out_od_af (S_GPIOPIN *lp_pin, EHRTGPIOSPEED sp );
#endif
#if (HRDCPU == 2 || HRDCPU == 3 || HRDCPU == 4)
void _pin_low_init_out_pp_af ( uint8_t af_codemux, S_GPIOPIN *lp_pin , EHRTGPIOSPEED sp);
void _pin_low_init_out_od_af ( uint8_t af_codemux, S_GPIOPIN *lp_pin, EHRTGPIOSPEED sp );
#endif

TIM_TypeDef *hard_get_tim (ESYSTIM t, bool *f_wdth_32);
void hard_tim_clock_enable (ESYSTIM tn);
void hard_usart_clock_enable (USART_TypeDef *p);
void hard_gpio_clock_enable (GPIO_TypeDef *port);
void _pin_low_init (S_GPMD_PIN_T *lp_pin, unsigned char cnt, EHRTGPIOSPEED sp, EHRTGPIOPULL pl);
void _pin_low_init_out_od (S_GPIOPIN *lp_pin, unsigned char cnt, EHRTGPIOSPEED sp);
void _pin_low_init_out_pp ( S_GPIOPIN *lp_pin, uint8_t cnt, EHRTGPIOSPEED sp );

void _pin_low_init_in (S_GPIOPIN *lp_pin, uint8_t cnt, EHRTGPIOSPEED sp, EHRTGPIOPULL pl);
void _pin_low_init_int (S_GPIOPIN *lp_pin, unsigned char cnt, EGPINTMOD md, EHRTGPIOSPEED sp);
void _pin_low_init_adc ( S_GPIOPIN *lp_pin, uint8_t cnt, EHRTGPIOSPEED sp );
bool _pin_input ( S_GPIOPIN *lp_pin );
void _pin_output ( S_GPIOPIN *lp_pin, bool val );
#define _pin_set_to(x,y) _pin_output(x,y)
#define _pin_get(x) _pin_input(x)

#endif

