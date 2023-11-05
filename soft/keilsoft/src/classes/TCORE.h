#ifndef _H_ADAMS_RECTIFIER_CORE_H_
#define _H_ADAMS_RECTIFIER_CORE_H_


#include "stm32f4xx_hal.h"
#include "TAIN.h"
#include "TcontRect.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "ST7565R_SPI.H"
#include "LCD132x64.h"
#include "TEASYKEYS.h"
#include "TM24Cxxx.h"
#include "STMSTRING.h"
#include "TPARAMCONTROL.h"


enum EKEYSID {EKEYSID_RIGHT = 0, EKEYSID_LEFT, EKEYSID_UP, EKEYSID_DOWN , EKEYSID_OK, EJSTCPINS_ENDENUM};
enum EPRMIX {EPRMIX_RECT_ENABLE = 0, EPRMIX_SPEED_CTRL_ENABLE = 1, EPRMIX_R_ANGLE_ON = 2, EPRMIX_R_ANGLE_OFF = 3, EPRMIX_SPEED_START = 4, EPRMIX_SPEED_STOP = 5, \
EPRMIX_M_ANGLE_ON = 6, EPRMIX_M_ANGLE_OFF = 7, EPRMIX_M_FREQ = 8, EPRMIX_M_PWM = 9, EPRMIX_ENDENUM = 10};


#pragma pack (push, 1)


#pragma pack (pop)


typedef struct {
	EKEYSID key;
	EJSTMSG msg;
} S_PGMESSAGE_T;

enum EPAGE {EPAGE_NONE = 0, EPAGE_MAIN, EPAGE_PARAM_EDIT, EPAGE_PARAM_LIST, EPAGE_ENDENUM};

class TCORERCT: public TFFC {
		uint8_t gui_item_param_height ();
		uint8_t border_updown_height ();
		uint8_t gui_dislp_item_cnt ();
	
		long edit_param_ix;
		long cursor_param_ix;
		long view_param_start_ix;
		void update_view_start ();
		bool f_is_edit_param_mode;
		bool f_lcd_needupdate;
	
	protected:
		virtual void Task () override;
		uint8_t strtemporarymem[128];
		TSTMSTRING str_tmp;
	
		TPARAMCONTRL *params;
	
		TCONTRECT *rectifier_contrl;
		TLCDCANVABW *canva;
		TEASYKEYS *keys;
		TM24CIF *memi2c;
	
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
		
	public:
		TCORERCT (TCONTRECT *rectifier, TLCDCANVABW *c, TEASYKEYS *k, TM24CIF *m);
		bool is_lcd_update ();
		void set_page (EPAGE p);
		
		
};



#endif
