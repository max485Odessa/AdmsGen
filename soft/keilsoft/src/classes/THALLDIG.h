#ifndef _H_STM32HALL_DIGITAL_HALL_H_
#define _H_STM32HALL_DIGITAL_HALL_H_

#include "stm32f4xx_hal.h"
#include "hard_rut.h"
#include "IEXTINT.h"
#include "ITIMINT.h"
#include "TFTASKIF.h"
#include "SYSBIOS.H"



class IFHALLCB {
	public:
		virtual void cb_ifhall (uint32_t ps) = 0;
};



typedef struct {
	float angl;
	uint32_t en_name;
	uint32_t oc_offset;
	uint32_t counter;
	bool enable;
} S_HALPOINT_T;



// модуль дискретного датчика хола
// Функционал:
// по IEXTISRCB интерфейсу получает сигнал синхронизации (прерывания по внешнему сигналу), измеряет длительность между прерываниями
// вычисляет RPM, устанавливает угловую меру для для точек прерывания, управление по прерываниям передается в IFHALLCB интерфейс
class THALLDIG: public TFFC, public ITIMCB, public IEXTISRCB {
		S_HALPOINT_T *points;
		const uint8_t c_points_n;
		long add_points_ix;
		long find_point_index (uint32_t e);
		void sorted_point ();
		void recalculate_point_offsets (uint32_t scnnt);

		long search_ix;
	
		IFHALLCB *cb;
		//ESYSTIM etim;
		TTIM_MKS_ISR *etim;	
		TEXTINT_ISR *extisr;
		EPWMCHNL used_ch;
		virtual void isr_gpio_cb_isr (uint8_t isr_n, bool pinstate);
		virtual void tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch) override;	
		uint32_t acc_current_period_value;
		uint32_t prev_period_value;
		uint32_t c_treshold_step;
	
		void add_acc_period (uint32_t v);
		void sync_reset ();
	
		float c_offset_angl;
		float c_period_quantangle;

		
		S_GPIOPIN *c_pin_out;
		bool f_enable;
	
	public:
		THALLDIG (TTIM_MKS_ISR *t, TEXTINT_ISR *ei, EPWMCHNL usdch, S_GPIOPIN *c_pout, uint8_t npnt);		// S_GPIOPIN *p, EGPINTMOD md
		void set_cb (IFHALLCB *c);
		
		// добавить точку прерывания
		bool add_replace_point (float angl, uint32_t enname);
		void enable_point (uint32_t enname, bool enbl);
		float get_point (uint32_t enname);
	
		void setoffset (float angl);
		void enable (bool v);
		uint32_t getrpm ();
};


#endif
