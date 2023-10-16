#ifndef _H_HARDWARE_RUTINES_H_
#define _H_HARDWARE_RUTINES_H_



#include "stm32f4xx_hal.h"

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


void hard_usart_clock_enable (USART_TypeDef *p);
void hard_gpio_clock_enable (GPIO_TypeDef *port);
void _pin_low_init (S_GPMD_PIN_T *lp_pin, unsigned char cnt);
void _pin_low_init_out_od ( S_GPIOPIN *lp_pin, uint8_t cnt );
void _pin_low_init_out_pp ( S_GPIOPIN *lp_pin, uint8_t cnt );
void _pin_low_init_out_pp_af ( uint8_t af_codemux, S_GPIOPIN *lp_pin );
void _pin_low_init_in ( S_GPIOPIN *lp_pin, uint8_t cnt );
void _pin_low_init_in_pull (S_GPIOPIN *lp_pin, uint8_t cnt, bool vl);
void _pin_low_init_int (S_GPIOPIN *lp_pin, unsigned char cnt, EGPINTMOD md);
void _pin_low_init_adc ( S_GPIOPIN *lp_pin, uint8_t cnt );
bool _pin_input ( S_GPIOPIN *lp_pin );
void _pin_output ( S_GPIOPIN *lp_pin, bool val );
#define _pin_set_to(x,y) _pin_output(x,y)
#define _pin_get(x) _pin_input(x)

#endif

