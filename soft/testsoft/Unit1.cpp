//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <math.h>
#include "textrut.hpp"
#include "TMWriteStream.hpp"
#include "TMReadStream.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
f_activated = false;
}
//---------------------------------------------------------------------------






bool TForm1::checking_floatval (TEdit *ed)
{
bool rv = false;
if (ed) {
    rv = CheckFloatValue (ed->Text.c_str());
    }
TColor cl = clYellow;
if (rv) {
    cl = clWhite;
    }
ed->Color = cl;
return rv;
}



bool TForm1::gui_to_data (S_FULLPARAM_T &dst)
{
bool rv = true;
    do  {
        if (!checking_floatval (Edit1)) rv = false;
        if (rv) dst.inductance = StrConvToFloat (Edit1->Text.c_str());

        if (!checking_floatval (Edit2)) rv = false;
        if (rv) dst.freq = StrConvToFloat (Edit2->Text.c_str());

        if (!checking_floatval (Edit3)) rv = false;
        if (rv) dst.voltage = StrConvToFloat (Edit3->Text.c_str());

        if (!checking_floatval (Edit6)) rv = false;
        if (rv) dst.resistance_coil = StrConvToFloat (Edit6->Text.c_str());

        if (!checking_floatval (Edit7)) rv = false;
        if (rv) dst.resistance_load = StrConvToFloat (Edit7->Text.c_str());

        if (!checking_floatval (Edit4)) rv = false;
        if (rv) dst.start_angle = StrConvToFloat (Edit4->Text.c_str());

        if (!checking_floatval (Edit5)) rv = false;
        if (rv) dst.stop_angle = StrConvToFloat (Edit5->Text.c_str());

        if (!checking_floatval (Edit8)) rv = false;
        if (rv) dst.recup_voltage = StrConvToFloat (Edit8->Text.c_str());

        } while (false);
return rv;
}



bool TForm1::data_to_gui (const S_FULLPARAM_T &dst)
{
// TDString FloatToStr_V (float DataF, unsigned char DrobSize);
bool rv = false;
    do  {
        Edit1->Text = FloatToStr_V (dst.inductance,6).c_str();
        Edit2->Text = FloatToStr_V (dst.freq,0).c_str();
        Edit3->Text = FloatToStr_V (dst.voltage,2).c_str();
        Edit6->Text = FloatToStr_V (dst.resistance_coil,2).c_str();
        Edit7->Text = FloatToStr_V (dst.resistance_load,2).c_str();
        Edit4->Text = FloatToStr_V (dst.start_angle,2).c_str();
        Edit5->Text = FloatToStr_V (dst.stop_angle,2).c_str();
        Edit8->Text = FloatToStr_V (dst.recup_voltage,2).c_str();
        rv = true;
        } while (false);
return rv;
}



bool TForm1::load ()
{
bool rv = false;
TMCreateReadStream *rd = new TMCreateReadStream ();
if (rd)
    {
    if (rd->OpenStream("sets.bin"))
        {
        if (rd->ReadData ((char*)&params, sizeof (params)) == sizeof(params)) rv = true;
        rd->CloseStream();
        }
    delete rd;
    }
return rv;
}



bool TForm1::save ()
{
bool rv = false;
TMCreateWrireStream *wr = new TMCreateWrireStream ();
if (wr) {
    if (wr->CreateStream ("sets.bin")) {
        wr->WriteBlock ((char*)&params, sizeof (params));
        wr->CloseStream();
        rv = true;
        }
    delete wr;
    }
return rv;
}



void TForm1::set_to_default ()
{
params.inductance = 0.02;
params.freq = 1000;
params.voltage = 55;
params.resistance_coil = 14.5;
params.resistance_load = 50;
params.start_angle = 87.0F;
params.stop_angle = 90.0F;
params.recup_voltage = 100;
}






void __fastcall TForm1::Edit1Change(TObject *Sender)
{
if (checking_floatval (Edit1))
    {

    }
}



//---------------------------------------------------------------------------
void __fastcall TForm1::Edit2Change(TObject *Sender)
{
if (checking_floatval (Edit2))
    {

    }
}



