//---------------------------------------------------------------------------

#ifndef commandsH
#define commandsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnCtrls.hpp>
#include <ActnMan.hpp>
#include <ActnMenus.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include "Graphs.h"
#include "ChooseFile.h"
#include "ModelParam.h"
#include "TextEdit.h"
#include <fstream.h>
#include "FRAPData.h"
#include <Dialogs.hpp>
#include <Grids.hpp>
#include "FRAPProj.h"
#include <math.h>
#include "fmPar.h"
#include <ComCtrls.hpp>
#include "FitMenu.h"
//---------------------------------------------------------------------------
int state;						//self-defined variable for checking errors
THeapStatus mem;			//memory control
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *itData;
	TMenuItem *itHelp;
	TMenuItem *itLoadExp;
	TMemo *mReport;
	TStringGrid *grData1;
	TMenuItem *itModels;
	TMenuItem *itManual;
	TMenuItem *itAbout;
	TMenuItem *itSetInd;
	TMenuItem *itNormalization;
	TMenuItem *itDoubleNorm;
	TMenuItem *itSingleNorm;
	TMenuItem *itRainerNorm;
	TMenuItem *itBackMultNorm;
	TSplitter *Splitter1;
	TMenuItem *itWork;
	TMenuItem *itOptim;
	TMenuItem *itChColNm;
	TOpenDialog *dlOpen;
	TMenuItem *itLoadFiles;
	TPanel *pnright;
	TSplitter *Splitter2;
	TMenuItem *itDoubleNormFull;
	TMenuItem *itSingleNormFull;
	TMenuItem *itSetBleach;
	TMenuItem *itDataProc;
	TMenuItem *itNewAnal;
	TMenuItem *itDelAnal;
	TSaveDialog *dlSave;
	TMenuItem *itNoNorm;
	TMenuItem *itCombFit;
	TMenuItem *itPostToUn;
	TMenuItem *itPostSigUn;
	TTreeView *trShow;
	TStatusBar *stMainWind;
	TMenuItem *itDelExp;
	TMenuItem *itAddDt;
	TMenuItem *itDelDt;
	TPanel *pnSet;
	TLabel *lbSet;
	TMenuItem *itSetBleachT;
	TPanel *pnleft;
	TChart *chFRAPs1;
	TSplitter *splLeft1;
	TComboBox *cmbPlType;
	TComboBox *cmbShowTab;
	TLabel *lbShowTab;
	TMenuItem *itSvAllPar;
	TMenuItem *itSaveDataS;
	TPopupMenu *pmSave;
	TMenuItem *ipSave;
	TMenuItem *itTest;
	TMenuItem *itState1;
	TMenuItem *itForCurrExp;
	TMenuItem *itAddAverDt;
	TLabel *Label1;
	TPopupMenu *pmParProp;
	TMenuItem *ipShIter;
	TMenuItem *ipShGood;
	TMenuItem *itBeadNorm;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall itAboutClick(TObject *Sender);
	void __fastcall itLoadExpClick(TObject *Sender);
	void __fastcall itSingExpClick(TObject *Sender);
	void __fastcall itSetIndClick(TObject *Sender);
	void __fastcall itDoubExpClick(TObject *Sender);
	void __fastcall itSoumpasisDiffClick(TObject *Sender);
	void __fastcall itEllenbergDiffClick(TObject *Sender);
	void __fastcall itDoubleNormClick(TObject *Sender);
	void __fastcall itSingleNormClick(TObject *Sender);
	void __fastcall itOptimClick(TObject *Sender);
	void __fastcall itSetTmArrClick(TObject *Sender);
	void __fastcall itTpDataClick(TObject *Sender);
	void __fastcall itOneBndClick(TObject *Sender);
	void __fastcall itTwoBndClick(TObject *Sender);
	void __fastcall itChColNmClick(TObject *Sender);
	void __fastcall itWorkMClick(TObject *Sender);
	void __fastcall itbnddiffClick(TObject *Sender);
	void __fastcall itLoadFilesClick(TObject *Sender);
	void __fastcall rgShowClick(TObject *Sender);
	void __fastcall itDoubleNormFullClick(TObject *Sender);
	void __fastcall itSingleNormFullClick(TObject *Sender);
	void __fastcall itSetBleachClick(TObject *Sender);
	void __fastcall itNewAnalClick(TObject *Sender);
	void __fastcall itDelAnalClick(TObject *Sender);
	void __fastcall itpolymmodClick(TObject *Sender);
	void __fastcall itNoNormClick(TObject *Sender);
	void __fastcall itCombFitClick(TObject *Sender);
	void __fastcall itpolymmod2Click(TObject *Sender);
	void __fastcall itSvNormDtClick(TObject *Sender);
	void __fastcall itPostToUnClick(TObject *Sender);
	void __fastcall itPostSigUnClick(TObject *Sender);
	void __fastcall trShowClick(TObject *Sender);
	void __fastcall itDelExpClick(TObject *Sender);
	void __fastcall itAddDtClick(TObject *Sender);
	void __fastcall itDelDtClick(TObject *Sender);
	void __fastcall itSetBleachTClick(TObject *Sender);
	void __fastcall cmbPlTypeChange(TObject *Sender);
	void __fastcall cmbShowTabChange(TObject *Sender);
	void __fastcall itModelsClick(TObject *Sender);
	void __fastcall trShowEdited(TObject *Sender, TTreeNode *Node, AnsiString &S);
	void __fastcall itSvAllParClick(TObject *Sender);
	void __fastcall itSaveDataSClick(TObject *Sender);
	void __fastcall ipSaveClick(TObject *Sender);
	void __fastcall itTestClick(TObject *Sender);
	void __fastcall itState1Click(TObject *Sender);
	void __fastcall itAddAverDtClick(TObject *Sender);
	void __fastcall trShowChange(TObject *Sender, TTreeNode *Node);
	void __fastcall ipShGoodClick(TObject *Sender);
	void __fastcall ipShIterClick(TObject *Sender);
	void __fastcall itBeadNormClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

	//
