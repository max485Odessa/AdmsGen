#ifndef _H_MOTOR_PULSE_CLASS_H_
#define _H_MOTOR_PULSE_CLASS_H_


#include "THALLRATIOMETRIC.h"

class TPULSEMOTOR: public IFHALL {
		virtual void cb_ifhall (float angl, EHALLPOINT ps) override;
	public:
		TPULSEMOTOR ();
	
};


#endif
