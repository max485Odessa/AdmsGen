#include "TCORE.h"
#include "hard_rut.h"

extern const unsigned char resname_ptmono8_f[];
static const uint16_t graphics_value_count_arr_amount[EPAGE_ENDENUM] = {C_GRAPHICS_TIMS_AMOUNT, EGTIM_W_MAIN_ENDENUM/*main*/, C_GRAPHICS_TIMS_AMOUNT, C_GRAPHICS_TIMS_AMOUNT};

static const char *txt_state_on = "On";
static const char *txt_state_off = "Off";
static const char *txt_rpm = "rpm:";
static const char *txt_sysstate = "sys:";
static const char *txt_SRC = "SRC:";
static const char *txt_OUT = "OUT:";
static const char *txt_BAT = "BAT:";
static const char *txt_MOT = "MOT:";
static const char *txt_V = " V";
static const char *txt_A = " A";
//static const char *txt_to_sek = "to sek.";

//static const float c_angle_step_change = 0.5;
 static S_MVPARAM_U32_T param_rectifier_enable = {"rectifier.en", MAV_PARAM_TYPE_UINT32, 0, 0, 1, 0, 1};
 static S_MVPARAM_FLOAT_T param_motor_speed = {"motor.speed", MAV_PARAM_TYPE_REAL32, 0, 0, 1, 0, 1};
 static S_MVPARAM_FLOAT_T param_speed_start = {"speed.start", MAV_PARAM_TYPE_REAL32, 0, 0, 1, 0, 1};
 static S_MVPARAM_FLOAT_T param_motor_P = {"motor.P", MAV_PARAM_TYPE_REAL32, 0, 0, 1, 0, 1};
 static S_MVPARAM_FLOAT_T param_motor_D = {"motor.D", MAV_PARAM_TYPE_REAL32, 0, 0, 1, 0, 1};
 static S_MVPARAM_U32_T param_motor_freq = {"motor.freq", MAV_PARAM_TYPE_UINT32, 0, 0, 1, 0, 1};
 static S_MVPARAM_FLOAT_T param_r_angle_on = {"angl.on", MAV_PARAM_TYPE_REAL32,  0, 0, 180, 50, 0.2F};
 static S_MVPARAM_FLOAT_T param_r_angle_off = {"angl.off", MAV_PARAM_TYPE_REAL32,  0, 0, 180, 90, 0.2F};
 static S_MVPARAM_U32_T param_magnets_cnt = {"magnets.cnt", MAV_PARAM_TYPE_UINT32, 0, 0, 1, 0, 1};
 static S_MVPARAM_FLOAT_T param_rpm_speed_start = {"hi.volt.on", MAV_PARAM_TYPE_REAL32, 1, 6, 30, 1, 1};
 static S_MVPARAM_FLOAT_T param_rpm_speed_stop = {"hi.volt.off" , MAV_PARAM_TYPE_REAL32, 0, 1, 1000, 1, 1};
 



static const S_MVPARAM_HDR_T *curlist[EPRMIX_ENDENUM] = {(S_MVPARAM_HDR_T*)&param_rectifier_enable, (S_MVPARAM_HDR_T*)&param_motor_speed, (S_MVPARAM_HDR_T*)&param_speed_start, \
(S_MVPARAM_HDR_T*)&param_motor_P, (S_MVPARAM_HDR_T*)&param_motor_D, (S_MVPARAM_HDR_T*)&param_motor_freq, \
(S_MVPARAM_HDR_T*)&param_r_angle_on, (S_MVPARAM_HDR_T*)&param_r_angle_off, (S_MVPARAM_HDR_T*)&param_magnets_cnt, (S_MVPARAM_HDR_T*)&param_rpm_speed_start, \
(S_MVPARAM_HDR_T*)&param_rpm_speed_stop};



