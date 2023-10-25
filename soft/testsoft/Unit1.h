//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
typedef struct {
    float inductance;
    float freq;
    float voltage;
    float resistance_coil;
    float resistance_load;
    float start_angle;
    float stop_angle;
    float recup_voltage;
} S_FULLPARAM_T;


class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TEdit *Edit1;
    TLabel *Label1;
    TEdit *Edit2;
    TLabel *Label2;
    TEdit *Edit3;
    TLabel *Label3;
    TEdit *Edit4;
    TEdit *Edit5;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *Edit6;
    TLabel *Label6;
    TEdit *Edit7;
    TLabel *Label7;
    TEdit *Edit8;
    TLabel *Label8;
    TButton *Button1;
    TMemo *Memo1;
    void __fastcall Edit1Change(TObject *Sender);
    void __fastcall Edit2Change(TObject *Sender);
    void __fastcall Edit3Change(TObject *Sender);
    void __fastcall Edit6Change(TObject *Sender);
    void __fastcall Edit7Change(TObject *Sender);
    void __fastcall Edit4Change(TObject *Sender);
    void __fastcall Edit5Change(TObject *Sender);
    void __fastcall Edit8Change(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
    bool checking_floatval (TEdit *ed);
    bool gui_to_data ( S_FULLPARAM_T &dst);
    bool data_to_gui (const S_FULLPARAM_T &dst);
    S_FULLPARAM_T params;
    bool f_activated;
    bool load ();
    bool save ();
    void set_to_default ();
    
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
