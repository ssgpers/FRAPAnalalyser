object fmGraphs: TfmGraphs
  Left = 145
  Top = 218
  Caption = 'Plots'
  ClientHeight = 625
  ClientWidth = 897
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter2: TSplitter
    Left = 713
    Top = 0
    Width = 4
    Height = 625
    ExplicitHeight = 645
  end
  object pnleft: TPanel
    Left = 0
    Top = 0
    Width = 713
    Height = 625
    Align = alLeft
    TabOrder = 0
    object splPlots: TSplitter
      Left = 1
      Top = 517
      Width = 711
      Height = 4
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 537
    end
    object chResid: TChart
      Left = 1
      Top = 521
      Width = 711
      Height = 103
      BackWall.Brush.Color = clWhite
      BackWall.Brush.Style = bsClear
      Legend.TopPos = 9
      Legend.Visible = False
      Title.Text.Strings = (
        '')
      BottomAxis.Title.Caption = 'time, s'
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 0.890000000000000500
      DepthAxis.Minimum = -0.109999999999999800
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 0.890000000000000500
      DepthTopAxis.Minimum = -0.109999999999999800
      LeftAxis.Title.Caption = 'Residuals'
      View3D = False
      Align = alBottom
      Color = clWhite
      TabOrder = 0
      Anchors = [akLeft, akTop, akRight, akBottom]
      PrintMargins = (
        15
        43
        15
        43)
      object serResid1: TPointSeries
        Marks.Callout.Brush.Color = clBlack
        Marks.Visible = False
        ClickableLine = False
        Pointer.HorizSize = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.VertSize = 2
        Pointer.Visible = True
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
    object chFRAPs1: TChart
      Left = 1
      Top = 1
      Width = 711
      Height = 516
      BackWall.Brush.Color = clWhite
      BackWall.Brush.Style = bsClear
      Legend.Color = clSilver
      Legend.HorizMargin = 1
      Legend.LegendStyle = lsSeries
      Legend.ResizeChart = False
      Legend.TopPos = 61
      Title.Text.Strings = (
        '')
      Title.Visible = False
      BottomAxis.Title.Caption = 'Time, s'
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = -0.310000000000008300
      DepthAxis.Minimum = -1.310000000000007000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = -0.310000000000008300
      DepthTopAxis.Minimum = -1.310000000000007000
      LeftAxis.ExactDateTime = False
      LeftAxis.Increment = 0.100000000000000000
      LeftAxis.Title.Caption = 'FRAP recovery'
      View3D = False
      Align = alClient
      Color = clBtnHighlight
      TabOrder = 1
      PrintMargins = (
        15
        14
        15
        14)
    end
  end
  object rgPlots: TRadioGroup
    Left = 717
    Top = 0
    Width = 180
    Height = 625
    Align = alClient
    Caption = 'Available Plots'
    TabOrder = 1
    OnClick = rgPlotsClick
  end
  object MainMenu1: TMainMenu
    Left = 16
    object itRenew: TMenuItem
      Caption = 'Renew'
      Visible = False
      OnClick = itRenewClick
    end
    object itResid: TMenuItem
      Caption = 'Residuals'
      Visible = False
      object itHideRes: TMenuItem
        Caption = 'Hide residuals'
        OnClick = itHideResClick
      end
    end
    object itSave: TMenuItem
      Caption = 'Save'
      OnClick = itSaveClick
    end
    object itWork: TMenuItem
      Caption = '[Work]'
      Visible = False
      OnClick = itWorkClick
    end
    object itProp: TMenuItem
      Caption = 'Options'
      OnClick = itPropClick
    end
  end
  object dlPSave: TSaveDialog
    Title = 'Save Plot'
    Left = 120
    Top = 65528
  end
end
