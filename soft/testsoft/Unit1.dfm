object Form1: TForm1
  Left = 595
  Top = 199
  BorderStyle = bsDialog
  Caption = 'Calculator "Adams" coils'
  ClientHeight = 500
  ClientWidth = 743
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 136
    Top = 16
    Width = 164
    Height = 13
    Caption = #1048#1085#1076#1091#1082#1090#1080#1074#1085#1086#1089#1090#1100' '#1082#1072#1090#1091#1096#1082#1080' '#1074' '#1043#1077#1085#1088#1080
  end
  object Label2: TLabel
    Left = 136
    Top = 48
    Width = 88
    Height = 13
    Caption = #1063#1072#1089#1090#1086#1090#1072' '#1074' '#1075#1077#1088#1094#1072#1093
  end
  object Label3: TLabel
    Left = 136
    Top = 80
    Width = 144
    Height = 13
    Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077' '#1093#1086#1083#1086#1089#1090#1086#1075#1086' '#1093#1086#1076#1072
  end
  object Label4: TLabel
    Left = 496
    Top = 16
    Width = 153
    Height = 13
    Caption = #1057#1090#1072#1088#1090#1086#1074#1099#1081' '#1091#1075#1086#1083' '#1074#1099#1087#1088#1103#1084#1083#1077#1085#1080#1103
  end
  object Label5: TLabel
    Left = 496
    Top = 48
    Width = 148
    Height = 13
    Caption = #1057#1090#1086#1087#1086#1074#1099#1081' '#1091#1075#1086#1083' '#1074#1099#1087#1088#1103#1084#1083#1077#1085#1080#1103
  end
  object Label6: TLabel
    Left = 136
    Top = 112
    Width = 123
    Height = 13
    Caption = #1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077' '#1082#1072#1090#1091#1096#1082#1080
  end
  object Label7: TLabel
    Left = 136
    Top = 144
    Width = 145
    Height = 13
    Caption = #1053#1072#1075#1088#1091#1079#1086#1095#1085#1086#1077' '#1089#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077
  end
  object Label8: TLabel
    Left = 496
    Top = 80
    Width = 232
    Height = 13
    Caption = #1059#1088#1086#1074#1077#1085#1100' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103' '#1088#1077#1082#1091#1087#1077#1088#1072#1090#1080#1074#1085#1086#1075#1086' '#1073#1083#1086#1082#1072
  end
  object Edit1: TEdit
    Left = 8
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 0
    OnChange = Edit1Change
  end
  object Edit2: TEdit
    Left = 8
    Top = 40
    Width = 121
    Height = 21
    TabOrder = 1
    OnChange = Edit2Change
  end
  object Edit3: TEdit
    Left = 8
    Top = 72
    Width = 121
    Height = 21
    TabOrder = 2
    OnChange = Edit3Change
  end
  object Edit4: TEdit
    Left = 368
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 3
    OnChange = Edit4Change
  end
  object Edit5: TEdit
    Left = 368
    Top = 40
    Width = 121
    Height = 21
    TabOrder = 4
    OnChange = Edit5Change
  end
  object Edit6: TEdit
    Left = 8
    Top = 104
    Width = 121
    Height = 21
    TabOrder = 5
    OnChange = Edit6Change
  end
  object Edit7: TEdit
    Left = 8
    Top = 136
    Width = 121
    Height = 21
    TabOrder = 6
    OnChange = Edit7Change
  end
  object Edit8: TEdit
    Left = 368
    Top = 72
    Width = 121
    Height = 21
    Color = clMenu
    Enabled = False
    TabOrder = 7
    OnChange = Edit8Change
  end
  object Button1: TButton
    Left = 288
    Top = 176
    Width = 145
    Height = 25
    Caption = #1056#1086#1079#1088#1072#1093#1091#1074#1072#1090#1080
    TabOrder = 8
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 8
    Top = 216
    Width = 729
    Height = 273
    TabOrder = 9
  end
end
