//---------------------------------------------------------------------------

#ifndef GraphsH
#define GraphsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnCtrls.hpp>
#include <ActnMan.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include "FRAPExper.h"
#include <Dialogs.hpp>

//#define sernum 13

//---------------------------------------------------------------------------
extern int state;
extern bool showplt;

class TfmGraphs : public TForm
{
__published:	// IDE-managed Components
	TChart *chFRAPs1;
	TMainMenu *MainMenu1;
	TMenuItem *itRenew;
	TMenuItem *itResid;
	TChart *chResid;
	TPointSeries *serResid1;
	TMenuItem *itHideRes;
	TPanel *pnleft;
	TSplitter *splPlots;
	TSplitter *Splitter2;
	TRadioGroup *rgPlots;
	TMenuItem *itSave;
	TSaveDialog *dlPSave;
	TMenuItem *itWork;
	TMenuItem *itProp;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall itRenewClick(TObject *Sender);
	void __fastcall itHideResClick(TObject *Sender);
	void __fastcall rgPlotsClick(TObject *Sender);
	void __fastcall itSaveClick(TObject *Sender);
	void __fastcall itWorkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall itPropClick(TObject *Sender);
private:	// User declarations
	void __fastcall showexp (int _ind);
	void __fastcall showan (int _ind);
public:		// User declarations
	__fastcall TfmGraphs(TComponent* Owner);
	bool resid;
	int sernum;
	//FRAPExper* ExrPlot;
};
//---------------------------------------------------------------------------
extern PACKAGE TfmGraphs *fmGraphs;
//---------------------------------------------------------------------------
#endif
