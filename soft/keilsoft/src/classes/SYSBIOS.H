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


class Periodic;

enum EPERCBMODE {EPERCBMODE_STOP = 0, EPERCBMODE_PERIODIC, EPERCBMODE_REPEATE, EPERCBMODE_ENDENUM};

typedef struct {
	Periodic *obj;
	EPERCBMODE state;

	uint32_t c_period;
	uint32_t l_period;
	uint32_t repeate_counter;
} s_evntdata_cb_t;

typedef s_evntdata_cb_t* HPERIOD;

class Periodic {
		static s_evntdata_cb_t evnt_arr_per[C_SBCPP_PERIODIC];
		static uint32_t evdatacb_cnt;
		static bool check_handler (HPERIOD h);
	
	protected:
		static HPERIOD create_timer_ev (Periodic *obj);
		static void timer_start_one_shot (HPERIOD h, uint32_t period);
		static void timer_start_periodic (HPERIOD h, uint32_t period);
		static void timer_start_repeate (HPERIOD h, uint32_t period, uint32_t rep_cnt);
		static void timer_stop (HPERIOD h);
		
	public:
		virtual void periodic_cb (HPERIOD h) = 0;
		static uint32_t get_periodic_cb_cnt ();
		static s_evntdata_cb_t *get_periodic_tag (uint32_t ix);
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

