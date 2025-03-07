#ifndef _H_PIDMOTOR_CLASS_H_
#define _H_PIDMOTOR_CLASS_H_



#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "THALLDIG.h"
#include "rutine.h"
#include "TPWM.h"



enum EPIDSTATE {EPIDSTATE_OFF = 0, EPIDSTATE_STAB, EPIDSTATE_INC, EPIDSTATE_DEC, EPIDSTATE_ENDENUM};
#define C_SEQ_ERROR_PID_VIEW 2


class TPIDPWM : public TFFC, public IFHALLSYNCPULSECB {
		TPWMIFC *pwm;
		//const uint32_t c_check_period;
		//SYSBIOS::Timer time_check;
	
		//float const *c_freq_data;	// где брать текущую скорость
	
		float need_freq;	// необходима€ частота
		long seq_pole_error_plus_cnt;
		long seq_pole_error_minus_cnt;
		
		float freq_mult_value;
		float c_set_cur_p;	
		float c_set_cur_i;	// прирост интегральной составл€ющей
		float acc_cur_i;
		float acc_cur_d;
		bool f_enable_sys;
		//bool f_enable_p;
		bool f_enable_i;
		bool f_enable_d;
	
		float stab_cinetic_power;
		
		virtual void Task () override;		// iface TFFC
	
		EPIDSTATE last_pid_state;

		float last_error_freq;
	
		virtual void cb_ifhallsync_pulse (EHALLPULSESYNC rslt, uint32_t delt_mks) override;		// iface IFHALLSYNCPULSECB
		EHALLPULSESYNC last_sync_rslt;
		uint32_t last_delta_mks;
		bool f_update_sync;
		
		//void enable_p (bool val);
		void enable_i (bool val);
		void enable_d (bool val);
	
	public:
		TPIDPWM (TPWMIFC *pwmi);
		void set_freq (float hz);
		void freq_mult (float v);
		void set_p (float p);
		void set_i (float i);
		void enable_sys (bool val);
	
		EPIDSTATE is_state ();

		
};


#endif