private:	// User declarations
	void __fastcall mssuccess(char* method);
	void __fastcall mainmenuavail(void);
	void __fastcall simdial(bool _s=true);
	void __fastcall errmes(int _n=0);
	void __fastcall fit_iter(const int _mind, const int _imax, const int _stpoint, const int _finpoint);
	void __fastcall updatestatus(void);
	bool __fastcall flstoexp(void);													//loading of files to the experiment. Returns "true" if files were loaded.

	void __fastcall ChangeWindow(void);											//change main program window
	//commands for tree update
	void __fastcall exptotree(void);												//add experiment to tree view
	void __fastcall datatotree(void);												//add data to tree view
	void __fastcall expfromtree(void);											//remove experiment from tree view
	void __fastcall datafromtree(void);											//remove data from tree view
	void __fastcall findcurnode(void);											//remove data from tree view

	//commands for plot
	void __fastcall plotrenew(void);												//renew plot
	void __fastcall plotexp(tcol _ind);											//plot selected experiment or dataset
	void __fastcall edatatoplot(CFRAPData* _fd,tcol _ind);	//add experimental data to plot
	void __fastcall plotall(void);													//plot all measured intensities in a dataset

	//commands for table
	void __fastcall updatetab();                            //update table
	void __fastcall showdata(CFRAPData* _fd);               //show data in the table
	void __fastcall showparam();						                //show parameters in the table

	//commands for export
	bool __fastcall OverWrFl(const char* _nm);							//the dialog to ask if the existing file should be overwritten

	//variables
	TTreeNodes* titems;    																	// items of the tree view
	TTreeNode* cur;                                         //pointer to the node in the tree
	int sernum;																							//number of lines on the plot

public:		// User declarations
				__fastcall TfmMain(TComponent* Owner);
	CFRAPProj* FProj;						//FRAP project - created in the main program
	CFRAPExper* FExper;					//pointer to the current experiment
	CFRAPData* FData;						//pointer to the current dataset
	int fexp;										//index of current experiment
	int fdat;										//index of current dataset
  SFitPar fitpar;							//parameters for fitting


	int fnum;

};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