TCORERCT::TCORERCT (THALLDIG *rectifier, S_GPIOPIN *c_pout, TLCDCANVABW *c, TEASYKEYS *k, TM24CIF *m, TAIN *ain, TTINA226 *vc_mr, TTINA226 *vc_s, TPIDPWM *mot)
{
	motor_pid = mot;
	va_motor = vc_mr;
	va_src24 = vc_s;
	ainobj = ain;
	ain_array[EAINPIN_320V] = ainobj->adr_voltage(EAINPIN_320V);
	ain_array[EAINPIN_BAT] = ainobj->adr_voltage(EAINPIN_BAT);
	ain_array[EAINPIN_SRC24V] = ainobj->adr_voltage(EAINPIN_SRC24V);
	c_pin_out = c_pout;
	_pin_low_init_out_pp ( c_pin_out, 1, EHRTGPIOSPEED_MID );
	memi2c = m;
	params = new TPARAMCONTRL (memi2c, 0, 8192, (S_MVPARAM_HDR_T**)curlist, EPRMIX_ENDENUM);
	params->load ();
	rectifier_contrl = rectifier;
	rectifier_contrl->enable (true);
	rectifier_contrl->set_phase_cb (this);
	canva = c;
	keys = k;
	f_lcd_needupdate = true;
	str_tmp.set_space (strtemporarymem, sizeof(strtemporarymem)-1);
	set_page (EPAGE_PARAM_LIST);
	params_aply ();
	
	memset (grph_tims, 0, C_GRAPHICS_TIMS_AMOUNT);
	max_gr_tims = 0;
	gr_timer = SYSBIOS::CreateCB_SYS ();
	gr_timer->Set_CB (this);
	gr_timer->Start_Periodic (1);
	
	ptxt_rpm = new TGRAPHPARAM (rectifier, MAV_PARAM_TYPE_UINT32, EHALPAPAM_RPM, 20);
	ptxt_rpm->set_prephix (txt_rpm);
	
	f_system_state = false;
	ptxt_state = new TGRAPHPARAM (&f_system_state, 10, txt_state_on, txt_state_off);
	ptxt_state->set_prephix (txt_sysstate);
	
	ptxt_v320_v = new TGRAPHPARAM (ain->adr_voltage (EAINPIN_320V), 16, 0);
	ptxt_v320_v->set_prephix (txt_SRC);
	ptxt_v320_v->set_postfix (txt_V);

	ptxt_v24_v = new TGRAPHPARAM (ain->adr_voltage (EAINPIN_SRC24V), 16, 1);
	ptxt_v24_v->set_prephix (txt_OUT);
	ptxt_v24_v->set_postfix (txt_V);
	
	ptxt_v24_a = new TGRAPHPARAM (va_src24->adr_current (), 16, 2);
	ptxt_v24_a->set_postfix (txt_A);
	
	ptxt_v12bat_v = new TGRAPHPARAM (ain->adr_voltage (EAINPIN_BAT), 16, 1);
	ptxt_v12bat_v->set_prephix (txt_BAT);
	ptxt_v12bat_v->set_postfix (txt_V);
	
	ptxt_mot_v = new TGRAPHPARAM (va_motor->adr_volt(), 16, 1);
	ptxt_mot_v->set_prephix (txt_MOT);
	ptxt_mot_v->set_postfix (txt_V);
	
	ptxt_mot_a = new TGRAPHPARAM (va_motor->adr_current (), 16, 2);
	ptxt_mot_a->set_postfix (txt_A);

	
}


// TimerCB 'gr_timer' CallBack event
void TCORERCT::timer_cb (uint32_t id)
{
	if (max_gr_tims)
		{
		uint32_t ix = 0;
		while (ix < max_gr_tims)
			{
			if (grph_tims[ix])subval_u32 (grph_tims[ix], 1);
			ix++;
			}
		}
}



void TCORERCT::cb_ifhall_phase (uint32_t ps)
{
	switch (ps)
		{
		case EANGLPCOD_A: 
		case EANGLPCOD_C:
			{
			_pin_output (c_pin_out, true);
			break;
			}
		case EANGLPCOD_D:
		case EANGLPCOD_B:
			{
			_pin_output (c_pin_out, false);
			break;
			}
		default: break;
		}
}



