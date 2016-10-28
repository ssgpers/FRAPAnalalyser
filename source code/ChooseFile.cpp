//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChooseFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmChooseFile *fmChooseFile;
//---------------------------------------------------------------------------
__fastcall TfmChooseFile::TfmChooseFile(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmChooseFile::btOKClick(TObject*)
{
	ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfmChooseFile::btCancelClick(TObject*)
{
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
