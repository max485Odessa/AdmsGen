#ifndef _H_ADAMS_RECTIFIER_CORE_H_
#define _H_ADAMS_RECTIFIER_CORE_H_


#include "stm32f4xx_hal.h"
#include "TAIN.h"
#include "THALLDIG.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "ST7565R_SPI.H"
#include "LCD132x64.h"
#include "TEASYKEYS.h"
#include "TM24Cxxx.h"
#include "STMSTRING.h"
#include "TPARAMCONTROL.h"
#include "TINA226.h"
#include "TAIN.h"
#include "TPIDMOTOR.h"
#include "TDRAWPARAM.h"


enum EKEYSID {EKEYSID_RIGHT = 0, EKEYSID_LEFT, EKEYSID_SELECT, EKEYSID_MENU , EKEYSID_ONOFF, EKEYSID_ENDENUM};
enum EPRMIX {\
	EPRMIX_AUTOSTART_ENABLE = 0, /* автостарт при включении питания */ \
	EPRMIX_WORK_MOTOR_RPM , /* номинальная скорость мотора в герцах */ \
	EPRMIX_START_SYS_RPM , /* скорость начала работы системы */ \
	EPRMIX_MOTOR_P , /* настройка управления мотором */ \
	EPRMIX_MOTOR_D , /* настройка управления мотором */ \
	EPRMIX_R_ANGLE_ON , /* настройка системы коммутации */ \
	EPRMIX_R_ANGLE_OFF , /* настройка системы коммутации */ \
	EPRMIX_MAGNETS_N , /* общее количество магнитов */ \
	EPRMIX_VOLT_ON , /* напряжение при котором разрешается преобразование */ \
	EPRMIX_VOLT_OFF , /* напряжение при котором запрещается преобразование */ \
	EPRMIX_R_ANGLE_OFFSET, /* угловое смещение относительно датчика хола (в пределах от 0 до 359.5 градусов)  */ \
  EPRMIX_ENDENUM };



enum EANGLPCOD {EANGLPCOD_A = 0, EANGLPCOD_B, EANGLPCOD_C, EANGLPCOD_D, EANGLPCOD_ENDENUM};


#pragma pack (push, 1)


#pragma pack (pop)


typedef struct {
	EKEYSID key;
	EJSTMSG msg;
} S_PGMESSAGE_T;



#define C_GRAPHICS_TIMS_AMOUNT 12

// графические таймера визуального обновления величины
enum EGTIM_W_MAIN {EGTIM_W_MAIN_STATE = 0, EGTIM_W_MAIN_RPM, EGTIM_W_MAIN_SRC_320V, EGTIM_W_MAIN_DST24_V, EGTIM_W_MAIN_MOT_V, EGTIM_W_MAIN_BAT12V, EGTIM_W_MAIN_MOT_A, EGTIM_W_MAIN_DST24_A, EGTIM_W_MAIN_PULSES, EGTIM_W_MAIN_OPTO, EGTIM_W_ERROR_PID, EGTIM_W_MAIN_ENDENUM};
enum EPAGE {EPAGE_NONE = 0, EPAGE_MAIN, EPAGE_PARAM_EDIT, EPAGE_PARAM_LIST, EPAGE_ENDENUM};
enum ECOREPDC {ECOREPDC_TIMS = 0, ECOREPDC_PROCESS, ECOREPDC_ENDENUM};

#define C_MINSRC_VOLTAGE 150.0F
#define C_MAXSRC_VOLTAGE 400.0F
#define C_MINIMAL_ANGLE_WIDEBAND 1.0F

class TCORERCT: public TFFC, public IFHALLPHASECB, public SYSBIOS::TimerCB {
		uint8_t gui_item_param_height ();
		uint8_t border_updown_height ();
		uint8_t gui_dislp_item_cnt ();
	
		long edit_param_ix;
		long cursor_param_ix;
		long view_param_start_ix;
		void update_view_start ();
		bool f_is_edit_param_mode;
		bool f_lcd_needupdate;
	
		void cb_ifhall_phase (uint32_t ps) override;		// iface IFHALLCB
	
