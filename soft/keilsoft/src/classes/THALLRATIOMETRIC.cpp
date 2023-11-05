#include "THALLRATIOMETRIC.h"
#include "math.h"



TLINEARHALL::TLINEARHALL (uint16_t *adrdata_sa, uint16_t *adrdata_sb, float quant_volt, float mvgaus, IFHALL *c, ESYSTIM t) : ain_quant_volt (quant_volt), sens_mlvolt_g (mvgaus)
{
	cb = c;
	etim = t;
	ain_sens_a = adrdata_sa;
	ain_sens_b = adrdata_sb;
	timisr = new ITIM_ISR (t, this);
	timisr->timer_init (10, 1000000);
	ctrlpos = EHALLCTRLPOS_SYNC;
	asintab = new float[C_ASINTAB_SIZE];
	asintab_generate ();
}



float TLINEARHALL::asingen (float sval)
{
	float rv = asin (sval) * (180.0/3.141592653589793238463);
	return rv;
}



void TLINEARHALL::asintab_generate ()
{
	uint16_t ix = 0;
	float cur_sval = 0;
	const float angl_step = 1.0F / C_ASINTAB_SIZE;
	float sinval, asin;
	while (ix < C_ASINTAB_SIZE)
		{
		sinval = cur_sval / C_ASINTAB_SIZE;
		asin = asingen (sinval);
		asintab[ix++]	 = asin;
		cur_sval += angl_step; 
		}
}



void TLINEARHALL::tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch)
{
	uint16_t raw_sa = *ain_sens_a;
	uint16_t raw_sb = *ain_sens_b;
	float angl = calc_angle (raw_sa, raw_sb);
	switch (ctrlpos)
		{
		case EHALLCTRLPOS_INTERNAL:
			{
			break;
			}
		case EHALLCTRLPOS_EXTERNAL:
			{
			break;
			}
		default: break;
		}
}



void TLINEARHALL::Task ()
{
}



///static const uint8_t arrpole[]
float TLINEARHALL::calc_angle (uint16_t raw_a, uint16_t raw_b)
{
	float rv = 0;
	uint8_t sel = 0;
	float valp_a = raw_a;
	float valp_b = raw_b;
	
	valp_a -= c_zero_gaus;
	valp_b -= c_zero_gaus;
	if (valp_a >= 0) sel |= 1;
	if (valp_b >= 0) sel |= 2;
	
	float valsin_a = valp_a;
	if (valsin_a < 0) valsin_a *= -1;
	float ampsinval_a = valsin_a / c_peack_gaus;
	
	float val_asin = asingen (ampsinval_a);
	
	switch (sel)
		{
		case ESPOLE_0_89:
			{
			rv = val_asin;
			break;
			}
		case ESPOLE_90_179:
			{
			rv = 180.0F - val_asin;
			break;
			}
		case ESPOLE_180_269:
			{
			rv = 180.0F + val_asin;
			break;
			}
		case ESPOLE_270_359:
			{
			rv = 360.0F - val_asin;
			break;
			}
		}
return rv;
}



void TLINEARHALL::setbaund (uint16_t min, uint16_t max)
{
}


void TLINEARHALL::setangle_start (float angl)
{
}



void TLINEARHALL::setangle_stop (float angl)
{
}



void TLINEARHALL::enabled (bool v)
{
}


		