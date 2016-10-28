//---------------------------------------------------------------------------

#ifndef ChooseFileH
#define ChooseFileH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfmChooseFile : public TForm
{
__published:	// IDE-managed Components
  TButton *btOK;
  TButton *btCancel;
  TLabel *Label1;
  TEdit *edflname;
  TCheckBox *chbTitles;
  void __fastcall btOKClick(TObject *Sender);
  void __fastcall btCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfmChooseFile(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmChooseFile *fmChooseFile;
//---------------------------------------------------------------------------
#endif
