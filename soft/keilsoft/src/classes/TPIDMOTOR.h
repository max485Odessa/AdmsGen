#ifndef _H_PIDMOTOR_CLASS_H_
#define _H_PIDMOTOR_CLASS_H_



#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "THALLDIG.h"
#include "rutine.h"
#include "TPWM.h"






class TPIDPWM : public TFFC, public IFHALLSYNCPULSECB {
		TPWMIFC *pwm;
		//const uint32_t c_check_period;
		//SYSBIOS::Timer time_check;
	
		//float const *c_freq_data;	// где брать текущую скорость
	
		float need_freq;	// необходима€ частота

		float c_set_cur_p;	
		float c_set_cur_i;	// прирост интегральной составл€ющей
		float acc_cur_i;
		bool f_enable_sys;
		bool f_enable_pd;
		bool f_enable_i;
		
		virtual void Task () override;		// iface TFFC

		float last_error_freq;
	
		virtual void cb_ifhallsync_pulse (EHALLPULSESYNC rslt, uint32_t delt_mks) override;		// iface IFHALLSYNCPULSECB
		EHALLPULSESYNC last_sync_rslt;
		uint32_t last_delta_mks;
		bool f_update_sync;
	
	public:
		TPIDPWM (TPWMIFC *pwmi);
		void set_freq (float hz);
		void set_p (float p);
		void set_i (float i);
		void enable_sys (bool val);
		void enable_pd (bool val);
		void enable_i (bool val);
		
};


#endif