void TCORERCT::params_aply ()
{
	S_MDAT_T *prm;
	float s, p;
	do	{
			prm = params->get_value (EPRMIX_R_ANGLE_ON);
			if (!prm) break;
			s = prm->u.f;
		
			prm = params->get_value (EPRMIX_R_ANGLE_OFF);
			if (!prm) break;
			p = prm->u.f;
		
			s = 89.0;
			p = 90;
			rectifier_contrl->add_replace_point (s,EANGLPCOD_A);
			rectifier_contrl->add_replace_point (p,EANGLPCOD_B);
			s = 89.0+180;
			p = 90+180;
			rectifier_contrl->add_replace_point (s,EANGLPCOD_C);
			rectifier_contrl->add_replace_point (p,EANGLPCOD_D);
		
			rectifier_contrl->enable (true);
		
			prm = params->get_value (EPRMIX_RECT_ENABLE);
			if (!prm) break;
			//rectifier_contrl->enable (prm->u.u32);
			
			}	while (false);

}



bool TCORERCT::is_lcd_update ()
{
	bool rv = f_lcd_needupdate;
	f_lcd_needupdate = false;
	return rv;
}



void TCORERCT::set_page (EPAGE p)
{
	if (cur_page < EPAGE_ENDENUM)
		{
		if (p != cur_page)
			{
			max_gr_tims = graphics_value_count_arr_amount[p];// C_GRAPHICS_TIMS_AMOUNT;
			canva->Cls (0);
			sw_timer.set (0);
			cur_page = p;
			f_lcd_needupdate = true;
			}
		}
	f_is_edit_param_mode = false;
}



uint8_t TCORERCT::gui_item_param_height ()
{
	return canva->GetFontHeight () + 2;
}



uint8_t TCORERCT::border_updown_height ()
{
	return (canva->GetCanvaHeight () % gui_item_param_height ());
}



uint8_t TCORERCT::gui_dislp_item_cnt ()
{
	return canva->GetCanvaHeight () /gui_item_param_height ();
}



void TCORERCT::update_view_start ()
{
	long view_last = view_param_start_ix + gui_dislp_item_cnt () - 1;

	if (cursor_param_ix < view_param_start_ix)
		{
		view_param_start_ix = cursor_param_ix;
		}
	else
		{
		if (cursor_param_ix > view_last) 
			{
			view_param_start_ix = cursor_param_ix - gui_dislp_item_cnt () + 1;
			if (view_param_start_ix < 0) view_param_start_ix = 0;
			}
		}
}



void TCORERCT::cursor_up ()
{
	cursor_param_ix--;
	if (cursor_param_ix < 0) cursor_param_ix = 0;

}



void TCORERCT::cursor_down ()
{
	cursor_param_ix++;
	if (cursor_param_ix >= EPRMIX_ENDENUM) cursor_param_ix = EPRMIX_ENDENUM - 1;

}



void TCORERCT::set_edit_mode (bool v)
{
	f_is_edit_param_mode = v;
}



void TCORERCT::draw_item_param (long prm_ix, long y_k, bool is_curs, bool is_edit)
{
	S_BASETAGPARAM_T *prm = (S_BASETAGPARAM_T*)params->get_param_tag (prm_ix);
	if (prm) {
			uint8_t rawstr[128];
			TSTMSTRING str(rawstr, sizeof(rawstr));
			str += ' ';
			str.Add_String (prm->hdr.param_id, sizeof(prm->hdr.param_id));
			str += ": ";

			add_dig_to_str (str, prm->hdr.type, prm->value, 1);
			
			str += ' ';
			uint32_t w = canva->GetCanvaWidth ();
			uint32_t wstr = canva->GetDrawStringWidth (str.c_str());
			if (wstr < w)
				{
				uint32_t ofs_x = (w - wstr)/2;
				canva->lastX = ofs_x;
				canva->lastY = y_k;
				canva->SetInverseMode (is_curs);
				canva->PrintString (str.c_str());
				}

		}
}



void TCORERCT::draw_param_list (long y_start, long height)
{
	canva->SetFonts ((MaxFontMicro*)&resname_ptmono8_f);
	canva->Cls (0);
	update_view_start ();
	long prm_ix = cursor_param_ix;
	long start_view_pos = view_param_start_ix;
	long y_cords = y_start;
	long cnt_line = gui_dislp_item_cnt ();
	long hght_end = y_cords + height;
	while (start_view_pos < EPRMIX_ENDENUM && cnt_line)
		{
		draw_item_param (start_view_pos, y_cords, (start_view_pos == cursor_param_ix)?true:false, f_is_edit_param_mode);
		y_cords += gui_item_param_height ();
		if (y_cords >= hght_end) break;
		cnt_line--;
		start_view_pos++;
		}
	f_lcd_needupdate = true;
}



