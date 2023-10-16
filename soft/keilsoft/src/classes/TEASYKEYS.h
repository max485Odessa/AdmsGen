#ifndef _H_EASYKEYS_CLASS_H_
#define _H_EASYKEYS_CLASS_H_



#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"



enum EJSTCPINS {EJSTCPINS_RIGHT = 0, EJSTCPINS_LEFT, EJSTCPINS_UP, EJSTCPINS_DOWN , EJSTCPINS_OK, EJSTCPINS_ENDENUM};
enum EJSTMSG {EJSTMSG_NONE = 0, EJSTMSG_CLICK, EJSTMSG_DBLCLICK, EJSTMSG_ENDENUM = 3};



typedef struct {
	S_GPIOPIN keypin;
	uint32_t push_time;		// pushed time
	uint32_t pop_time;		// time after pop key (for control doubleclick)
	uint32_t last_push_time;	
	uint32_t block_time;		// blocked time for messages
	bool f_block_next_msg;	// blocked next message
	bool pushstate_cur;			// current state
	bool pushstate_prev;		// prev state
	EJSTMSG messg;
} S_KEYSETS_T;



class TEASYKEYS: public TFFC {
		S_KEYSETS_T *pins;
		const uint8_t c_pins_cnt;
		uint8_t gmsg_ix;

		void clear_key (S_KEYSETS_T &kp);
		void update_push_state_all ();
		bool update_push_state (S_KEYSETS_T &ps);
	
		virtual void Task () override;
		uint32_t last_ticks;
	
		SYSBIOS::Timer relax_jstc_tim;

		uint32_t last_pushed_mask;
	
	public:
		TEASYKEYS (S_GPIOPIN *p);
	
		void block_next_msg (EJSTCPINS p);
		void block_time (EJSTCPINS p, uint32_t tbl);
	
		uint32_t get_pushed_mask ();
		uint32_t get_pushtime_cur (EJSTCPINS p);
		uint32_t get_pushtime_last (EJSTCPINS p);
		EJSTMSG get_message (EJSTCPINS &kn);	


};



#endif