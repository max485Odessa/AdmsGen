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
#include "TPARAMS.h"



enum EPRMIX {EPRMIX_PRESS_PROFILE = 0, EPRMIX_PRESS_ON_A = 1, EPRMIX_PRESS_OFF_A = 2, EPRMIX_PRESS_ON_B = 3, EPRMIX_PRESS_OFF_B = 4, EPRMIX_TIME_RELAX = 5, EPRMIX_MODE = 6, EPRMIX_CALIBR_PRESS_ZERO = 7, \
EPRMIX_CALIBR_PRESS_MAX = 8, EPRMIX_PRESS_BAR = 9, EPRMIX_ENDENUM = 10};


#pragma pack (push, 1)
typedef struct {
		float sets_angle_on;
		float sets_angle_off;
		bool f_sets_speed_control;
		float sets_speed_on_rpm;
		float sets_speed_off_rpm;
		bool f_sets_rectifier_enabled;
} S_INIDATA_T;



typedef struct {
	S_INIDATA_T data;
	uint32_t crc32;
} S_SETTINGS_T;
#pragma pack (pop)


typedef struct {
	EJSTCPINS key;
	EJSTMSG msg;
} S_PGMESSAGE_T;

enum EPAGE {EPAGE_NONE = 0, EPAGE_MAIN, EPAGE_ENDENUM};

class TCORERCT: public TFFC {
		virtual void Task () override;
		uint8_t strtemporarymem[128];
		TSTMSTRING str_tmp;
	
		IRFPARAMS *params;
	
		TCONTRECT *rectifier_contrl;
		TLCDCANVABW *canva;
		TEASYKEYS *keys;
		TM24CIF *memi2c;
	
		bool f_lcd_needupdate;
	
		SYSBIOS::Timer sw_timer;
		EPAGE cur_page;
	
		void draw_main_page_task (const S_PGMESSAGE_T &msg);
		long cursor_ix;
		bool f_is_edit_setings;
	
		S_INIDATA_T sets;
		void sets_to_def ();
		bool load_settings ();
		void save_settings ();
		bool f_settings_changed;
		
	public:
		TCORERCT (TCONTRECT *rectifier, TLCDCANVABW *c, TEASYKEYS *k, TM24CIF *m);
		bool is_lcd_update ();
		void set_page (EPAGE p);
		
		
};



#endif