void TCORERCT::draw_list_param_task (const S_PGMESSAGE_T &msg)
{
	if (msg.msg == EJSTMSG_CLICK)
		{
		switch (msg.key)
			{
			case EKEYSID_MENU:
				{
				set_page (EPAGE_MAIN);
				break;
				}
			case EKEYSID_SELECT:
				{
				edit_param_ix = cursor_param_ix;
				set_page (EPAGE_PARAM_EDIT);
				break;
				}
			case EKEYSID_LEFT:
				{
				cursor_up ();
				break;
				}
			case EKEYSID_RIGHT:
				{
				cursor_down ();
				break;
				}
			}
		}
	else
		{
		uint32_t pushtime = keys->get_pushtime_cur (EKEYSID_SELECT);
		if (pushtime > 2000)
			{
			set_page (EPAGE_MAIN);
			keys->block_next_msg (EKEYSID_SELECT);
			keys->push_time_clear (EKEYSID_SELECT);
			}
		else
			{
			canva->SetInverseMode (0);
			draw_param_list (0, canva->GetCanvaHeight ());
			}
		}
}



uint32_t TCORERCT::draw_str_center (long ys, TSTMSTRING &s, bool inv)
{
	char *rawstr = (char*)s.c_str();
	uint32_t wstr = canva->GetDrawStringWidth (rawstr);
	VID::TMRect rct;
	if (wstr)
		{
		rct.X = 0;
		rct.Y = ys;
		rct.Height = canva->GetFontHeight ();
		rct.Width = canva->GetCanvaWidth ();
		if (inv) canva->FillRect (&rct);
		
		canva->SetInverseMode (inv);
		canva->lastX = (rct.Width - wstr) / 2;
		canva->lastY = ys;
		canva->PrintString (rawstr);
		canva->SetInverseMode (false);
		}
	return rct.Height;
}



void TCORERCT::add_dig_to_str (TSTMSTRING &s, MAV_PARAM_TYPE tp, const S_MDAT_T &d, uint8_t dg)
{
if (tp == MAV_PARAM_TYPE_UINT32)
	{
	s += (unsigned long)d.u.u32;
	}
else
	{
	s.Insert_Float ((float)d.u.f, dg);
	}
}



void TCORERCT::param_change_updown (long prm_ix, bool finc)
{
	S_BASETAGPARAM_T *tag = (S_BASETAGPARAM_T*)params->get_param_tag (prm_ix);
	if (tag)
		{
		switch (tag->hdr.type)
			{
			case MAV_PARAM_TYPE_UINT32:
				{
				long val = tag->value.u.u32;
				if (finc)
					{
					val += tag->step_val.u.u32;
					if (val > tag->max.u.u32) val = tag->max.u.u32;
					}
				else
					{
					val -= tag->step_val.u.u32;
					if (val < (long)tag->min.u.u32) val = tag->min.u.u32;
					}
				tag->value.u.u32 = val;
				break;
				}
			case MAV_PARAM_TYPE_REAL32:
				{
				float val = tag->value.u.f;
				if (finc)
					{
					val += tag->step_val.u.f;
					if (val > tag->max.u.f) val = tag->max.u.f;
					}
				else
					{
					val -= tag->step_val.u.f;
					if (val < tag->min.u.f) val = tag->min.u.f;
					}
				tag->value.u.f = val;
				break;
				}
			}
		}
}




void TCORERCT::draw_paramedit_page (long prm_ix)
{

		S_BASETAGPARAM_T *tag = (S_BASETAGPARAM_T*)params->get_param_tag (prm_ix);
		if (tag)
			{
			uint8_t rawstr[128];
			TSTMSTRING str(rawstr, sizeof(rawstr));
			str = tag->hdr.param_id;
			long yk = 0;
			// param name
			uint32_t hsz = draw_str_center (yk, str, true);
			// value
			str = "Value: ";
			if (f_is_edit_param_mode) str += '[';
			add_dig_to_str (str, tag->hdr.type, tag->value, 2);
			if (f_is_edit_param_mode) str += ']';
			yk += hsz + 10; 
			canva->PrintStringCentrRect (str.c_str(), 0, yk, canva->GetCanvaWidth (), 1);
			yk += ((hsz * 2) + 2);
			// min max
			str = "min:";
			add_dig_to_str (str, tag->hdr.type, tag->min, 0);
			str += ",max:";
			add_dig_to_str (str, tag->hdr.type, tag->max, 0);
			draw_str_center (yk, str, true);
			}

}



