//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlotProp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmPlotProp *fmPlotProp;
//---------------------------------------------------------------------------
__fastcall TfmPlotProp::TfmPlotProp(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmPlotProp::btOKClick(TObject*)
{
	ModalResult=mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfmPlotProp::btCancelClick(TObject*)
{
  ModalResult=mrCancel; 
}
//---------------------------------------------------------------------------

