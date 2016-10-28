//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Graphs.h"
#include "commands.h"
#include "mathappllib.h"
#include "mesg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmGraphs *fmGraphs;
//---------------------------------------------------------------------------
__fastcall TfmGraphs::TfmGraphs(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::FormCreate(TObject*){
	rgPlots->Items->Clear();
	rgPlots->Items->Add("All curves");
	rgPlots->Items->Add("FRAP ROIs");
	rgPlots->Items->Add("Reference ROIs");
	rgPlots->Items->Add("Background ROIs");
	rgPlots->Items->Add("Normalized data");
	rgPlots->Items->Add("Fitted data");
	rgPlots->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::itRenewClick(TObject*){
/*	int i;

	for (i=0;i<sernum;i++)
		delete chFRAPs1->Series[0];
	sernum=0;
	serResid1->Clear();
	switch (rgPlots->ItemIndex){
		case 0: showexp(ExrPlot->ExpData[0]->ifrap); break;
		case 1: showexp(ExrPlot->ExpData[0]->iref);  break;
		case 2: showexp(ExrPlot->ExpData[0]->ibase); break;
		case 3: showexp(ExrPlot->ExpData[0]->inorm); break;
		default: showan(rgPlots->ItemIndex-4);
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::itHideResClick(TObject*){
/*	resid= false;
	itRenewClick(this); */
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::rgPlotsClick(TObject*){
	itRenewClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::showexp (int _ind){
	int i, j;
	FRAPExpData* ExpD;
	TPointSeries* Ser;
	chResid->Visible=false;
	splPlots->Visible=false;
	itProp->Enabled=true;
	sernum=ExrPlot->sampnum;

	for (i=0;i<sernum;i++){
		Ser=new TPointSeries(this);
		Ser->Pointer->VertSize=2;
		Ser->Pointer->HorizSize=2;
		chFRAPs1->AddSeries(Ser);
	}
	for (i=0;i<sernum;i++){
		ExpD=ExrPlot->ExpData[i];
		chFRAPs1->Series[i]->Title=fmMain->rgShow->Items->Strings[i].c_str();
		if (_ind>-1)
			for (j=0;j<ExpD->point_num;j++)
				chFRAPs1->Series[i]->AddXY(ExpD->time[j],ExpD->Intens[_ind][j]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::showan (int _ind){
	FRAPAnData* AnD;
	TPointSeries* SerP;
	TLineSeries* SerL;
	int j,isavr, iserr, imodl, iresd, pnum;

	chResid->Visible=true;
	splPlots->Visible=true;
	itProp->Enabled=false;
	sernum=4;
	SerP=new TPointSeries(this);
	SerP->Pointer->VertSize=2;
	SerP->Pointer->HorizSize=2;
	chFRAPs1->AddSeries(SerP);

	for (j=0; j<3;j++){
		SerL=new TLineSeries(this);
		chFRAPs1->AddSeries(SerL);
	}
	chFRAPs1->Series[1]->Pen->Width=2;

	chFRAPs1->Series[0]->Title="Average";
	chFRAPs1->Series[1]->Title="Model";
	chFRAPs1->Series[2]->Title="Upper limit";
	chFRAPs1->Series[2]->Color=clRed;
	chFRAPs1->Series[3]->Title="Lower limit";
	AnD=ExrPlot->AnData[_ind];
	isavr=AnD->isign;
	iserr=AnD->iserr;
	imodl=AnD->imodl;
	iresd=AnD->iresd;
	pnum=AnD->point_num;

	for (j=0;j<pnum;j++){
		chFRAPs1->Series[0]->AddXY(AnD->time[j], AnD->Intens[isavr][j]);
		chFRAPs1->Series[1]->AddXY(AnD->time[j], AnD->Intens[imodl][j]);
		chFRAPs1->Series[2]->AddXY(AnD->time[j], AnD->Intens[isavr][j]+AnD->Intens[iserr][j]);
		chFRAPs1->Series[3]->AddXY(AnD->time[j], AnD->Intens[isavr][j]-AnD->Intens[iserr][j]);
		serResid1->AddXY(AnD->time[j], AnD->Intens[iresd][j]);
	}
	if (AnD->avnum==1){
		chFRAPs1->Series[2]->Visible=false;
		chFRAPs1->Series[3]->Visible=false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::itSaveClick(TObject*){
	AnsiString nm;

	dlPSave->Options.Clear();
	dlPSave->Filter = "Bitmap Files (*.bmp)|*.bmp|Windows MetaFiles (*.wmf)|*.wmf";
	dlPSave->FilterIndex = 1; //start the dialog showing all files
	dlPSave->DefaultExt="bmp";
	dlPSave->FileName=rgPlots->Items->Strings[rgPlots->ItemIndex];

	if(dlPSave->Execute()){
		nm=dlPSave->FileName;
		if	(std::ifstream(nm.c_str(), ios::in | ios::nocreate)!=NULL)
			if (!OverWrFl(nm.c_str()))
				return;
		switch(dlPSave->FilterIndex){
			case 1: chFRAPs1->SaveToBitmapFile(nm); break;
			case 2:	chFRAPs1->SaveToMetafile(nm.SubString(1,nm.Length()-4)+".wmf"); break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::itWorkClick(TObject*){
	THeapStatus mem;
	mem=GetHeapStatus();
	Application->MessageBox(AnsiString(mem.TotalAllocated).c_str(), "Memory test", MB_OK);

	//--------------------------

	/*TChartSeries* Ser;
	chFRAPs1->RemoveAllSeries();
	Ser=chFRAPs1->Series[0];
	chFRAPs1->RemoveSeries(Ser);
	delete Ser;*/
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::FormClose(TObject*, TCloseAction&){
	if (showplt)
		showplt=!showplt;
	else
		ChState(-551);																												//fail
	for (int i=0;i<sernum;i++)
		delete chFRAPs1->Series[0];
	sernum=0;
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphs::itPropClick(TObject*){
	int i;
	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->Caption="Plot Options";
	fmPr->hcPar->Sections->Items[1]->Text="Dataset";
	fmPr->hcPar->Sections->Items[2]->Text="Show";
	fmPr->SetNVPar(sernum,false,true,false);
	for (i=0; i<sernum; i++) {
		fmPr->fms[i]->lbName->Caption=fmMain->rgShow->Items->Strings[i].c_str();
		fmPr->fms[i]->chbVar->Checked=chFRAPs1->Series[i]->Visible;
	}
	if (fmPr->ShowModal()==mrOk)
		for (i=0; i<sernum; i++)
			chFRAPs1->Series[i]->Visible=fmPr->fms[i]->chbVar->Checked;
	return;
}
//---------------------------------------------------------------------------