uint32_t TCORERCT::calc_repeate_frompushtime (uint32_t ptime)
{
	uint32_t rv;

		if (ptime <= 5000) 
			{
			rv = (5000 - ptime) / 10;
			if (rv < 200) rv = 200;
			}
		else
			{
			rv = 200;
			}

	return rv;
}



void TCORERCT::draw_edit_param_task (const S_PGMESSAGE_T &msg)
{
	if (msg.msg == EJSTMSG_CLICK)
		{
		switch (msg.key)
			{
			case EKEYSID_SELECT:
				{
				f_is_edit_param_mode = !f_is_edit_param_mode;
				break;
				}
			case EKEYSID_LEFT:
				{
				if (f_is_edit_param_mode)
					{
					param_change_updown (edit_param_ix, true);
					params_aply ();
					f_lcd_needupdate = true;
					}
				break;
				}
			case EKEYSID_RIGHT:
				{
				if (f_is_edit_param_mode)
					{
					param_change_updown (edit_param_ix, false);
					params_aply ();
					f_lcd_needupdate = true;
					}
				break;
				}
			}
		}
	uint32_t pushtime;
	if (!key_rep_timer.get())
		{
		if (f_is_edit_param_mode)
			{
			pushtime = keys->get_pushtime_cur (EKEYSID_LEFT);
			if (pushtime)
				{
				param_change_updown (edit_param_ix, true);
				key_rep_timer.set (calc_repeate_frompushtime (pushtime));
				keys->block_next_msg (EKEYSID_LEFT);
				params_aply ();
				f_lcd_needupdate = true;
				}
			pushtime = keys->get_pushtime_cur (EKEYSID_RIGHT);
			if (pushtime)
				{
				param_change_updown (edit_param_ix, false);
				key_rep_timer.set (calc_repeate_frompushtime (pushtime));
				keys->block_next_msg (EKEYSID_RIGHT);
				params_aply ();
				f_lcd_needupdate = true;
				}
			}
		}
	
		
	pushtime = keys->get_pushtime_cur (EKEYSID_SELECT);
	if (pushtime > 2000)
		{
		set_page (EPAGE_PARAM_LIST);
		keys->block_next_msg (EKEYSID_SELECT);
		keys->push_time_clear (EKEYSID_SELECT);
		params->save ();
		}
	else
		{
		canva->SetInverseMode (0);
		draw_paramedit_page (edit_param_ix);
		f_lcd_needupdate = true;
		}
}


/*
void TCORERCT::print_rpm_small (short x, short y, uint32_t rpm)
{
	static uint8_t lats_w = 0;
	uint8_t rawstr[32];
	TSTMSTRING str(rawstr, sizeof(rawstr));
	str = "rpm:";
	str += rpm;
	canva->lastX = x;
	canva->lastY = y;
	uint8_t c_w = canva->GetDrawStringWidth (str.c_str()), dlt_w = 0;
	if (lats_w > c_w) 
		{
		dlt_w = lats_w - c_w;
		}
	canva->PrintString (str.c_str());
	canva->DrawVerticalBlankCnt (dlt_w);
	lats_w = c_w;
}
*/





void TCORERCT::print_voltage_small (short x, short y, EAINCH ch)
{
	if (ch < EAINPIN_ENDENUM)
		{
		static uint8_t lats_w = 0;
		uint8_t rawstr[32];
		TSTMSTRING str(rawstr, sizeof(rawstr));
		str.Insert_Float (*ain_array[ch], 1);
		str += "v";
		canva->lastX = x;
		canva->lastY = y;
		uint8_t c_w = canva->GetCanvaWidth (), dlt_w = 0;
		if (lats_w > c_w) dlt_w = lats_w - c_w;
		canva->PrintString (str.c_str());
		canva->DrawVerticalBlankCnt (dlt_w);
		lats_w = c_w;
		}
}





