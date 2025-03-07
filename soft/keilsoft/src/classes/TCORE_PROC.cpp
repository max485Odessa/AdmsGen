#include "TCORE.h"
#include "hard_rut.h"


/*
void TCORERCT::ptocess_isr_task ()
{
	bool f_cr_propulsine = false;
	bool f_opto = false;
	
	pd_state = motor_pid->is_state ();
	
	do {
		
		if (!f_system_state) break;
		
		float freq_hz = rectifier_contrl->get_freq ();
		uint32_t rpm = freq_hz * 60;//rectifier_contrl->getrpm ();
		//motor_pid->set_freq (freq_hz);
		// check minimal speed
		if (rpm < seting_start_sys_rpm) break;
		// check to bad settings...
		if (seting_sys_voltage_on < C_MINSRC_VOLTAGE || seting_sys_voltage_off < C_MINSRC_VOLTAGE) 
			{
			// error messages
			break;
			}

		float cur_src_volt = ptxt_v320_v->get_f ();
		
		// check src voltage wideband
		if (cur_src_volt >= seting_sys_voltage_on) f_opto = true;		// не трансформировать если напряжение не превысило минимальный порог
		if (cur_src_volt <= seting_sys_voltage_off) f_cr_propulsine = true;	// блокировка преобразования если напряжение превысило максимальный порог

	
	} while (false);

	xdc_optocoupler_en (f_opto);					// enable or disable src optocoupler for small voltage
	rectifier_contrl->enable (f_cr_propulsine);	// enable or disable propulsine system
	motor_pid->enable_sys (f_system_state);			// enable or disable motor system

	f_optocoupler = f_opto;
	f_pulses_state = f_cr_propulsine;
}
*/



void TCORERCT::ptocess_isr_task ()
{
	bool f_cr_propulsine = false;
	bool f_opto = false;
	bool f_motor_state = false;
	pd_state = motor_pid->is_state ();
	
	do {
		
		if (!f_system_state) break;
		
		f_motor_state = f_system_state;	// motor on
		
		float freq_hz = rectifier_contrl->get_freq ();
		uint32_t rpm = freq_hz * 60;//rectifier_contrl->getrpm ();
		//motor_pid->set_freq (freq_hz);
		// check minimal speed
		if (rpm < seting_start_sys_rpm) break;
		// check to bad settings...
		if (seting_sys_voltage_on < C_MINSRC_VOLTAGE || seting_sys_voltage_off < C_MINSRC_VOLTAGE) 
			{
			// error messages
			break;
			}

		float cur_src_volt = ptxt_v320_v->get_f ();
		
		// check src voltage wideband
		if (cur_src_volt >= seting_sys_voltage_on) f_opto = true;						// не включать БП, если напряжение не превысило стартовый порог
		if (cur_src_volt >= seting_sys_voltage_off) f_motor_state = false;	// отключение мотора если напряжение критическое

	
	} while (false);

	xdc_optocoupler_en (f_opto);					// enable or disable src optocoupler for small voltage
	rectifier_contrl->enable (f_cr_propulsine);	// enable or disable propulsine system
	motor_pid->enable_sys (f_motor_state);			// enable or disable motor system

	f_optocoupler = f_opto;
	f_pulses_state = f_cr_propulsine;
}



