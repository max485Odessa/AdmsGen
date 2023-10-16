#include "TCORE.h"


static const float c_angle_step_change = 0.5;
const static S_CONTROL_FLOAT_T param_press_profile = {EPARAMTYPE_FLOAT, "profile", 1, 6, 3};
const static S_CONTROL_FLOAT_T param_press_on_a = {EPARAMTYPE_FLOAT, "presure_a.on", 1, 6, 3};
const static S_CONTROL_FLOAT_T param_press_on_b = {EPARAMTYPE_FLOAT, "presure_b.on", 1, 6, 3};
const static S_CONTROL_FLOAT_T param_press_off_a = {EPARAMTYPE_FLOAT, "presure_a.off", 1, 6, 3};
const static S_CONTROL_FLOAT_T param_press_off_b = {EPARAMTYPE_FLOAT, "presure_b.off", 1, 6, 3};
const static S_CONTROL_UINT32_T param_time_relax = {EPARAMTYPE_U32, "time.relax", 1000, 1000, 1000};
const static S_CONTROL_UINT32_T param_station_mode = {EPARAMTYPE_U32, "station.mode", 2, 3, 2};		// 2 - auto/manual
const static S_CONTROL_FLOAT_T param_press_clbr_zero = {EPARAMTYPE_FLOAT, "pres.calibr.zero", 0, 6, 0.5F};
const static S_CONTROL_FLOAT_T param_press_clbr_max = {EPARAMTYPE_FLOAT, "pres.calibr.max", 0, 6, 4.5F};
const static S_CONTROL_FLOAT_T param_press_bar = {EPARAMTYPE_FLOAT, "pres.bar", 0.1, 20, 12};

static const S_HDRPARAM_T *curlist[EPRMIX_ENDENUM] = {(S_HDRPARAM_T*)&param_press_profile, (S_HDRPARAM_T*)&param_press_on_a, (S_HDRPARAM_T*)&param_press_off_a, \
(S_HDRPARAM_T*)&param_press_on_b, (S_HDRPARAM_T*)&param_press_off_b, (S_HDRPARAM_T*)&param_time_relax, (S_HDRPARAM_T*)&param_station_mode, \
(S_HDRPARAM_T*)&param_press_clbr_zero, (S_HDRPARAM_T*)&param_press_clbr_max, (S_HDRPARAM_T*)&param_press_bar,\
};



TCORERCT::TCORERCT (TCONTRECT *rectifier, TLCDCANVABW *c, TEASYKEYS *k, TM24CIF *m)
{
	memi2c = m;
	params = new IRFPARAMS (memi2c, (S_HDRPARAM_T*)curlist, EPRMIX_ENDENUM);
	rectifier_contrl = rectifier;
	canva = c;
	keys = k;
	f_lcd_needupdate = true;
	cursor_ix = -1;
	str_tmp.set_space (strtemporarymem, sizeof(strtemporarymem)-1);
	if (!load_settings ())
		{
		sets_to_def ();
		save_settings ();
		}
}



void TCORERCT::sets_to_def ()
{
	sets.f_sets_rectifier_enabled = false;
	sets.f_sets_speed_control = false;
	sets.sets_angle_off = 90;
	sets.sets_angle_on = 50;
	sets.sets_speed_off_rpm = 0;
	sets.sets_speed_on_rpm = 0;
	f_settings_changed = true;
}



bool TCORERCT::load_settings ()
{
	bool rv = false;
	S_SETTINGS_T tmpdata;
	memi2c->read (0, (uint8_t*)&tmpdata, sizeof(tmpdata));
	
	uint32_t crc = CalcCRC32Data ((uint8_t*)&tmpdata, sizeof(S_INIDATA_T));
	if (tmpdata.crc32 == crc)
		{
		f_settings_changed = false;
		sets = tmpdata.data;
		rv = true;
		}
	return rv;
}



void TCORERCT::save_settings ()
{
	S_SETTINGS_T tmpdata;
	tmpdata.data = sets;
	tmpdata.crc32 = CalcCRC32Data ((uint8_t*)&tmpdata, sizeof(S_INIDATA_T));
	memi2c->write (0, (uint8_t*)&tmpdata, sizeof(tmpdata));
	f_settings_changed = false;
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
			sw_timer.set (0);
			cur_page = p;
			}
		}
}



extern const unsigned char resname_wendy16_engl_f[];
void TCORERCT::draw_main_page_task (const S_PGMESSAGE_T &msg)
{
	canva->SetFonts ((MaxFontMicro*)&resname_wendy16_engl_f);
	
}




void TCORERCT::Task ()
{
	S_PGMESSAGE_T msg;
	msg.msg= keys->get_message (msg.key);
	
	switch (cur_page)
		{
		case EPAGE_NONE:
			{
			break;
			}
		case EPAGE_MAIN:
			{
			draw_main_page_task (msg);
			break;
			}
		}
}


