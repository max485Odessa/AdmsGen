#include "THALLDIG.h"
#include "rutine.h"



THALLDIG::THALLDIG (TTIM_MKS_ISR *t, TEXTINT_ISR *ei, EPWMCHNL usdch, S_GPIOPIN *c_pout, uint8_t npnt): c_points_n ((!npnt)?1:npnt)
{
	points = new S_HALPOINT_T[c_points_n];
	add_points_ix = 0;
	cb = 0;
	etim = t;
	extisr = ei;
	used_ch = usdch;
	c_pin_out = c_pout;
	etim->set_tim_cb (used_ch, this);
	etim->enable_timer_oc (used_ch, true);
	etim->enable_timer_isr (true);
}



long THALLDIG::find_point_index (uint32_t e)
{
long rv = -1, ix = 0;
while (ix < add_points_ix) {
	if (points[ix].en_name == e) {
		rv = ix;
		break;
		}
	ix++;
	}
return rv;	
}



void THALLDIG::sorted_point ()
{
if (add_points_ix > 1)
	{
	uint32_t fist_ix = 0;
	float value;
	bool f_find;
	while (fist_ix < (add_points_ix-1))
		{
		value = points[fist_ix].angl;
		uint32_t s_ix = fist_ix + 1;
		uint32_t find_ix = s_ix;
		float find_value;
		f_find = false;
		while (s_ix < add_points_ix)
			{
			find_value = points[s_ix].angl;
			if (find_value < value)
				{
				find_ix = s_ix;
				value = find_value;
				f_find = true;
				}
			s_ix++;
			}
		if (f_find)
			{
			// swap places
			S_HALPOINT_T tag = points[fist_ix];
			points[fist_ix] = points[find_ix];
			points[find_ix] = tag;
			}
		fist_ix++;
		}
	}
}



bool THALLDIG::add_replace_point (float angl, uint32_t enname)
{
	bool rv = false;
	long ix = find_point_index (enname);
	if (ix < 0) {
		if (add_points_ix < c_points_n) ix = add_points_ix++;
		}
	if (ix >= 0) 
		{
		points[ix].angl = angl;
		points[ix].en_name = enname;
		points[ix].enable = true;
		points[ix].counter = 0;
		points[ix].oc_offset = 0;
		rv = true;
		}
	return rv;
}



void THALLDIG::enable_point (uint32_t enname, bool enbl)
{
	long ix = find_point_index (enname);
	if (ix >= 0) points[ix].enable = enbl;
}



float THALLDIG::get_point (uint32_t enname)
{
	float rv = 0;
	long ix = find_point_index (enname);
	if (ix >= 0) rv = points[ix].angl;
	return rv;
}



void THALLDIG::enable (bool v)
{
	f_enable = v;
}



void THALLDIG::set_cb (IFHALLCB *c)
{
	cb = c;
}



void THALLDIG::add_acc_period (uint32_t v)
{
if (v > c_treshold_step)
	{
	acc_current_period_value = v;
	}
else
	{
	acc_current_period_value += ((prev_period_value + v)/2);
	}
}



void THALLDIG::isr_gpio_cb_isr (uint8_t isr_n, bool pinstate)
{
	// need control rpm
	int32_t cv = etim->get_timer_counter ();
	int32_t value = etim->get_delta (prev_period_value, cv);
	add_acc_period (value);
	prev_period_value = cv;
	
	c_period_quantangle = acc_current_period_value;
	c_period_quantangle = c_period_quantangle / 360.0F;
	
	recalculate_point_offsets (cv);
	
	if (add_points_ix)
		{
		search_ix = 0;
		etim->set_timer_oc_value (used_ch, points[search_ix].oc_offset);
		}
}




void THALLDIG::recalculate_point_offsets (uint32_t scnnt)
{
long ix = 0;
uint32_t val;
while (ix < add_points_ix)
	{
	val = scnnt + ((c_offset_angl + points[ix].angl) * c_period_quantangle); 
	points[ix].oc_offset = val;
	ix++;
	}
}



void THALLDIG::tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch)
{
	if (f_enable == false) return;
	if (add_points_ix && search_ix < add_points_ix)
		{
		etim->set_timer_oc_value (used_ch, points[search_ix + 1].oc_offset);
		cb->cb_ifhall (points[search_ix].en_name);
		search_ix++;
		}
}



void THALLDIG::setoffset (float angl)
{
	c_offset_angl = angl;
}



uint32_t THALLDIG::getrpm ()
{
}




