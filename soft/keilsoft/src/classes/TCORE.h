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
#include "THBRIDGE.h"


enum EKEYSID {EKEYSID_RIGHT = 0, EKEYSID_LEFT, EKEYSID_SELECT, EKEYSID_MENU , EKEYSID_ONOFF, EKEYSID_ENDENUM};
enum EPRMIX {\
	EPRMIX_AUTOSTART_ENABLE = 0,
	EPRMIX_FREQ ,
	EPRMIX_WIDTH ,
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
enum EGTIM_W_MAIN {EGTIM_W_MAIN_STATE = 0, EGTIM_VOLT_IN, EGTIM_VOLT_OUT, EGTIM_W_MAIN_ENDENUM};
enum EPAGE {EPAGE_NONE = 0, EPAGE_MAIN, EPAGE_PARAM_EDIT, EPAGE_PARAM_LIST, EPAGE_ENDENUM};
enum ECOREPDC {ECOREPDC_TIMS = 0, ECOREPDC_PROCESS, ECOREPDC_ENDENUM};

#define C_MINSRC_VOLTAGE 150.0F
#define C_MAXSRC_VOLTAGE 400.0F
#define C_MINIMAL_ANGLE_WIDEBAND 1.0F

class TCORERCT: public TFFC, public SYSBIOS::TimerCB {
		uint8_t gui_item_param_height ();
		uint8_t border_updown_height ();
		uint8_t gui_dislp_item_cnt ();
	
		long edit_param_ix;
		long cursor_param_ix;
		long view_param_start_ix;
		void update_view_start ();
		bool f_is_edit_param_mode;
		bool f_lcd_needupdate;
	
		bool f_mm_select_freq;
		uint32_t inc_dec_freq_val;
		float gen_myltvalue_fromkt (uint32_t t);
	
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


		uint32_t seting_magnets_pair;

		
	
	protected:
		virtual void Task () override;
		uint8_t strtemporarymem[128];
		TSTMSTRING str_tmp;
	

		TGRAPHPARAM *ptxt_a_in_v;	// in a
		TGRAPHPARAM *ptxt_b_in_v;	// in b
		TGRAPHPARAM *ptxt_out_v;	// out
		TGRAPHPARAM *ptxt_wrk_state;	// work

		
		bool f_pulses_state;
	
		TPARAMCONTRL *params;
	
		TLCDCANVABW *canva;
		TEASYKEYS *keys;
		TM24CIF *memi2c;
		TAIN *ainobj;
		THBRIDGE *h_bridge;
	
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
		
		//void param_change_updown (long p, bool finc);
		void param_change_updown (long p, bool finc, float multval);
		void draw_item_param (long prm_ix, long y_k, bool is_curs, bool is_edit);
		
		bool f_settings_changed;
		void draw_param_list (long y_start, long height);
		void draw_paramedit_page (long prm_ix);
		void draw_main_screens ();

		void params_aply (EPRMIX ix);
		void all_params_aply ();
		
		
	public:
		TCORERCT (TLCDCANVABW *c, TEASYKEYS *k, TM24CIF *m, TAIN *ain, THBRIDGE *hbr);
		bool is_lcd_update ();
		void set_page (EPAGE p);
};



#endif
