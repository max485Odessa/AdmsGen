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
	EPRMIX_RECT_ENABLE = 0, /* on/off всей системы преобразования */ \
	EPRMIX_MOTOR_SPEED = 1, /* номинальная скорость мотора в герцах */ \
	EPRMIX_SPEED_START = 2, /* скорость начала работы системы */ \
	EPRMIX_MOTOR_P = 3, /* настройка управления мотором */ \
	EPRMIX_MOTOR_D = 4, /* настройка управления мотором */ \
	EPRMIX_MOTOR_FREQ = 5, /* настройка управления мотором */ \
	EPRMIX_R_ANGLE_ON = 6, EPRMIX_R_ANGLE_OFF = 7, /* настройка системы коммутации */ \
	EPRMIX_MOTOR_N = 8, /* общее количество магнитов */ \
	EPRMIX_VOLT_ON = 9, /* напряжение при котором разрешается преобразование */ \
	EPRMIX_VOLT_OFF = 10, /* напряжение при котором запрещается преобразование */ \
	
  EPRMIX_ENDENUM = 11};



enum EANGLPCOD {EANGLPCOD_A = 0, EANGLPCOD_B, EANGLPCOD_C, EANGLPCOD_D, EANGLPCOD_ENDENUM};


#pragma pack (push, 1)


#pragma pack (pop)


typedef struct {
	EKEYSID key;
	EJSTMSG msg;
} S_PGMESSAGE_T;



#define C_GRAPHICS_TIMS_AMOUNT 10

// графические таймера визуального обновления величины
enum EGTIM_W_MAIN {EGTIM_W_MAIN_STATE = 0, EGTIM_W_MAIN_RPM, EGTIM_W_MAIN_SRC_320V, EGTIM_W_MAIN_DST24_V, EGTIM_W_MAIN_MOT_V, EGTIM_W_MAIN_BAT12V, EGTIM_W_MAIN_MOT_A, EGTIM_W_MAIN_DST24_A, EGTIM_W_MAIN_ENDENUM};

enum EPAGE {EPAGE_NONE = 0, EPAGE_MAIN, EPAGE_PARAM_EDIT, EPAGE_PARAM_LIST, EPAGE_ENDENUM};


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
		
		
		bool f_system_state;

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

		void params_aply ();
		S_GPIOPIN *c_pin_out;
		
	public:
		TCORERCT (THALLDIG *rectifier, S_GPIOPIN *c_pout, TLCDCANVABW *c, TEASYKEYS *k, TM24CIF *m, TAIN *ain, TTINA226 *vc_mr, TTINA226 *vc_s, TPIDPWM *mot);
		bool is_lcd_update ();
		void set_page (EPAGE p);
};



#endif
