object fmMain: TfmMain
  Left = 180
  Top = 131
  Caption = 'FRAPAnalyser 2.0'
  ClientHeight = 674
  ClientWidth = 978
  Color = clBtnFace
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 674
    Top = 0
    Width = 4
    Height = 655
    ExplicitHeight = 639
  end
  object pnright: TPanel
    Left = 678
    Top = 0
    Width = 300
    Height = 655
    Align = alClient
    TabOrder = 0
    object Splitter2: TSplitter
      Left = 1
      Top = 213
      Width = 298
      Height = 4
      Cursor = crVSplit
      Align = alTop
      ExplicitWidth = 299
    end
    object mReport: TMemo
      Left = 1
      Top = 217
      Width = 298
      Height = 437
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 0
    end
    object trShow: TTreeView
      Left = 1
      Top = 83
      Width = 298
      Height = 130
      Align = alTop
      HideSelection = False
      Indent = 19
      TabOrder = 1
      OnChange = trShowChange
      OnClick = trShowClick
      OnEdited = trShowEdited
    end
    object pnSet: TPanel
      Left = 1
      Top = 1
      Width = 298
      Height = 82
      Align = alTop
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object lbSet: TLabel
        Left = 5
        Top = 0
        Width = 38
        Height = 13
        Caption = 'Settings'
      end
      object lbShowTab: TLabel
        Left = 5
        Top = 26
        Width = 64
        Height = 13
        Caption = 'Show in table'
      end
      object Label1: TLabel
        Left = 5
        Top = 48
        Width = 62
        Height = 13
        Caption = 'Show on plot'
      end
      object cmbPlType: TComboBox
        Left = 88
        Top = 45
        Width = 185
        Height = 21
        ItemHeight = 13
        TabOrder = 0
        Text = 'show on plot....'
        OnChange = cmbPlTypeChange
        Items.Strings = (
          'All measured intensities'
          'FRAP curves'
          'Reference curves'
          'Background curves'
          'Normalised curves'
          'Fitted curves'
          'Normalised and fitted curves'
          'Residuals')
      end
      object cmbShowTab: TComboBox
        Left = 88
        Top = 20
        Width = 185
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 1
        Text = 'Intensities'
        OnChange = cmbShowTabChange
        Items.Strings = (
          'Intensities'
          'Parameters values')
      end
    end
  end
  object stMainWind: TStatusBar
    Left = 0
    Top = 655
    Width = 978
    Height = 19
    Panels = <
      item
        Text = 'State'
        Width = 50
      end
      item
        Text = 'Memory'
        Width = 100
      end
      item
        Text = 'Unspecified'
        Width = 50
      end>
  end
  object pnleft: TPanel
    Left = 0
    Top = 0
    Width = 674
    Height = 655
    Align = alLeft
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvNone
    TabOrder = 2
    object splLeft1: TSplitter
      Left = 0
      Top = 421
      Width = 674
      Height = 3
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 405
    end
    object chFRAPs1: TChart
      Left = 0
      Top = 424
      Width = 674
      Height = 231
      BackWall.Brush.Color = clWhite
      BackWall.Brush.Style = bsClear
      Legend.CheckBoxes = True
      Legend.Color = clSilver
      Legend.HorizMargin = 1
      Legend.LegendStyle = lsSeries
      Legend.TopPos = 5
      Title.Text.Strings = (
        '')
      Title.Visible = False
      BottomAxis.ExactDateTime = False
      BottomAxis.Increment = 1.000000000000000000
      BottomAxis.Title.Caption = 'Time, s'
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 1.699999999999989000
      DepthAxis.Minimum = 0.699999999999990000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 1.699999999999989000
      DepthTopAxis.Minimum = 0.699999999999990000
      LeftAxis.ExactDateTime = False
      LeftAxis.Increment = 0.100000000000000000
      LeftAxis.Title.Caption = 'FRAP recovery'
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      TopAxis.ExactDateTime = False
      TopAxis.Increment = 0.100000000000000000
      View3D = False
      Align = alBottom
      Color = clBtnHighlight
      PopupMenu = pmSave
      TabOrder = 0
      PrintMargins = (
        15
        33
        15
        33)
    end
    object grData1: TStringGrid
      Left = 0
      Top = 0
      Width = 674
      Height = 421
      Align = alClient
      BevelEdges = [beLeft, beTop, beRight]
      ColCount = 2
      DefaultColWidth = 100
      DefaultRowHeight = 18
      RowCount = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
      ParentFont = False
      PopupMenu = pmParProp
      TabOrder = 1
    end
  end
  object MainMenu1: TMainMenu
    Left = 288
    object itData: TMenuItem
      Caption = 'Data'
      object itLoadExp: TMenuItem
        Caption = 'Load Experiment'
        OnClick = itLoadExpClick
      end
      object itForCurrExp: TMenuItem
        Caption = 'For current experiment'
        object itDelExp: TMenuItem
          Caption = 'Delete Experiment'
          OnClick = itDelExpClick
        end
        object itAddDt: TMenuItem
          Caption = 'Add Data'
          OnClick = itAddDtClick
        end
        object itDelDt: TMenuItem
          Caption = 'Delete Selected Data'
          OnClick = itDelDtClick
        end
        object itAddAverDt: TMenuItem
          Caption = 'Add Averaged Data'
          OnClick = itAddAverDtClick
        end
      end
      object itSetInd: TMenuItem
        Caption = 'Set ROIs'
        Enabled = False
        OnClick = itSetIndClick
      end
      object itSetBleach: TMenuItem
        Caption = 'Set Bleach Time Point'
        OnClick = itSetBleachClick
      end
      object itSetBleachT: TMenuItem
        Caption = 'Set Bleach Time'
        OnClick = itSetBleachTClick
      end
      object itChColNm: TMenuItem
        Caption = 'Change column name'
        Visible = False
        OnClick = itChColNmClick
      end
      object itLoadFiles: TMenuItem
        Caption = 'Load files'
        Visible = False
        OnClick = itLoadFilesClick
      end
      object itSvAllPar: TMenuItem
        Caption = 'Save All Parameters'
        OnClick = itSvAllParClick
      end
      object itSaveDataS: TMenuItem
        Caption = 'Save current intensity table'
        OnClick = itSaveDataSClick
      end
    end
    object itNormalization: TMenuItem
      Caption = 'Normalization'
      Enabled = False
      object itDoubleNorm: TMenuItem
        Caption = 'Double Normalization'
        OnClick = itDoubleNormClick
      end
      object itSingleNorm: TMenuItem
        Caption = 'Single Normalization'
        OnClick = itSingleNormClick
      end
      object itRainerNorm: TMenuItem
        Caption = 'Rainer Normalization'
        Enabled = False
        Visible = False
      end
      object itBackMultNorm: TMenuItem
        Caption = 'Back Multiplication Normalization'
        Enabled = False
        Visible = False
      end
      object itDoubleNormFull: TMenuItem
        Caption = 'Double Normalization+Full Scale'
        OnClick = itDoubleNormFullClick
      end
      object itSingleNormFull: TMenuItem
        Caption = 'Single Normalization+Full Scale'
        OnClick = itSingleNormFullClick
      end
      object itNoNorm: TMenuItem
        Caption = 'Data are already normalized'
        OnClick = itNoNormClick
      end
      object itPostToUn: TMenuItem
        Caption = 'PostToUn'
        Enabled = False
        OnClick = itPostToUnClick
      end
      object itBeadNorm: TMenuItem
        Caption = 'Bead normalisation (subtraction)'
        Enabled = False
        Visible = False
        OnClick = itBeadNormClick
      end
    end
    object itWork: TMenuItem
      Caption = '[Work]'
      Enabled = False
      Visible = False
      object itTest: TMenuItem
        Caption = 'Test'
        OnClick = itTestClick
      end
    end
    object itDataProc: TMenuItem
      Caption = 'Analysis'
      Enabled = False
      object itNewAnal: TMenuItem
        Caption = 'New Analysis'
        Visible = False
        OnClick = itNewAnalClick
      end
      object itDelAnal: TMenuItem
        Caption = 'Delete Analysis'
        Visible = False
        OnClick = itDelAnalClick
      end
      object itModels: TMenuItem
        Caption = 'Modelling panel'
        OnClick = itModelsClick
      end
      object itOptim: TMenuItem
        Caption = 'Fitting panel'
        OnClick = itOptimClick
      end
      object itCombFit: TMenuItem
        Caption = 'Combined Fitting'
        Visible = False
        OnClick = itCombFitClick
      end
      object itPostSigUn: TMenuItem
        Caption = 'PostSigUn'
        Visible = False
        OnClick = itPostSigUnClick
      end
    end
    object itHelp: TMenuItem
      Caption = 'Help'
      object itManual: TMenuItem
        Caption = 'Manual'
        Enabled = False
        Visible = False
      end
      object itAbout: TMenuItem
        Caption = 'About'
        OnClick = itAboutClick
      end
      object itState1: TMenuItem
        Caption = 'Clear errors'
        OnClick = itState1Click
      end
    end
  end
  object dlOpen: TOpenDialog
    InitialDir = 'D:\Data'
    Options = [ofReadOnly, ofHideReadOnly, ofAllowMultiSelect, ofFileMustExist, ofEnableSizing]
    Title = 'Input files'
    Left = 464
  end
  object dlSave: TSaveDialog
    Title = 'Save current dataset'
    Left = 368
  end
  object pmSave: TPopupMenu
    Left = 256
    Top = 416
    object ipSave: TMenuItem
      Caption = 'Save plot'
      OnClick = ipSaveClick
    end
  end
  object pmParProp: TPopupMenu
    Left = 304
    Top = 168
    object ipShIter: TMenuItem
      Caption = 'Show Iterations'
      OnClick = ipShIterClick
    end
    object ipShGood: TMenuItem
      Caption = 'Show goodness criterion'
      OnClick = ipShGoodClick
    end
  end
end
