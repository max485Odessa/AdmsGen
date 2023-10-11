#include "SYSBIOS.H"



namespace SYSBIOS {
	
#ifdef SBCPP
	s_evntdata_cb_t Periodic::evnt_arr_per[C_SBCPP_PERIODIC];
	uint32_t Periodic::evdatacb_cnt = 0;
#endif
	
volatile unsigned long TickCount_Timer = 0;
CALLBACKTIMER CBTIMERS_ISR[C_MAXTIMCB_ISR];
volatile long INX_CBTIMER_ISR = 0;
CALLBACKTIMER CBTIMERS[C_MAXTIMCB];
volatile long INX_CBTIMER = 0;
PPTHREAD THREADsPeriodics_ISR[C_MAXPPROC_ISR];
PPTHREAD THREADsPeriodics[C_MAXPPROC];
volatile unsigned long INX_PPROC_ISR = 0;
volatile unsigned long INX_PPROC = 0;
volatile char F_BlockPerThreCalls = 0;
utimer_t *TIMER_S[C_MAXTIMSYS];
volatile unsigned char STMR_INDX_S = 0;
utimer_t *TIMER_I[C_MAXTIMINT];
volatile unsigned char STMR_INDX_I = 0;
unsigned char LastDeltaTicks = 0;






unsigned long GetTickCountLong()
{
return TickCount_Timer;
}



unsigned short GetTickCountShort()
{
return TickCount_Timer;
}



unsigned char GetTickCountChar()
{
return TickCount_Timer;
}



unsigned long FindIndxTimer_S (utimer_t *lpSTim)
{
unsigned long rv = 0;
if (STMR_INDX_S)
	{
	unsigned long Indx = 0;
	while (Indx < STMR_INDX_S)
		{
		if (TIMER_S[Indx] == lpSTim)
			{
			rv = Indx + 1;
			break;
			}
		Indx++;
		}
	}
return rv;
}



unsigned long FindIndxTimer_ISR (utimer_t *lpSTim)
{
unsigned long rv = 0;
if (STMR_INDX_I)
	{
	unsigned long Indx = 0;
	while (Indx < STMR_INDX_I)
		{
		if (TIMER_I[Indx] == lpSTim)
			{
			rv = Indx + 1;
			break;
			}
		Indx++;
		}
	}
return rv;
}






char ADD_TIMER_SYS (utimer_t *lpSTim)
{
char rv =0;
if (STMR_INDX_S < C_MAXTIMSYS)
	{
	unsigned long Ifnd = FindIndxTimer_S (lpSTim);
	if (!Ifnd)
		{
		TIMER_S[STMR_INDX_S] = lpSTim;
		STMR_INDX_S++;
		}
	rv = 1;
	}
return rv;
}



char ADD_TIMER_ISR (utimer_t *lpSTim)
{
char rv =0;
if (STMR_INDX_I < C_MAXTIMINT)
	{
	unsigned long Ifnd = FindIndxTimer_ISR (lpSTim);
	if (!Ifnd)
		{
		TIMER_I[STMR_INDX_I] = lpSTim;
		STMR_INDX_I++;
		}
	rv = 1;
	}
return rv;
}



char DEL_TIMER_SYS (utimer_t *lpSTim)
{
char rv =0;
unsigned long Ifnd = FindIndxTimer_S (lpSTim);
if (Ifnd)
	{
	unsigned long IndSrc = STMR_INDX_S - 1;
	Ifnd--;
	unsigned long IndDst = Ifnd;
	if (IndSrc > IndDst)
		{
		unsigned long CntMov = IndSrc - IndDst;
		IndSrc = IndDst + 1;
		while (CntMov)
			{
			TIMER_S[IndDst] = TIMER_S[IndSrc];
			IndSrc++;
			IndDst++;
			CntMov--;
			}
		}
	STMR_INDX_S--;
	}
return rv;
}



char DEL_TIMER_ISR (utimer_t *lpSTim)
{
char rv =0;
unsigned long Ifnd = FindIndxTimer_ISR (lpSTim);
if (Ifnd)
	{
	unsigned long IndSrc = STMR_INDX_I - 1;
	Ifnd--;
	unsigned long IndDst = Ifnd;
	if (IndSrc > IndDst)
		{
		unsigned long CntMov = IndSrc - IndDst;
		IndSrc = IndDst + 1;
		while (CntMov)
			{
			TIMER_I[IndDst] = TIMER_I[IndSrc];
			IndSrc++;
			IndDst++;
			CntMov--;
			}
		}
	STMR_INDX_I--;
	}
return rv;
}




void TIMER_RUTINE_S (utimer_t DPTime)
{
unsigned char Indxs = 0;
utimer_t *lpLNG;
utimer_t DT;
while (Indxs < STMR_INDX_S)
		{
		lpLNG = TIMER_S[Indxs];
		DT = lpLNG[0];
		if (DT >= DPTime)
				{
				DT = DT - DPTime;
				}
		else
				{
				DT = 0;
				}
		lpLNG[0] = DT;
		Indxs++;
		}
}




static void TIMER_RUTINE_I_DLT (uint32_t dlt)
{
unsigned char Indxs = 0;
utimer_t *lpLNG;
utimer_t DT;
while (Indxs < STMR_INDX_I)
		{
		lpLNG = TIMER_I[Indxs];
		DT = lpLNG[0];
		if (DT > dlt)
			{
			DT -= dlt;
			}
		else
			{
			DT = 0;
			}
		lpLNG[0] = DT;
		Indxs++;
		}
}




unsigned long FICBP_tmp (unsigned long *MaxRegs, PPTHREAD *lTabl, CALLBK_CLASS_STATIC *lpParam)
{
unsigned long rv = 0;
if (MaxRegs[0])
	{
	unsigned long Indx = 0;
	while (Indx < MaxRegs[0])
		{
		PPTHREAD *lpCTH = (PPTHREAD*) &lTabl[Indx];
		if (lpCTH->ParamStart.lpStaticRutine == lpParam->lpStaticRutine && lpCTH->ParamStart.lpThisObject == lpParam->lpThisObject)
			{
			rv = Indx + 1;
			break;
			}
		Indx++;
		}
	}
return rv;
}



unsigned long FindIndxCallBackPeriodic_ISR (CALLBK_CLASS_STATIC *lpParam)
{
return FICBP_tmp ((unsigned long*)&INX_PPROC_ISR, THREADsPeriodics_ISR, lpParam);
}



unsigned long FindIndxCallBackPeriodic (CALLBK_CLASS_STATIC *lpParam)
{
return FICBP_tmp ((unsigned long*)&INX_PPROC, THREADsPeriodics, lpParam);
}






char AddPeriodicThread_ISR (CALLBK_CLASS_STATIC *lpParam, utimer_t DelayTime)
{
char rv = 0;
if (INX_PPROC_ISR < C_MAXPPROC_ISR && lpParam)
        {
        unsigned long IndxC = FindIndxCallBackPeriodic_ISR (lpParam);   // проверить и не добавлять если уже присутствует
        if (!IndxC)
            {
            PPTHREAD *lpCTH = (PPTHREAD*) &THREADsPeriodics_ISR[INX_PPROC_ISR];
            lpCTH->DelayTime = DelayTime;
            lpCTH->tmpDT  = 0;
            lpCTH->ParamStart.lpStaticRutine = lpParam->lpStaticRutine;
            lpCTH->ParamStart.lpThisObject = lpParam->lpThisObject;
            lpCTH->Status = 1;
            INX_PPROC_ISR++;
            }
        rv = 1;
        }
return rv;
}




char AddPeriodicThread (CALLBK_CLASS_STATIC *lpParam, utimer_t DelayTime)
{
char rv = 0;
if (INX_PPROC < C_MAXPPROC && lpParam)
        {
        unsigned long IndxC = FindIndxCallBackPeriodic (lpParam);   // проверить и не добавлять если уже присутствует
        if (!IndxC)
            {
            PPTHREAD *lpCTH = (PPTHREAD*) &THREADsPeriodics[INX_PPROC];
            lpCTH->DelayTime = DelayTime;
            lpCTH->tmpDT  = 0;
            lpCTH->ParamStart.lpStaticRutine = lpParam->lpStaticRutine;
            lpCTH->ParamStart.lpThisObject = lpParam->lpThisObject;
            lpCTH->Status = 1;
            INX_PPROC++;
            }
        rv = 1;
        }
return rv;
}



char DPTh_tmp (unsigned long *lIndxMax, unsigned long *Ifnd, PPTHREAD *ltable)
{
char rv = 0;
if (lIndxMax[0])
    {
	unsigned long IndSrc = lIndxMax[0] - 1;
	Ifnd[0]--;
	unsigned long IndDst = Ifnd[0];
	if (IndSrc > IndDst)
		{
		unsigned long CntMov = IndSrc - IndDst;
		IndSrc = IndDst + 1;
		while (CntMov)
			{
			ltable[IndDst] = ltable[IndSrc];
			IndSrc++;
			IndDst++;
			CntMov--;
			}
		}
	lIndxMax[0]--;
    rv = 1;
    }
return rv;
}




char DelPeriodicThread_ISR (CALLBK_CLASS_STATIC *lpParam)  // utimer_t *lpSTim
{
char rv =0;
unsigned long Ifnd = FindIndxCallBackPeriodic_ISR (lpParam);
if (Ifnd) rv = DPTh_tmp ((unsigned long *)&INX_PPROC_ISR, &Ifnd, THREADsPeriodics_ISR);
return rv;
}




char DelPeriodicThread (CALLBK_CLASS_STATIC *lpParam)  // utimer_t *lpSTim
{
char rv =0;
unsigned long Ifnd = FindIndxCallBackPeriodic (lpParam);
if (Ifnd) rv = DPTh_tmp ((unsigned long *)&INX_PPROC, &Ifnd, THREADsPeriodics);
return rv;
}



char DelPeriodicThread_ISR_A (void *lpThis, void *lpStaticProc)
{
CALLBK_CLASS_STATIC tmpP;
tmpP.lpStaticRutine = (void(*)(void *param))lpStaticProc;
tmpP.lpThisObject = lpThis;
return DelPeriodicThread_ISR (&tmpP);
}


char DelPeriodicThread_A (void *lpThis, void *lpStaticProc)
{
CALLBK_CLASS_STATIC tmpP;
tmpP.lpStaticRutine = (void(*)(void *param))lpStaticProc;
tmpP.lpThisObject = lpThis;
return DelPeriodicThread (&tmpP);
}





char AddPeriodicThread_ISR_A (void *lpThis, void *lpStaticProc, utimer_t DelayTime)
{
CALLBK_CLASS_STATIC tmpP;
tmpP.lpStaticRutine = (void(*)(void *param))lpStaticProc;
tmpP.lpThisObject = lpThis;
return SYSBIOS::AddPeriodicThread_ISR (&tmpP,DelayTime);
}



char AddPeriodicThread_A (void *lpThis, void *lpStaticProc, utimer_t DelayTime)
{
CALLBK_CLASS_STATIC tmpP;
tmpP.lpStaticRutine = (void(*)(void *param))lpStaticProc;
tmpP.lpThisObject = lpThis;
return SYSBIOS::AddPeriodicThread (&tmpP,DelayTime);
}



unsigned long FICBT_tmp (unsigned long *lIndxCnt, CALLBACKTIMER *lCbTm, utimer_t *lpSTim)
{
unsigned long rv = 0;
if (lIndxCnt[0])
	{
	unsigned long Indx = 0;
	CALLBACKTIMER *lpCBT;
	while (Indx < lIndxCnt[0])
		{
		lpCBT = &lCbTm[Indx];
		if (lpCBT->TimerCallBack && lpCBT->TimerCallBack == lpSTim)
			{
			rv = Indx + 1;
			break;
			}
		Indx++;
		}
	}
return rv;
}



unsigned long FindIndxCBck_Timer_ISR (utimer_t *lpSTim)
{
return FICBT_tmp ((unsigned long*)&INX_CBTIMER_ISR, CBTIMERS_ISR, lpSTim);
}



unsigned long FindIndxCBck_Timer (utimer_t *lpSTim)
{
return FICBT_tmp ((unsigned long*)&INX_CBTIMER, CBTIMERS, lpSTim);
}



char DLCBT_tmp (unsigned long *lInxCnt, unsigned long *Ifnd, CALLBACKTIMER *lTabl)
{
char rv = 0;
if (Ifnd[0])
	{
	unsigned long IndSrc = lInxCnt[0] - 1;
	Ifnd[0]--;
	unsigned long IndDst = Ifnd[0];
	if (IndSrc > IndDst)
		{
		unsigned long CntMov = IndSrc - IndDst;
		IndSrc = IndDst + 1;
		while (CntMov)
			{
			lTabl[IndDst] = lTabl[IndSrc];
			IndSrc++;
			IndDst++;
			CntMov--;
			}
		}
	lInxCnt[0]--;
    rv = true;
	}
return rv;
}



char DEL_CB_TIMER_ISR (utimer_t *lpSTim)
{
char rv =0;
unsigned long Ifnd = FindIndxCBck_Timer_ISR (lpSTim);
if (Ifnd) rv = DLCBT_tmp ((unsigned long*)&INX_CBTIMER_ISR, &Ifnd, CBTIMERS_ISR);
return rv;
}



char DEL_CB_TIMER (utimer_t *lpSTim)
{
char rv =0;
unsigned long Ifnd = FindIndxCBck_Timer (lpSTim);
if (Ifnd) rv = DLCBT_tmp ((unsigned long*)&INX_CBTIMER, &Ifnd, CBTIMERS);
return rv;
}




char ADD_CB_TIMER_ISR (void *lpThis, void *lpStaticProc, utimer_t *lpSTim)
{
char rv = 0;
if (INX_CBTIMER_ISR < C_MAXTIMCB_ISR && lpStaticProc)
        {
        unsigned long Ifnd = FindIndxCBck_Timer_ISR (lpSTim);
        if (!Ifnd)
            {
            CALLBACKTIMER *lpCBT = &CBTIMERS_ISR[INX_CBTIMER_ISR];
            lpCBT->ParamStart.lpStaticRutine = (void (*)(void *))lpStaticProc;
            lpCBT->ParamStart.lpThisObject = lpThis;
            lpCBT->TimerCallBack = lpSTim;
            INX_CBTIMER_ISR++;
            }
		rv = 1;
        }
return rv;
}




char ADD_CB_TIMER (void *lpThis, void *lpStaticProc, utimer_t *lpSTim)
{
char rv = 0;
if (INX_CBTIMER < C_MAXTIMCB && lpStaticProc)
        {
        unsigned long Ifnd = FindIndxCBck_Timer (lpSTim);
        if (!Ifnd)
            {
            CALLBACKTIMER *lpCBT = &CBTIMERS[INX_CBTIMER];
            lpCBT->ParamStart.lpStaticRutine = (void (*)(void *))lpStaticProc;
            lpCBT->ParamStart.lpThisObject = lpThis;
            lpCBT->TimerCallBack = lpSTim;
            INX_CBTIMER++;
            }
		rv = 1;
        }
return rv;
}



static void ExecutePeriodic_CBTimers_ISR_DLT (uint32_t dlt)
{
if (INX_CBTIMER_ISR)
    {
    unsigned char CINDX = 0;
    while (CINDX < INX_CBTIMER_ISR)
        {
        CALLBACKTIMER *lpCTH = &CBTIMERS_ISR[CINDX];
        if (lpCTH->TimerCallBack)
            {
            utimer_t tmr = lpCTH->TimerCallBack[0];
            if (tmr)
                {
                if (tmr > dlt)
                    {
                    tmr -= dlt;
                    }
                else
                    {
                    tmr = 0;
                    }
                lpCTH->TimerCallBack[0] = tmr;
                if (!tmr)
                    {
                    ExecuteClassStaticProc (&lpCTH->ParamStart);
                    }
                }
            }
        CINDX++;
        }
    }
}








char SetThreadStatus_ISR (void *lpStaticProc, char Stats)
{
char rv = 0;
    if (INX_PPROC_ISR)
        {
        unsigned char CINDX = 0;
        while (CINDX < INX_PPROC_ISR)
            {
            PPTHREAD *lpCTH = (PPTHREAD*)&THREADsPeriodics_ISR[CINDX];
            if ((void*)lpCTH->ParamStart.lpStaticRutine == lpStaticProc)
                {
                lpCTH->Status = Stats;
                rv = 1;
                break;
                }
            CINDX++;
            }
        }
return rv;
}



void ExecuteClassStaticProc (CALLBK_CLASS_STATIC *lpSTparam)
{
if (!F_BlockPerThreCalls)
    {
    if (lpSTparam->lpStaticRutine)
      {
      if (lpSTparam->lpThisObject)
        {
        void (*SPCBk) (void *lpObj);
        SPCBk = lpSTparam->lpStaticRutine;
        SPCBk (lpSTparam->lpThisObject);
        }
      else
        {
        void (*SPCBk) (void);
        SPCBk = (void (*)())lpSTparam->lpStaticRutine;
        SPCBk ();	
        }
      }
    }
}





void EXECUTE_PERIODIC_ISR (uint32_t delt)
{
if (INX_PPROC_ISR)
		{
		unsigned char CINDX = 0;
		while (CINDX < INX_PPROC_ISR)
				{
				PPTHREAD *lpCTH = (PPTHREAD*)&THREADsPeriodics_ISR[CINDX];
				if (lpCTH->Status == 1)
						{
                        if (lpCTH->tmpDT > delt)
                            {
                            lpCTH->tmpDT -= delt;
                            }
                        else
                            {
                            ExecuteClassStaticProc (&lpCTH->ParamStart);
                            lpCTH->tmpDT = lpCTH->DelayTime;
                            }
						}
				CINDX++;
				}
		}
ExecutePeriodic_CBTimers_ISR_DLT (delt);
TIMER_RUTINE_I_DLT (delt);
TickCount_Timer += delt;
}



static void ExecutePeriodic_CBTimers_dlt (uint32_t delt)
{
if (INX_CBTIMER)
	{
	unsigned char CINDX = 0;
	while (CINDX < INX_CBTIMER)
		{
		CALLBACKTIMER *lpCTH = &CBTIMERS[CINDX];
		if (lpCTH->TimerCallBack)
			{
			utimer_t tmr = lpCTH->TimerCallBack[0];
			if (tmr)
				{
                if (tmr > delt)
                    {
                    tmr -= delt;
                    }
                else
                    {
                    tmr = 0;
                    }
				lpCTH->TimerCallBack[0] = tmr;
				if (!tmr)
					{
					ExecuteClassStaticProc (&lpCTH->ParamStart);
					}
				}
			}
		CINDX++;
		}
	}
}



void Wait (utimer_t timee)
{
utimer_t cr_tm = GetTickCountLong(), dlt;
while (true)
	{
	dlt = GetTickCountLong() - cr_tm;
	if (dlt > timee) break;
	}
}


#ifdef SBCPP


Timer::Timer ()
{
	timer_data = 0;
	ADD_TIMER_SYS (&timer_data);
}


Timer::~Timer ()
{
	DEL_TIMER_ISR (&timer_data);
}


utimer_t Timer::get ()
{
	return timer_data;
}



void Timer::set (utimer_t v)
{
	timer_data = v;
}


HPERIOD Periodic::create_timer_ev (Periodic *obj)
{
	HPERIOD rv = 0;
if (evdatacb_cnt < C_SBCPP_PERIODIC)
	{
	evnt_arr_per[evdatacb_cnt].obj = obj;
	evnt_arr_per[evdatacb_cnt].c_period = 0;
	evnt_arr_per[evdatacb_cnt].l_period = 0;
	evnt_arr_per[evdatacb_cnt].repeate_counter = 0;
	evnt_arr_per[evdatacb_cnt].state = EPERCBMODE_STOP;
	rv = &evnt_arr_per[evdatacb_cnt];
	evdatacb_cnt++;
	}
return rv;
}



bool Periodic::check_handler (HPERIOD h)
{
	bool rv = false;
	if (h >= &evnt_arr_per[0] && h < &evnt_arr_per[evdatacb_cnt]) rv = true;
	return rv;
}


void Periodic::timer_stop (HPERIOD h)
{
	if (check_handler (h)) h->state = EPERCBMODE_STOP;
}



void Periodic::timer_start_one_shot (HPERIOD h, uint32_t period)
{
	if (check_handler (h)) 
		{
		h->c_period = period;
		h->l_period = period;
		h->repeate_counter = 1;
		h->state = EPERCBMODE_REPEATE;
		}
}



void Periodic::timer_start_periodic (HPERIOD h, uint32_t period)
{
	if (check_handler (h)) 
		{
		h->c_period = period;
		h->l_period = period;
		h->state = EPERCBMODE_PERIODIC;
		}
}



void Periodic::timer_start_repeate (HPERIOD h, uint32_t period, uint32_t rep_cnt)
{
	if (check_handler (h)) 
		{
		h->c_period = period;
		h->l_period = period;
		h->repeate_counter = rep_cnt;
		h->state = EPERCBMODE_REPEATE;
		}
}


uint32_t Periodic::get_periodic_cb_cnt ()
{
	return evdatacb_cnt;
}



s_evntdata_cb_t *Periodic::get_periodic_tag (uint32_t ix)
{
	s_evntdata_cb_t *rv = 0;
	if (ix < evdatacb_cnt) rv = &evnt_arr_per[ix];
	return rv;
}


static void PeriodicEvent_execute (uint32_t dltms)
{
uint32_t ix = 0;
uint32_t lix = Periodic::get_periodic_cb_cnt ();
s_evntdata_cb_t *tag;
while (ix < lix)
	{
	tag = Periodic::get_periodic_tag (ix);
	if (tag) 
		{
		if (tag->state != EPERCBMODE_STOP)
			{
			if (tag->c_period) 
				{
				if (tag->l_period > dltms)
					{
					tag->l_period -= dltms;
					}
				else
					{
					tag->obj->periodic_cb (tag);
					tag->l_period = tag->c_period;
					if (tag->state == EPERCBMODE_REPEATE)
						{
						if (tag->repeate_counter) tag->repeate_counter--;
						if (!tag->repeate_counter) tag->state = EPERCBMODE_STOP;
						}
					}
				}
			}
		}
	ix++;
	}
}


#endif



void EXECUTE_PERIODIC_SYS ()
{
static unsigned long LastTicks = 0;
unsigned long DltT = TickCount_Timer - LastTicks;
if (DltT)
	{
	if (INX_PPROC)
		{
		unsigned char CINDX = 0;
		while (CINDX < INX_PPROC)
			{
			PPTHREAD *lpCTH = (PPTHREAD*)&THREADsPeriodics[CINDX];
			if (lpCTH->Status == 1 && DltT)
				{
				if (DltT >= lpCTH->tmpDT)
					{
					ExecuteClassStaticProc (&lpCTH->ParamStart);
					lpCTH->tmpDT = lpCTH->DelayTime;
					}
				else
					{
					lpCTH->tmpDT -= DltT;
					}
				}
			CINDX++;
			}
		}
	LastTicks = TickCount_Timer;
	#ifdef SBCPP
		PeriodicEvent_execute (DltT);
	#endif
	TIMER_RUTINE_S (DltT);
	ExecutePeriodic_CBTimers_dlt (DltT);
	}
}





};