		float *ain_array[EAINPIN_ENDENUM];

	
		//void print_rpm_small (short x, short y, uint32_t rpm);
		void print_voltage_small (short x, short y, EAINCH ch);
	
		
		virtual void timer_cb (uint32_t id) override;
		SYSBIOS::Timer ww_timer;	// auto update timer
		SYSBIOS::TCBHANDLE *gr_timer;
		uint32_t grph_tims[C_GRAPHICS_TIMS_AMOUNT];	
		uint8_t max_gr_tims;
		
		void ptocess_isr_task ();
		SYSBIOS::TCBHANDLE *proc_cb_isr_task;
		
		uint32_t seting_work_rpm;
		uint32_t seting_start_sys_rpm;
		float seting_motor_P;
		float seting_motor_I;
		float seting_angle_pulse_on;
		float seting_angle_pulse_off;
		float seting_angle_offset;
		uint32_t seting_magnets_pair;
		float seting_sys_voltage_on;
		float seting_sys_voltage_off;
		
		void param_angles_aply ();
	
	protected:
		virtual void Task () override;
		uint8_t strtemporarymem[128];
		TSTMSTRING str_tmp;
	
		TGRAPHPARAM *ptxt_state;	// ok
		TGRAPHPARAM *ptxt_rpm;		// pk
		TGRAPHPARAM *ptxt_v320_v;	// src
		TGRAPHPARAM *ptxt_v24_v;	// out
		TGRAPHPARAM *ptxt_v24_a;	// out
		TGRAPHPARAM *ptxt_v12bat_v;	// bat
		TGRAPHPARAM *ptxt_mot_v;	// mot
		TGRAPHPARAM *ptxt_mot_a;	// mot
		TGRAPHPARAM *ptxt_pulses;	// phase pulses
		TGRAPHPARAM *ptxt_optocoupler;	// optocoupler
		TGRAPHPARAM *ptxt_pdenum;	// optocoupler
		
		bool f_system_state;
		bool f_pulses_state;
		bool f_optocoupler;
	
		uint8_t pd_state;

		//void draw_param (short x, short y, TGRAPHPARAM *p);
	
		TPARAMCONTRL *params;
	
		THALLDIG *rectifier_contrl;
		TLCDCANVABW *canva;
		TEASYKEYS *keys;
		TM24CIF *memi2c;
		TAIN *ainobj;
		TPIDPWM *motor_pid;
	
		TTINA226 *va_motor;
		TTINA226 *va_src24;
	
		SYSBIOS::Timer key_rep_timer;
		SYSBIOS::Timer sw_timer;
		EPAGE cur_page;
	
		uint32_t calc_repeate_frompushtime (uint32_t ptime);
	
		void draw_main_page_task (const S_PGMESSAGE_T &msg);
		void draw_edit_param_task (const S_PGMESSAGE_T &msg);
		void draw_list_param_task (const S_PGMESSAGE_T &msg);

		uint32_t draw_str_center (long ys, TSTMSTRING &s, bool inv);
		void add_dig_to_str (TSTMSTRING &s, MAV_PARAM_TYPE tp, const S_MDAT_T &d, uint8_t dg);

		void cursor_up ();
		void cursor_down ();
		void set_edit_mode (bool v);
		
		void param_change_updown (long p, bool finc);
		void draw_item_param (long prm_ix, long y_k, bool is_curs, bool is_edit);
		
		bool f_settings_changed;
		void draw_param_list (long y_start, long height);
		void draw_paramedit_page (long prm_ix);
		void draw_main_screens ();

		void params_aply (EPRMIX ix);
		void all_params_aply ();
		
		S_GPIOPIN *c_pin_out;
		S_GPIOPIN *c_pin_xdc_opto_en;
		
		void xdc_optocoupler_en (bool v);
		
	public:
		TCORERCT (THALLDIG *rectifier, S_GPIOPIN *sdcact, S_GPIOPIN *c_pout, TLCDCANVABW *c, TEASYKEYS *k, TM24CIF *m, TAIN *ain, TTINA226 *vc_mr, TTINA226 *vc_s, TPIDPWM *mot);
		bool is_lcd_update ();
		void set_page (EPAGE p);
};



#endif
