#ifndef ___SYSBIOS_H_
#define ___SYSBIOS_H_

#include "config_sysbios.h"
#include "stdint.h"


#define utimer_t uint32_t


typedef struct {
	void (*lpStaticRutine)(void *param);
	void *lpThisObject;
} CALLBK_CLASS_STATIC;




namespace SYSBIOS {
	

	
#ifdef SBCPP
class Timer {
		utimer_t timer_data;
	
	public:
		Timer ();
		~Timer ();
		utimer_t get ();
		void set (utimer_t v);
};



enum EPERCBMODE {EPERCBMODE_STOP = 0, EPERCBMODE_PERIODIC, EPERCBMODE_REPEATE, EPERCBMODE_ENDENUM};



class TimerCB {
	public:
		virtual void timer_cb (uint32_t id) = 0;
};



typedef struct {
	TimerCB *obj;
	uint32_t cb_id;
	
	EPERCBMODE state;
	
	uint32_t c_period;
	uint32_t l_period;
	uint32_t repeate_counter;
	
	EPERCBMODE state_prev;
	uint32_t repeate_counter_prev;
	
} s_tagdata_cb_t;


/*
class TCBTIMBASE {
	protected:
		#ifdef C_SBCPP_PERIODIC_ISR
			static s_tagdata_cb_t isr_evdatacb_arr_per[C_SBCPP_PERIODIC_ISR];
			static uint32_t isr_evdatacb_cnt;
		#endif
	
		#ifdef C_SBCPP_PERIODIC_SYS
			static s_tagdata_cb_t sys_evdatacb_arr_per[C_SBCPP_PERIODIC_SYS];
			static uint32_t sys_evdatacb_cnt;
		#endif
	public:
		
};
*/

class TCBHEXEC {
	public:
		s_tagdata_cb_t data;
};



class TCBHANDLE: public TCBHEXEC {

	
	public:
		TCBHANDLE ();
	
		void Start_Periodic (uint32_t period);
		void Start_OneShot (uint32_t period);
		void Start_Shots (uint32_t period, uint32_t rep_cnt);
		void Stop ();
		void Continue ();
		void Restart_period ();
	
		void Set_CB (TimerCB *cb);
		void Set_ID (uint32_t id);
};



class TTMEXECUTE {
	protected:
		#ifdef C_SBCPP_PERIODIC_ISR
			static TCBHANDLE cb_obj_arr_isr[C_SBCPP_PERIODIC_ISR];
			static uint16_t evdatacb_cnt_isr;
		#endif
		#ifdef C_SBCPP_PERIODIC_SYS
			static TCBHANDLE cb_obj_arr_sys[C_SBCPP_PERIODIC_SYS];
			static uint16_t evdatacb_cnt_sys;
		#endif
			static void exec (TCBHANDLE *base, uint32_t rcnt, uint32_t dltms);
	
	public:
		#ifdef C_SBCPP_PERIODIC_ISR
			static void execute_cb_isr (uint32_t dltms);
		#endif
		#ifdef C_SBCPP_PERIODIC_SYS
			static void execute_cb_sys (uint32_t dltms);
		#endif
		
};


class TCBMANAGER: public TTMEXECUTE {

	public:
		static TCBHANDLE *create_cb_slot (bool f_isr);
};





#endif

extern  unsigned char LastDeltaTicks;
unsigned long GetTickCountLong();
unsigned short GetTickCountShort();
unsigned char GetTickCountChar();


typedef struct {
	unsigned char Status;			
	CALLBK_CLASS_STATIC ParamStart;
	unsigned long DelayTime;
	unsigned long tmpDT;
} PPTHREAD;



typedef struct {
	CALLBK_CLASS_STATIC ParamStart;
	utimer_t *TimerCallBack;
} CALLBACKTIMER;

#ifdef SBCPP
TCBHANDLE *CreateCB_ISR ();
TCBHANDLE *CreateCB_SYS ();
#endif

char AddPeriodicThread_ISR (CALLBK_CLASS_STATIC *lpParam, utimer_t DelayTime);
char AddPeriodicThread_ISR_A (void *lpThis, void *lpStaticProc, utimer_t DelayTime);
char DelPeriodicThread_ISR (CALLBK_CLASS_STATIC *lpParam);
char DelPeriodicThread_ISR_A (void *lpThis, void *lpStaticProc);
char ADD_CB_TIMER_ISR (void *lpThis, void *lpStaticProc, utimer_t *lpSTim);	// по истечении таймера 1 раз запускает указанную процедуру
char DEL_CB_TIMER_ISR (utimer_t *lpSTim);
char ADD_TIMER_ISR (utimer_t *lpSTim);
char DEL_TIMER_ISR (utimer_t *lpSTim);
char SetThreadStatus_ISR (void *lpStaticProc, char Stats);
char AddPeriodicThread (CALLBK_CLASS_STATIC *lpParam, utimer_t DelayTime);
char AddPeriodicThread_A (void *lpThis, void *lpStaticProc, utimer_t DelayTime);
char DelPeriodicThread (CALLBK_CLASS_STATIC *lpParam);
char DelPeriodicThread_A (void *lpThis, void *lpStaticProc);
char ADD_CB_TIMER (void *lpThis, void *lpStaticProc, utimer_t *lpSTim);
char DEL_CB_TIMER (utimer_t *lpSTim);
char ADD_TIMER_SYS (utimer_t *lpSTim);
char DEL_TIMER_SYS (utimer_t *lpSTim);
void Wait (utimer_t timee);
void ExecuteClassStaticProc (CALLBK_CLASS_STATIC *lpSTparam);

void EXECUTE_PERIODIC_ISR (uint32_t delt);  // нужно поместить в мс прерывания
void EXECUTE_PERIODIC_SYS ();   // нужно поместить в системный поток для периодического вызова не реже пару милисекунд

}


#endif