void TCORERCT::draw_main_screens ()
{
	if (!f_lcd_needupdate)
		{
		if (!ww_timer.get ())
			{
			f_lcd_needupdate = true;
			ww_timer.set (300);
			}
		}
	
	if (!grph_tims[EGTIM_W_MAIN_RPM])
		{
		canva->draw_param (60, 0, ptxt_rpm, VID::EGALIGN_LEFT, 60, true);
		grph_tims[EGTIM_W_MAIN_RPM] = 500;
		f_lcd_needupdate = true;
		}
	
	if (!grph_tims[EGTIM_W_MAIN_STATE])
		{
		canva->draw_param (0, 0, ptxt_state, VID::EGALIGN_LEFT, 60, true);
		grph_tims[EGTIM_W_MAIN_STATE] = 1000;
		f_lcd_needupdate = true;
		}
		
	if (!grph_tims[EGTIM_W_MAIN_SRC_320V])
		{
		canva->draw_param (0, 12, ptxt_v320_v, VID::EGALIGN_LEFT, 70, true);
		grph_tims[EGTIM_W_MAIN_SRC_320V] = 500;
		f_lcd_needupdate = true;
		}
		
	if (!grph_tims[EGTIM_W_MAIN_DST24_V])
		{
		canva->draw_param (0, 24, ptxt_v24_v, VID::EGALIGN_LEFT, 70, true);
		grph_tims[EGTIM_W_MAIN_DST24_V] = 500;
		f_lcd_needupdate = true;
		}
		
	if (!grph_tims[EGTIM_W_MAIN_DST24_A])
		{
		canva->draw_param (80, 24, ptxt_v24_a, VID::EGALIGN_LEFT, 48, true);
		grph_tims[EGTIM_W_MAIN_DST24_A] = 500;
		f_lcd_needupdate = true;
		}
		
		
	if (!grph_tims[EGTIM_W_MAIN_BAT12V])
		{
		canva->draw_param (0, 36, ptxt_v12bat_v, VID::EGALIGN_LEFT, 70, true);
		grph_tims[EGTIM_W_MAIN_BAT12V] = 500;
		f_lcd_needupdate = true;
		}
		
	if (!grph_tims[EGTIM_W_MAIN_MOT_V])
		{
		canva->draw_param (0, 48, ptxt_mot_v, VID::EGALIGN_LEFT, 70, true);
		grph_tims[EGTIM_W_MAIN_MOT_V] = 500;
		f_lcd_needupdate = true;
		}
		
	if (!grph_tims[EGTIM_W_MAIN_MOT_A])
		{
		canva->draw_param (80, 48, ptxt_mot_a, VID::EGALIGN_LEFT, 48, true);
		grph_tims[EGTIM_W_MAIN_MOT_A] = 500;
		f_lcd_needupdate = true;
		}
		
	
}




void TCORERCT::draw_main_page_task (const S_PGMESSAGE_T &msg)
{

	if (msg.msg == EJSTMSG_CLICK)
		{
		switch (msg.key)
			{
			case EKEYSID_MENU:
				{
				set_page (EPAGE_PARAM_LIST);
				break;
				}
			case EKEYSID_SELECT:
				{
				f_is_edit_param_mode = !f_is_edit_param_mode;
				break;
				}
			case EKEYSID_LEFT:
				{
				if (f_is_edit_param_mode)
					{
					param_change_updown (edit_param_ix, true);
					params_aply ();
					f_lcd_needupdate = true;
					}
				break;
				}
			case EKEYSID_RIGHT:
				{
				if (f_is_edit_param_mode)
					{
					param_change_updown (edit_param_ix, false);
					params_aply ();
					f_lcd_needupdate = true;
					}
				break;
				}
			}
		}

		canva->SetInverseMode (0);
		draw_main_screens ();
		

}



void TCORERCT::Task ()
{
	S_PGMESSAGE_T msg;
	long key;
	msg.msg = keys->get_message (key);
	msg.key = (EKEYSID)key;
	
	switch (cur_page)
		{
		case EPAGE_NONE:
			{
			break;
			}
		case EPAGE_PARAM_LIST:
			{
			draw_list_param_task (msg);
			break;
			}
		case EPAGE_MAIN:
			{
			draw_main_page_task (msg);
			break;
			}
		case EPAGE_PARAM_EDIT:
			{
			draw_edit_param_task (msg);
			break;
			}
		}
}


