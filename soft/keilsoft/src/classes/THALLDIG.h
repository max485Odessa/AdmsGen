#ifndef _H_STM32HALL_DIGITAL_HALL_H_
#define _H_STM32HALL_DIGITAL_HALL_H_

#include "stm32f4xx_hal.h"
#include "hard_rut.h"
#include "IEXTINT.h"
#include "ITIMINT.h"
#include "TFTASKIF.h"
#include "SYSBIOS.H"
#include "TDRAWPARAM.h"


class IFHALLPHASECB {
	public:
		virtual void cb_ifhall_phase (uint32_t ps) = 0;
};



enum EHALLPULSESYNC {EHALLPULSESYNC_OK = 0, EHALLPULSESYNC_TIMEOUT, EHALLPULSESYNC_ENDENUM};



class IFHALLSYNCPULSECB {
	public:
		virtual void cb_ifhallsync_pulse (EHALLPULSESYNC rslt, uint32_t delt_mks) = 0;
};



typedef struct {
	float angl;
	uint32_t en_name;
	uint32_t oc_offset;
	uint32_t counter;
	bool enable;
} S_HALPOINT_T;



enum EHLDGTIMUID {EHLDGTIMUID_1000MS = 0};


enum EHALPAPAM {EHALPAPAM_RPM = 0, EHALPAPAM_ENDENUM};

// модуль дискретного датчика хола
// Функционал:
// по IEXTISRCB интерфейсу получает сигнал синхронизации (прерывания по внешнему сигналу), измеряет длительность между прерываниями
// вычисляет RPM, устанавливает угловую меру для для точек прерывания, управление по прерываниям передается в IFHALLCB интерфейс
	class THALLDIG: public TFFC, public ITIMCB, public IEXTISRCB, protected SYSBIOS::TimerCB, public TPARAMCB {
		S_HALPOINT_T *points;
		const uint8_t c_points_n;
		const uint8_t c_phases_n;
		long add_points_ix;
		long find_point_index (uint32_t e);
		void sorted_point ();
		void recalculate_point_offsets (uint32_t scnnt);

		long search_ix;
	
		IFHALLPHASECB *phase_cb;
		IFHALLSYNCPULSECB *sync_cb;
		
		virtual void timer_cb (uint32_t id) override;		// iface TimerCB
		SYSBIOS::TCBHANDLE *timcb_isr_handle_1000ms;

		TTIM_MKS_ISR *etim;	
		TEXTINT_ISR *extisr;
		EPWMCHNL used_ch;
		virtual void isr_gpio_cb_isr (uint8_t isr_n, bool pinstate);	// iface IEXTISRCB
		virtual void tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch) override;		// iface ITIMCB
		uint32_t acc_current_period_value;
		uint32_t prev_period_value;
		uint32_t c_treshold_step;
	
		void add_acc_period (uint32_t v);
		void sync_reset ();
	
		float c_offset_angl;
		float c_period_quantangle;

		
		bool f_enable;
		
		virtual float get_param_f (uint32_t ix) override;
		virtual uint32_t get_param_u32 (uint32_t ix) override;
		virtual int32_t get_param_i32 (uint32_t ix) override;
	
	public:
		THALLDIG (TTIM_MKS_ISR *t, TEXTINT_ISR *ei, EPWMCHNL usdch, uint8_t npnt, uint8_t phas_n);		// S_GPIOPIN *p, EGPINTMOD md S_GPIOPIN *c_pout,
		void set_phase_cb (IFHALLPHASECB *c);
		void set_sync_cb (IFHALLSYNCPULSECB *c);
		
		// добавить точку прерывания
		bool add_replace_point (float angl, uint32_t enname);
		void enable_point (uint32_t enname, bool enbl);
		float get_point (uint32_t enname);
	
		void setoffset (float angl);
		void enable (bool v);
		uint32_t getrpm ();
		float get_freq ();
};



#endif
