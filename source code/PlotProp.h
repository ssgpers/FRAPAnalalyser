//---------------------------------------------------------------------------

#ifndef PlotPropH
#define PlotPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TfmPlotProp : public TForm
{
__published:	// IDE-managed Components
  THeaderControl *HeaderControl1;
  TToolBar *tbser1;
  TLabel *Label1;
  TCheckBox *chbser1;
  TEdit *edser1;
  TToolBar *tbser2;
  TLabel *Label2;
  TCheckBox *chbser2;
  TEdit *edser2;
  TToolBar *tbser5;
  TLabel *Label3;
  TCheckBox *chbser5;
  TEdit *edser5;
  TToolBar *tbser4;
  TLabel *Label4;
  TCheckBox *chbser4;
  TEdit *edser4;
  TToolBar *tbser3;
  TLabel *Label5;
  TCheckBox *chbser3;
  TEdit *edser3;
  TButton *btOK;
  TButton *btCancel;
  void __fastcall btOKClick(TObject *Sender);
  void __fastcall btCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfmPlotProp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmPlotProp *fmPlotProp;
//---------------------------------------------------------------------------
#endif
