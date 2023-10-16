#include "TEASYKEYS.h"
#include "rutine.h"



TEASYKEYS::TEASYKEYS (S_GPIOPIN *p) : c_pins_cnt (EJSTCPINS_ENDENUM)
{
	uint8_t ix = 0;
	pins = new S_KEYSETS_T[c_pins_cnt];
	last_pushed_mask = 0;

	while (ix < EJSTCPINS_ENDENUM) 
		{
		pins[ix].keypin = p[ix];
		_pin_low_init_in_pull (&pins[ix].keypin, 1, true);
		clear_key (pins[ix]);
		ix++;
		}
	gmsg_ix = 0;
	last_ticks = SYSBIOS::GetTickCountLong ();
}



void TEASYKEYS::clear_key (S_KEYSETS_T &kp)
{
	kp.block_time = 0;
	kp.f_block_next_msg = false;
	kp.pop_time = 0;
	kp.pushstate_cur = false;
	kp.pushstate_prev = false;
	kp.push_time = 0;
	kp.messg = EJSTMSG_NONE;
}






void TEASYKEYS::update_push_state_all ()
{
	uint8_t ix = 0;
	bool stt;
	while (ix < EJSTCPINS_ENDENUM)  
		{
		stt = update_push_state (pins[ix]);
		if (stt)
			{
			last_pushed_mask |= (1UL << ix);
			}
		else
			{
			last_pushed_mask &= (0xFFFFFFFFUL ^ (1UL << ix));
			}
		ix++;
		}
}



bool TEASYKEYS::update_push_state (S_KEYSETS_T &ps)
{
	bool rv;
	ps.pushstate_cur = !_pin_get(&ps.keypin);
	rv = ps.pushstate_cur;
	return rv;
}



uint32_t TEASYKEYS::get_pushtime_cur (EJSTCPINS p)
{
	uint32_t rv = 0;
	if (p < EJSTCPINS_ENDENUM) rv = pins[p].push_time;
	return rv;
}



uint32_t TEASYKEYS::get_pushtime_last (EJSTCPINS p)
{
	uint32_t rv = 0;
	if (p < EJSTCPINS_ENDENUM) rv = pins[p].last_push_time;
	return rv;
}



EJSTMSG TEASYKEYS::get_message (EJSTCPINS &kn)
{
	EJSTMSG rv = EJSTMSG_NONE;
	uint8_t cntkeys = EJSTCPINS_ENDENUM;
	while (cntkeys)
		{
		if (gmsg_ix >= EJSTCPINS_ENDENUM) gmsg_ix = 0;
		if (pins[gmsg_ix].messg != EJSTMSG_NONE)
			{
			kn = (EJSTCPINS)gmsg_ix;
			rv = pins[gmsg_ix].messg;
			pins[gmsg_ix].messg = EJSTMSG_NONE;
			break;
			}
		gmsg_ix++;
		cntkeys--;
		}
	return rv;
}



void TEASYKEYS::block_next_msg (EJSTCPINS p)
{
	if (p < EJSTCPINS_ENDENUM) pins[p].f_block_next_msg = true;
}



void TEASYKEYS::block_time (EJSTCPINS p, uint32_t tbl)
{
	if (p < EJSTCPINS_ENDENUM) pins[p].block_time = tbl;
}



void TEASYKEYS::Task ()
{
	uint32_t curticks = SYSBIOS::GetTickCountLong (), c_dlt, dlt;
	c_dlt = curticks - last_ticks;
	if (c_dlt)
		{
		uint8_t ix = 0;
		dlt = c_dlt;
		update_push_state_all ();
		while (ix < EJSTCPINS_ENDENUM)
			{
			subval_u32 (pins[ix].block_time, c_dlt);
			if (pins[ix].pushstate_cur != pins[ix].pushstate_prev)
				{
				EJSTMSG messg = EJSTMSG_NONE;
				if (pins[ix].pushstate_cur)
					{
					if (pins[ix].pop_time && pins[ix].pop_time < 200) messg = EJSTMSG_DBLCLICK;
					pins[ix].pop_time = 0;
					pins[ix].push_time = 1;
					}
				else
					{
					messg = EJSTMSG_CLICK;
					pins[ix].last_push_time = pins[ix].push_time;
					pins[ix].push_time = 0;
					pins[ix].pop_time = 1;
					}
				if (pins[ix].f_block_next_msg)
					{
					messg = EJSTMSG_NONE;
					pins[ix].f_block_next_msg = false;
					}
				else
					{
					if (pins[ix].block_time) messg = EJSTMSG_NONE;
					}
				pins[ix].messg = messg;
				pins[ix].pushstate_prev = pins[ix].pushstate_cur;
				}
			else
				{
				// push/pop timers update
				if (pins[ix].pushstate_cur)
					{
					addval_u32 (pins[ix].push_time, c_dlt);
					}
				else
					{
					addval_u32 (pins[ix].pop_time, c_dlt);
					}
				}

			ix++;
			}
		last_ticks = curticks;
		}
		
}



uint32_t TEASYKEYS::get_pushed_mask ()
{
	return last_pushed_mask;
}


