object fmPlotProp: TfmPlotProp
  Left = 733
  Top = 355
  Caption = 'Plot properties'
  ClientHeight = 212
  ClientWidth = 210
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object HeaderControl1: THeaderControl
    Left = 0
    Top = 0
    Width = 210
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Sections = <
      item
        ImageIndex = -1
        Text = 'N'
        Width = 50
      end
      item
        ImageIndex = -1
        Text = 'Enabled'
        Width = 70
      end
      item
        ImageIndex = -1
        Text = 'Data source'
        Width = 90
      end>
    ParentFont = False
  end
  object tbser1: TToolBar
    Left = 0
    Top = 17
    Width = 210
    Height = 28
    ButtonHeight = 20
    TabOrder = 1
    DesignSize = (
      210
      28)
    object Label1: TLabel
      Left = 0
      Top = 0
      Width = 50
      Height = 20
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      Caption = '1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 0
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object chbser1: TCheckBox
      Left = 50
      Top = 0
      Width = 71
      Height = 20
      Anchors = [akTop]
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object edser1: TEdit
      Left = 121
      Top = 0
      Width = 85
      Height = 20
      ImeName = 'edser1'
      TabOrder = 1
      Text = '0'
    end
  end
  object tbser2: TToolBar
    Left = 0
    Top = 45
    Width = 210
    Height = 28
    ButtonHeight = 16
    TabOrder = 2
    DesignSize = (
      210
      28)
    object Label2: TLabel
      Left = 0
      Top = 0
      Width = 50
      Height = 16
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      Caption = '2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 0
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object chbser2: TCheckBox
      Left = 50
      Top = 0
      Width = 71
      Height = 16
      Anchors = [akTop]
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object edser2: TEdit
      Left = 121
      Top = 0
      Width = 85
      Height = 16
      ImeName = 'edser1'
      TabOrder = 1
      Text = '1'
    end
  end
  object tbser5: TToolBar
    Left = 0
    Top = 129
    Width = 210
    Height = 28
    ButtonHeight = 16
    TabOrder = 3
    DesignSize = (
      210
      28)
    object Label3: TLabel
      Left = 0
      Top = 0
      Width = 50
      Height = 16
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      Caption = '5'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 0
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object chbser5: TCheckBox
      Left = 50
      Top = 0
      Width = 71
      Height = 16
      Anchors = [akTop]
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object edser5: TEdit
      Left = 121
      Top = 0
      Width = 85
      Height = 16
      ImeName = 'edser1'
      TabOrder = 1
      Text = '4'
    end
  end
  object tbser4: TToolBar
    Left = 0
    Top = 101
    Width = 210
    Height = 28
    ButtonHeight = 16
    TabOrder = 4
    DesignSize = (
      210
      28)
    object Label4: TLabel
      Left = 0
      Top = 0
      Width = 50
      Height = 16
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      Caption = '4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 0
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object chbser4: TCheckBox
      Left = 50
      Top = 0
      Width = 71
      Height = 16
      Anchors = [akTop]
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object edser4: TEdit
      Left = 121
      Top = 0
      Width = 85
      Height = 16
      ImeName = 'edser1'
      TabOrder = 1
      Text = '3'
    end
  end
  object tbser3: TToolBar
    Left = 0
    Top = 73
    Width = 210
    Height = 28
    ButtonHeight = 16
    TabOrder = 5
    DesignSize = (
      210
      28)
    object Label5: TLabel
      Left = 0
      Top = 0
      Width = 50
      Height = 16
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      Caption = '3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 0
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object chbser3: TCheckBox
      Left = 50
      Top = 0
      Width = 71
      Height = 16
      Anchors = [akTop]
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object edser3: TEdit
      Left = 121
      Top = 0
      Width = 85
      Height = 16
      ImeName = 'edser1'
      TabOrder = 1
      Text = '2'
    end
  end
  object btOK: TButton
    Left = 24
    Top = 176
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 6
    OnClick = btOKClick
  end
  object btCancel: TButton
    Left = 112
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 7
    OnClick = btCancelClick
  end
end