//---------------------------------------------------------------------------
void __fastcall TForm1::Edit3Change(TObject *Sender)
{
if (checking_floatval (Edit3))
    {

    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit6Change(TObject *Sender)
{
if (checking_floatval (Edit6))
    {

    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit7Change(TObject *Sender)
{
if (checking_floatval (Edit7))
    {

    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit4Change(TObject *Sender)
{
if (checking_floatval (Edit4))
    {

    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit5Change(TObject *Sender)
{
if (checking_floatval (Edit5))
    {

    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit8Change(TObject *Sender)
{
if (checking_floatval (Edit8))
    {

    }
}


float getsin_volt (float angl, float volt)
{
float val = sin (angl * 3.14 / 180) * volt;
return val;
}



float joule_L_calc (float ind, float currnt)
{
float e = (currnt * currnt * ind) / 2;
return e;
}



float wats_freq (float joule, float freq)
{
float w = joule * freq;
return w;
}



float time_output (float freq, float start_angl, float stop_angl)
{
float period = 1.0F / freq;
float angle_grad_quant = period / 360.0F;
return (stop_angl - start_angl) * angle_grad_quant;
}



float midle_volt (float v_peak, float angl_s, float angl_p)
{
  return (getsin_volt (angl_s, v_peak) + getsin_volt (angl_p, v_peak)) / 2;
}



float joule_load (float volt, float cur, float period_time)
{
return volt * cur * period_time;
}





//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{

if (gui_to_data (params))
    {
    String str;
    float val_f;
    float voltage_load_start = getsin_volt (params.start_angle, params.voltage);
    float voltage_load_stop = getsin_volt (params.stop_angle, params.voltage);
    float full_resistance = (params.resistance_load + params.resistance_coil);

    float full_cur_start = voltage_load_start / full_resistance;
    float full_cur_stop = voltage_load_stop / full_resistance;
    float volt_to_load_start = params.resistance_load * full_cur_start;
    float volt_to_load_stop = params.resistance_load * full_cur_stop;
    float volt_to_coil_start = params.resistance_coil * full_cur_start;
    float volt_to_coil_stop = params.resistance_coil * full_cur_stop;

    float time_load_comutation = time_output (params.freq, params.start_angle, params.stop_angle) * 2;

    float joule_to_load_start = time_load_comutation * (volt_to_load_start * full_cur_start);
    float joule_to_load_stop = time_load_comutation * (volt_to_load_stop * full_cur_stop);

    float joule_fall_coil_start = time_load_comutation * (volt_to_coil_start * full_cur_start);
    float joule_fall_coil_stop = time_load_comutation * (volt_to_coil_stop * full_cur_stop);

    float wats_load_start = joule_to_load_start * params.freq;
    float wats_load_stop = joule_to_load_stop * params.freq;
    float wats_load_midle = (wats_load_stop + wats_load_start)/2;

    float wats_coil_start = joule_fall_coil_start * params.freq;
    float wats_coil_stop = joule_fall_coil_stop * params.freq;
    float wats_coil_midle = (wats_coil_stop + wats_coil_start)/2;

    // �������� � ��������

    str = "��������� ���� ����������: V=";
    str += FloatToStr_V (volt_to_load_start, 1).c_str();
    str += ", A=";
    str += FloatToStr_V (full_cur_start, 1).c_str();//
    str += ", W=";
    str += FloatToStr_V (wats_load_start, 1).c_str();
    str += "\n\r";
    Memo1->Lines->Add (str);

    str = "�������� ���� ����������: V=";
    str += FloatToStr_V (volt_to_load_stop, 1).c_str();
    str += ", A=";
    str += FloatToStr_V (full_cur_start, 1).c_str();//
    str += ", W=";
    str += FloatToStr_V (wats_load_stop, 1).c_str();
    str += "\n\r";
    Memo1->Lines->Add (str);

    str = "����������� ��������� �� ������ ���������� �� ��������: V=";
    str += FloatToStr_V ((volt_to_load_stop + volt_to_load_start)/2, 1).c_str();
    str += ", A=";
    str += FloatToStr_V ((full_cur_stop + full_cur_start)/2, 1).c_str();//
    str += ", Wcoil=";
    str += FloatToStr_V (wats_coil_midle, 1).c_str();
    str += ", Wload=";
    str += FloatToStr_V (wats_load_midle, 1).c_str();
    str += ", Wfull=";
    str += FloatToStr_V (wats_load_midle + wats_coil_midle, 1).c_str();
    str += "\n\r";
    Memo1->Lines->Add (str);


    float joule_in_coil = (full_cur_stop * full_cur_stop * params.inductance) / 2;
    float wats_in_coil = params.freq * joule_in_coil;

    str = "���� ���: W = ";
    str += FloatToStr_V (wats_in_coil, 1).c_str();
    str += "\n\r";
    Memo1->Lines->Add (str);

    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
gui_to_data (params);
save ();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormActivate(TObject *Sender)
{
if (!f_activated)
    {
    if (!load ()) set_to_default ();
    data_to_gui (params);
    f_activated = true;
    }
}
//---------------------------------------------------------------------------

// degrees = radians * (180.0/3.141592653589793238463)
float get_asin_volt (float amp_peack, float cur_peack)
{
if (cur_peack > amp_peack) cur_peack = amp_peack;
float tosin = cur_peack / amp_peack;

float val = asin (tosin) * (180.0/3.141592653589793238463);
return val;
}


uint16_t c_zero_gaus = 2048;
uint16_t c_peack_gaus = 2047;


float asingen (float sval)
{
	float rv = asin (sval) * (180.0/3.141592653589793238463);
	return rv;
}




float calc_angle (uint16_t raw_a, uint16_t raw_b)
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




void __fastcall TForm1::Button2Click(TObject *Sender)
{
float cur_angl = 0;
float c_angl_inc = 20;
float rawval_a, rawval_b;
while (cur_angl < 360)
    {
    rawval_a = 2048 + sin (cur_angl * 3.14 / 180) * 2047;
    rawval_b = 2048 + sin ((cur_angl + 90) * 3.14 / 180) * 2047;
    float rslt = calc_angle (rawval_a, rawval_b);
    cur_angl += c_angl_inc;
    }

}
//---------------------------------------------------------------------------

