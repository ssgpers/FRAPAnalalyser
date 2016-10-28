object fmChooseFile: TfmChooseFile
  Left = 459
  Top = 224
  Caption = 'Input file'
  ClientHeight = 98
  ClientWidth = 335
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 119
    Height = 13
    Caption = 'Name and path of the file'
  end
  object btOK: TButton
    Left = 248
    Top = 16
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = btOKClick
  end
  object btCancel: TButton
    Left = 248
    Top = 64
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = btCancelClick
  end
  object edflname: TEdit
    Left = 24
    Top = 40
    Width = 121
    Height = 21
    TabOrder = 2
    Text = 'samp1.txt'
  end
  object chbTitles: TCheckBox
    Left = 16
    Top = 72
    Width = 209
    Height = 17
    Caption = 'The first line contains titles of columns'
    Checked = True
    State = cbChecked
    TabOrder = 3
  end
end
