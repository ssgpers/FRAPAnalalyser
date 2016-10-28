//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <mem.h>
#include "commands.h"
#include "mathappllib.h"
//#include "Simplex_ft_FRAP.h"
//#include "Marquardt_ft.h"
#include "statappllib.h"
#include "mesg.h"
//#include "ErrAnalysis.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChooseFile"
#pragma resource "*.dfm"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
				: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormCreate(TObject*){
	FProj=new CFRAPProj();
	state=1;
	sernum=0;
	titems=trShow->Items;
	titems->Clear();
	fdat=fexp=-1;
	FData=NULL;
	FExper=NULL;
	ChangeWindow();

	mainmenuavail();
	mReport->Lines->Add("Start work from data loading");
	updatestatus();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormClose(TObject*, TCloseAction&){
	delete FProj;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itAboutClick(TObject*){
	AnsiString mes;
	mes="FRAP Analyser\n";
	mes=mes+"version 1.8\n";
	mes=mes+"Last update: March 18,2013\n";
	mes=mes+"Developed at the University of Luxembourg\n(Cytoskeleton and Cell Plasticity Laboratory,\n";
	mes=mes+"Life Sciences Research Unit, Faculty of\nScience Technology and Communication)\n";
	mes=mes+"Website: http://actinsim.uni.lu/\n";
	mes=mes+"e-mail: aliaksandr.halavatyi@uni.lu, halavatyi@gmail.com";

	Application->MessageBox(mes.c_str(), "About.....", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itTestClick(TObject*){
	//test for model function and numerical derivative
	/*FRAPMod* md;
	md=new DoubExpMod(0);
	int n=md->RetPrNum();
	mReport->Lines->Add(n);
	double* pr=new double[n];
	double* dr_num=new double[n];
	double* dr_an=new double[n];

	double t=150;
	pr[0]=0.2;
	pr[1]=0.3;
	pr[2]=0.4;
	pr[3]=200;
	pr[4]=125;
	mReport->Lines->Add(md->evaltpoint(t,pr));
	md->numderr(t,pr,dr_num);
	md->evalderr(t,pr,dr_an);

	for (int i=0; i<n; i++)
	mReport->Lines->Add(AnsiString(dr_num[i])+"\t"+AnsiString(dr_an[i]));

	delete [] pr;
	delete [] dr_num;
	delete [] dr_an;
	delete md;*/



	/*for (int i=0;i<sernum;i++)
		delete chFRAPs1->Series[0];
	sernum=0;

	TPointSeries* Ser;
	Ser=new TPointSeries(this);
	Ser->Pointer->VertSize=2;
	Ser->Pointer->HorizSize=2;

	Ser->AddXY(0,0);
	Ser->AddXY(1,1);
	Ser->AddXY(2,1);
	Ser->AddXY(3,0.5);

	chFRAPs1->AddSeries(Ser);
	sernum++;                 */


	//chFRAPs1->Visible=!chFRAPs1->Visible;

	/*FRAPMod* md;
	md=new SingExpMod(0);
	mReport->Lines->Add(md->RetPrNum());
	delete md;
	md=new DoubExpMod(0);
	mReport->Lines->Add(md->RetPrNum());
	delete md;  */


	/*dloaded();
	FExper=FProj->AddExper();
	exptotree();
	FExper->AddNewDFl("C:\\testdata\\samp10.txt");
	FData=FExper->RetD(0);
	datatotree();
	FExper->AddNewDFl("C:\\testdata\\OneBnd6.txt");
	FData=FExper->RetD(1);
	datatotree();
	FExper=FProj->AddExper();
	exptotree();
	FExper->AddNewDFl("C:\\testdata\\OneBnd4.txt");
	FData=FExper->RetD(0);
	datatotree();
	for (int i = 0; i < 10; i++) {
		FExper=FProj->AddExper();
		exptotree();
	}

	/*FData=new CExpData("C:\\testdata\\samp10.txt");
	dloaded();
	showdata(FData);
	delete FData;
	updatestatus(); */

}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itState1Click(TObject*){
	state=1;
	updatestatus();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::errmes(int _n){
	if (state<1) {
		mReport->Lines->Add("Unexpected error ("+AnsiString(state)+")");
		//state=1;
	}

	switch (_n) {
		case 1: Application->MessageBox("Error 0001. Please contact developers to recover it", "Application error!", MB_OK);return;
		case 2: Application->MessageBox("Parameters are out of the limits.\nPlease specify correct parameters", "Warning!", MB_OK);return;
		default:	Application->MessageBox("Unknown error", "Application error!", MB_OK);;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::updatetab(){
	switch (cmbShowTab->ItemIndex){
			case 0:
				if (fdat==-1){
					grData1->ColCount=-1;
					grData1->RowCount=-1;
				}
				else
					showdata(FData);
				pmParProp->AutoPopup=false;
				break;
			case 1:
				showparam();
				pmParProp->AutoPopup=true;
				break;
	}
	updatestatus();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::showdata(CFRAPData* _fd){
	int pnum=_fd->RetPtNum();
	int inum, i, j;
	tdat cdat=_fd->RetType();
	double* val;

	//bool fit=_fd->RetFt();
	inum=_fd->RetIntNum();
	grData1->ColCount=inum+2;
	grData1->RowCount=pnum+1;
	grData1->FixedCols=1;
	grData1->FixedRows=1;
	grData1->Cells[0][0]="N";
	grData1->Cells[1][0]="Time,s";
	val=_fd->RetTm();
	for (i=0;i<pnum;i++){
		grData1->Cells[0][i+1]=IntToStr(i+1);
		grData1->Cells[1][i+1]=FloatToStrF(val[i],ffGeneral,4,4);
	}
	j=_fd->RetMBl()+1;
	if (j>0)
		grData1->Cells[0][j]=grData1->Cells[0][j]+" (bleach at "+FloatToStrF(_fd->RetTBl(),ffGeneral,3,2)+" s)";

	//show intensity values
	for (int i=0; i<inum; i++){
		grData1->Cells[i+2][0]=AnsiString(_fd->RetIntNm(i));
		val=_fd->RetIntArr(i);
		for (int j=0;j<pnum;j++)
			 grData1->Cells[i+2][j+1]=FloatToStrF(val[j],ffGeneral,4,4);
	}

	if (cdat==expd){
		j=_fd->RetInd(ifrap)+2;
		if (j>1)
			grData1->Cells[j][0]=grData1->Cells[j][0]+" (frap)";
		j=_fd->RetInd(iref)+2;
		if (j>1)
			grData1->Cells[j][0]=grData1->Cells[j][0]+" (ref)";
		j=_fd->RetInd(ibase)+2;
		if (j>1)
			grData1->Cells[j][0]=grData1->Cells[j][0]+" (back)";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::showparam(){
	if (fexp<0){
		grData1->ColCount=-1;
		grData1->RowCount=-1;
		return;
	}

	FRAPMod* mdl=FExper->RetModl();
	if (mdl==NULL){
		grData1->ColCount=-1;
		grData1->RowCount=-1;
		return;
	}

	double* arr;
	int i;
	int	n=mdl->RetPrNum();
	grData1->ColCount=n+1;
	grData1->RowCount=2;
	grData1->FixedCols=1;
	grData1->FixedRows=1;

	double i_iter(0);			//index of iterations column
	double i_crit(0);     //index of criterion column

	if (ipShIter->Checked){    //reserve column for iterations
		i_iter=grData1->ColCount;
		grData1->ColCount+=1;
		grData1->Cells[i_iter][0]="Fit iterations";
	}

	if (ipShGood->Checked){    //reserve column for criterion
		i_crit=grData1->ColCount;
		grData1->ColCount+=1;
		grData1->Cells[i_crit][0]="Goodness criterion";
	}



	grData1->Cells[0][0]="Data Name";
	for (i=0; i<n; i++){
		if (strcmp(mdl->RetParUn(i),"")==0)
			grData1->Cells[i+1][0]=AnsiString(mdl->RetParLb(i));
		else
			grData1->Cells[i+1][0]=AnsiString(mdl->RetParLb(i))+", "+AnsiString(mdl->RetParUn(i));
	}
	if (fdat==-1){//show parameters values for the whole experiment
		int m=FExper->RetDtNum();
		grData1->RowCount=m+1;
		CFRAPData* fd;
		for (int j=0; j<m; j++){
			fd=FExper->RetD(j);
			grData1->Cells[0][j+1]=AnsiString(fd->RetNm());
			arr=fd->RetPPar();
			for (i=0; i<n; i++)
				grData1->Cells[i+1][j+1]=FloatToStrF(arr[i],ffGeneral,4,4);
			if (i_iter>0)
				grData1->Cells[i_iter][j+1]=IntToStr(fd->RetIter());
			if (i_crit>0)
				grData1->Cells[i_crit][j+1]=FloatToStrF(fd->RetCrit(),ffGeneral,4,4);
		}
	}
	else{//show parameters values for single dataset
		grData1->Cells[0][1]=AnsiString(FData->RetNm());
		arr=FData->RetPPar();
		for (i=0; i<n; i++)
			grData1->Cells[i+1][1]=FloatToStrF(arr[i],ffGeneral,4,4);
		if (i_iter>0)
			grData1->Cells[i_iter][1]=IntToStr(FData->RetIter());
		if (i_crit>0)
			grData1->Cells[i_crit][1]=FloatToStrF(FData->RetCrit(),ffGeneral,4,4);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::mainmenuavail(void){
	const bool data_load=(FProj->RetExpNum()>0);       //some data are loaded
	const bool datas_select=(data_load&&(fdat>=0));    // dataset is selected
	const bool tab_intens=(cmbShowTab->ItemIndex==0);  //intensities are currently shown in the data table

	pnleft->Visible=data_load;
	itForCurrExp->Enabled=data_load;
	itDelDt->Enabled=datas_select;
	itSetInd->Enabled=data_load;
	itSetBleach->Enabled=data_load;
	itSetBleachT->Enabled=data_load;
	itNormalization->Enabled=data_load;
	itDataProc->Enabled=data_load;
	itModels->Enabled=data_load;
	itSvAllPar->Enabled=data_load&&(!tab_intens);
	itSaveDataS->Enabled=datas_select&&tab_intens;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::updatestatus(void){
	TStatusPanels* pan=stMainWind->Panels;
	pan->Items[0]->Text=AnsiString(state);
	mem=GetHeapStatus();
	pan->Items[1]->Text=AnsiString(mem.TotalAllocated);
	pan->Items[2]->Text="Current experiment: ";
	if (fexp<0)
		pan->Items[2]->Text+="Not selected";
	else
		pan->Items[2]->Text+=AnsiString(FExper->RetNm());
	pan->Items[2]->Text+="; Current dataset: ";
	if (fdat<0)
		pan->Items[2]->Text+="Not selected";
	else
		pan->Items[2]->Text+=AnsiString(FData->RetNm());

}
//---------------------------------------------------------------------------
bool __fastcall TfmMain::flstoexp(void){
	int i;
	int pos;
	char cname[200];
	int fnum;
	AnsiString mes;

	dlOpen->Options << ofAllowMultiSelect << ofFileMustExist << ofHideReadOnly;
	dlOpen->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
	dlOpen->FilterIndex = 0; // start the dialog showing only text files

	if(dlOpen->Execute()){
		fnum=dlOpen->Files->Count;
		mes=AnsiString(fnum)+" files were chosen:\n";
		for (i=0; i<fnum;i++)
			mes=mes+"\n"+dlOpen->Files->Strings[i];
		if (Application->MessageBox(mes.c_str(), "Files are chosen", MB_OKCANCEL)==IDOK){
			mReport->Lines->Add("******************************");
			for (i=0; i<fnum;i++){
				StrCopy(cname,dlOpen->Files->Strings[i].c_str());
				pos=FExper->AddNewDFl(cname);
				if (fdat!=(pos-1)){ChState(-501);}				 												//fail?????????????????????
				FData=FExper->RetD(pos);
				datatotree();
				if(state==1)
					mReport->Lines->Add("File "+AnsiString(i+1)+" was loaded successfully");
				else{
					mReport->Lines->Add("Loading of file "+AnsiString(i+1)+" failed");
					Application->MessageBox("Problems with data loading", "Warning!!!", MB_OK);
					return false;
				}
				dlOpen->InitialDir=dlOpen->Files->Strings[0].c_str();
			}

			if(state==1){
				mReport->Lines->Add("Files were loaded successfully to "+AnsiString(FExper->RetNm()));
				mainmenuavail();
				trShow->Selected->Parent->Selected=true;
				itSetIndClick(this);
				itSetBleachClick(this);
			}
			else{
				mReport->Lines->Add("File loading failed.");
				return false;
			}
		}
		else
			return false;
	}
	else
		return false;
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itLoadExpClick(TObject*){
	FExper=FProj->AddExper();
	exptotree();
	if (flstoexp())
		fmMain->trShowClick(this);

	else{
		FProj->DelExper(fexp);
		expfromtree();
		mReport->Lines->Add("Experiment was not created.");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itDelExpClick(TObject*){
	//deletes selected experiment
	FProj->DelExper(fexp);
	expfromtree();
	if (FProj->RetExpNum()==0){
		mainmenuavail();
		mReport->Lines->Add("******************************");
		mReport->Lines->Add("All data were deleted");
		mReport->Lines->Add("You can load new data to continue");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itAddDtClick(TObject*){
	//appends datasets from files to the selected experiment
	if (flstoexp())
		findcurnode();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itDelDtClick(TObject*){
	//deletes selected dataset
	if (fdat==-1){
		mReport->Lines->Add("Dataset is not selected");
		return;
	}
	char expnm[30],datnm[30];
	StrLCopy(expnm,FExper->RetNm(),28);
	StrLCopy(datnm,FData->RetNm(),28);
	FExper->DelD(fdat);
	mReport->Lines->Add("Dataset \" "+AnsiString(FData->RetNm())+"\" was deleted from experiment \""+AnsiString(FExper->RetNm())+"\".");
	datafromtree();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormKeyDown(TObject *, WORD &Key,TShiftState Shift){
	if (Key==VK_DELETE)//catch pressing of "Delete" button
		if((FProj->RetExpNum()>0)&&(fdat>=0))//only if data are loaded and dataset is selected
			if (Application->MessageBox(("Do you want to delete dataset \" "+AnsiString(FData->RetNm())+"\" from experiment \""+AnsiString(FExper->RetNm())+"\"?").c_str(), "Deleting data",MB_YESNO| MB_ICONQUESTION | MB_DEFBUTTON1)==IDYES)//ask before deleting
				itDelDtClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itAddAverDtClick(TObject*){
	//evaluate averaged data
	if (fexp<0){ChState(-502);return;}							 												//fail
	int pos=FProj->RunAverE(fexp);
	FData=FExper->RetD(pos);
	datatotree();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::exptotree(void){
	if (titems->Count==0)
		cur=titems->Add(NULL,FExper->RetNm());
	else
		cur=titems->Add(trShow->Items->Item[0],FExper->RetNm());
	cur->Selected=true;
	findcurnode();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::datatotree(void){
	cur=trShow->Selected;
	if (fdat>-1)
		cur=cur->Parent;
	cur=titems->AddChild(cur,FData->RetNm());
  cur->Selected=true;
	findcurnode();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::expfromtree(void){
	TTreeNode *tmp1, *tmp2;
	if (fdat>-1)
		tmp1=cur->Parent;
	else
		tmp1=cur;
	if (fexp==0){
		if (tmp1->getNextSibling()!=NULL)
			tmp2=tmp1->getNextSibling();
		else
			tmp2=NULL;
	}
	else
		tmp2=tmp1->getPrevSibling();
	tmp1->Delete();
	trShow->Selected=tmp2;
	findcurnode();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::datafromtree(void){
	if (fdat<0){ChState(-503);return;}																			//fail
	TTreeNode* tmp;
	tmp=cur->GetPrev();
	cur->Delete();
	trShow->Selected=tmp;
	findcurnode();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::findcurnode(void){
	//return current experiment and dataset (indexes and pointers); show dataset; update menu
	if (titems->Count==0){
		fdat=fexp=-1;
		FExper=NULL;
		FData=NULL;
		return;
	}
	mainmenuavail();
	TTreeNode *par, *chl;
	chl=trShow->Selected;
	par=chl->Parent;
	if (par==NULL){
		fdat=-1;
		FData=NULL;
		par=chl;
		itDelDt->Enabled=false;
		//itSvTab->Enabled=false;
	}
	else{
		fdat=chl->Index;
		itDelDt->Enabled=true;
		//itSvTab->Enabled=true;
	}

	fexp=par->Index;
	FExper=FProj->RetE(fexp);
	if (strcmp(FExper->RetNm(),par->Text.c_str())!=0)
		ChState(-504);																												//fail

	if (fdat>-1){
		FData=FExper->RetD(fdat);
		if (strcmp(FData->RetNm(),chl->Text.c_str())!=0)ChState(-505);				//fail
	}
	cur=trShow->Selected;

	updatetab();
	if (chFRAPs1->Visible)
  	plotrenew();
	//for the testing purmoses
	//mReport->Lines->Add(AnsiString(fexp) + " ; " + AnsiString(fdat));
	//if (fdat==-1)
	//	mReport->Lines->Add(FExper->RetNm());
	//else
	//	mReport->Lines->Add(FData->RetNm());
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::trShowClick(TObject*){
	findcurnode();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::trShowChange(TObject*, TTreeNode*){
	trShowClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::trShowEdited(TObject*, TTreeNode*,AnsiString &S){
	//rename experiment
	findcurnode();
	if (fdat>-1){//if dataset is selected
		mReport->Lines->Add("Renaming of the dataset is not implemented");
		S=AnsiString(FData->RetNm());
	}
	else{//if experiment is selected
		S.SetLength(LenIntNm);
		FExper->SetNm(S.c_str());
	}
	cur->Text=S;
	findcurnode();

}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSvAllParClick(TObject*){
	//save parameters of all experiments in one file.
	//Only in cese all experiments are analysed with the same model.
	//Last column will be the name of experiment.
	dlSave->Options.Clear();
	dlSave->Title="Save parameters values";
	dlSave->Filter = "Text files (*.txt)|*.txt";
	dlSave->FilterIndex=0; // start the dialog showing text files
	dlSave->DefaultExt="txt";
	dlSave->FileName=FData->RetNm();

	if(dlSave->Execute()){
		if	(std::ifstream(dlSave->FileName.c_str(), ios::in | ios::nocreate)!=NULL)
			if (!OverWrFl(dlSave->FileName.c_str())){
				mReport->Lines->Add("File was not created.");
				return;
			}
		FProj->SvPar(dlSave->FileName.c_str(),ipShIter->Checked,ipShGood->Checked);
		if (state==1)
			mReport->Lines->Add("Parameters were saved successfully");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSaveDataSClick(TObject*){

	if ((fdat<0)||(cmbShowTab->ItemIndex!=0)){//this should newer happen, The menu option should be unavailable if dataset is not selected or intensities are not visualised
		ChState(-506);																												//fail
		mReport->Lines->Add("Dataset is not selected");
		return;
	}

	dlSave->Options.Clear();
	dlSave->Filter = "Text files (*.txt)|*.txt";
	dlSave->FilterIndex=0; // start the dialog showing text files
	dlSave->DefaultExt="txt";
	dlSave->FileName=FData->RetNm();

	if(dlSave->Execute()){
		if	(std::ifstream(dlSave->FileName.c_str(), ios::in | ios::nocreate)!=NULL)
			if (!OverWrFl(dlSave->FileName.c_str())){
				mReport->Lines->Add("File was not created.");
				return;
			}
		if (FProj->SvDataS(dlSave->FileName.c_str(),fexp,fdat))
			mReport->Lines->Add("Data were saved successfully");
		else
			mReport->Lines->Add("Data storage failed");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSetIndClick(TObject*){
//	if (fdat==-1){
//		mReport->Lines->Add("Select dataset to continue");
//		return;
//	}
//	if (FData->RetType()!=expd){
//		mReport->Lines->Add("You have to select experimental dataset for this operation");
//		return;
//	}

	CFRAPData* Fd;
	if (fdat==-1)
		Fd=FExper->RetD(0);
	else
		Fd=FData;

	int fr,ref, back;
	int i, j;
	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->SetNLPar(3);
	fmPr->Caption="Set ROIs";
	fmPr->hcPar->Sections->Items[2]->Width=0;
	fmPr->hcPar->Sections->Items[4]->Width=0;
	fmPr->fls[0]->cmbMeth->Items->Clear();
	fmPr->fls[1]->cmbMeth->Items->Clear();
	fmPr->fls[2]->cmbMeth->Items->Clear();
	for (int j=0; j<3; j++)
		for(i=0;i<Fd->RetIntNum()-3;i++)
			fmPr->fls[j]->cmbMeth->Items->Add(Fd->RetIntNm(i));
	fmPr->fls[0]->lbName->Caption="FRAP";
	fmPr->fls[1]->lbName->Caption="Reference";
	fmPr->fls[2]->lbName->Caption="Background";

	if (fmPr->ShowModal()==mrOk){
		fr=fmPr->fls[0]->cmbMeth->ItemIndex;
		ref=fmPr->fls[1]->cmbMeth->ItemIndex;
		back=fmPr->fls[2]->cmbMeth->ItemIndex;
		if (fdat==-1)
			FExper->SetInd(fr,ref,back);
		else{
			FData->SetInd(ifrap,fr);
			FData->SetInd(iref,ref);
			FData->SetInd(ibase,back);
		}
	}
	if(fdat>-1)
		showdata(FData);
	if (chFRAPs1->Visible)
		plotrenew();
	fmPr->Free();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSetBleachClick(TObject*)
{
	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->Caption="Set bleach time point";
	fmPr->SetNVPar(1,true,false,false);
	fmPr->fms[0]->lbName->Caption="bleach time point";
	if (fdat==-1)
    	fmPr->fms[0]->edVal->Text=IntToStr(FExper->RetD(0)->RetMBl()+1);
	else
		fmPr->fms[0]->edVal->Text=IntToStr(FData->RetMBl()+1);
	if(fmPr->ShowModal()==mrOk){
		int mbl=StrToInt(fmPr->fms[0]->edVal->Text)-1;
		if (fdat==-1)
			FExper->SetMBl(mbl);
		else
			FData->SetMBl(mbl);
	}
	fmPr->Free();
	if (fdat>-1)
		showdata(FData);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSetBleachTClick(TObject *){
	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->Caption="Set bleach time";
	fmPr->SetNVPar(1,true,false,true);
	fmPr->fms[0]->lbName->Caption="bleach time";
	if (fdat==-1)
		fmPr->fms[0]->edVal->Text=FloatToStrF(FExper->RetD(0)->RetTBl(),ffGeneral,4,4);
	else
		fmPr->fms[0]->edVal->Text=FloatToStrF(FData->RetTBl(),ffGeneral,4,4);
	fmPr->fms[0]->lbUn->Caption="s";
	if(fmPr->ShowModal()==mrOk){
		double tbl=fmPr->fms[0]->edVal->Text.ToDouble();
		if (fdat==-1)
			FExper->SetTBl(tbl);
		else
			FData->SetTBl(tbl);
	}
	fmPr->Free();
	if (fdat>-1)
		showdata(FData);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::ChangeWindow(void){
	pnleft->Visible=(FProj->RetExpNum()>0);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::plotrenew(void){
	for (int i=0;i<sernum;i++)
		delete chFRAPs1->Series[0];
	sernum=0;
	switch (cmbPlType->ItemIndex){
		case pall: plotall(); break;
		case pfrap: plotexp(ifrap);  break;
		case pref: plotexp(iref); break;
		case pback: plotexp(ibase); break;
		case pnorm: plotexp(inorm); break;
		case psiml: plotexp(isiml); break;
		case pnormsiml: plotexp(inorm); plotexp(isiml); break;
		case presid: plotexp(iresd); break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::plotexp(tcol _ind){
	if (fdat>-1)
		edatatoplot(FData,_ind);
	else
		for (int i=0; i<FExper->RetDtNum(); i++)
			edatatoplot(FExper->RetD(i) ,_ind);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::edatatoplot(CFRAPData* _fd,tcol _ind){
	//only signal simulated and residulals data are plotted for average dataset
	if ((_fd->RetType()!=expd)&&(_ind!=inorm)&&(_ind!=isiml)&&(_ind!=iresd))
		return;
	int pnum=_fd->RetPtNum();
	double* x=_fd->RetTm();
	double* y=_fd->RetIntArr(_fd->RetInd(_ind));


	TChartSeries* S;
	if (_ind==isiml){
		TLineSeries* Ser;
		Ser=new TLineSeries(this);
		Ser->Pen->Width=2;
    Ser->Color=clBlack;
		S=Ser;
	}
	else{
		TPointSeries* Ser;
		Ser=new TPointSeries(this);
		Ser->Pointer->VertSize=2;
		Ser->Pointer->HorizSize=2;
		S=Ser;
	}
	S->Title=_fd->RetNm();

	if ((x!=NULL)&&(y!=NULL))
		for (int j=0;j<pnum;j++){
			S->AddXY(x[j],y[j]);
	}
	chFRAPs1->AddSeries(S);
	sernum++;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::cmbPlTypeChange(TObject*){
	plotrenew();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::cmbShowTabChange(TObject*){
	updatetab();
  mainmenuavail();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::plotall(void){
	if (fdat==-1)
		return;
	if (FData->RetType()!=expd)
		return;
	TPointSeries* Ser;
	const int pnum=FData->RetPtNum();
	double* x=FData->RetTm();
	const int inum=FData->RetIntNum()-1-nfit;
	double* y;
	for (int i=0; i<inum; i++){
		y=FData->RetIntArr(i);
		Ser=new TPointSeries(this);
		Ser->Pointer->VertSize=2;
		Ser->Pointer->HorizSize=2;
		Ser->Title=FData->RetIntNm(i);
		if (i==FData->RetInd(ifrap))
			 Ser->Title=Ser->Title+"(frap)";
		if (i==FData->RetInd(iref))
			 Ser->Title=Ser->Title+"(ref)";
		if (i==FData->RetInd(ibase))
			 Ser->Title=Ser->Title+"(back)";



		if ((x!=NULL)&&(y!=NULL))
			for (int j=0;j<pnum;j++)
				Ser->AddXY(x[j],y[j]);
		chFRAPs1->AddSeries(Ser);
		sernum++;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::ipSaveClick(TObject*){
	//save plot
	AnsiString nm;
	int l;
	dlSave->Options.Clear();
	dlSave->Filter = "Bitmap Files (*.bmp)|*.bmp|Windows MetaFiles (*.wmf)|*.wmf";
	dlSave->FilterIndex = 2; //start the dialog showing *.wmf files
	dlSave->DefaultExt="wmf";

	dlSave->FileName=cur->Text;
	if(dlSave->Execute()){
		nm=dlSave->FileName;
		l=nm.LastDelimiter(".");
		if (l>0)
			nm.Delete(l,nm.Length()-l+1);
		l=dlSave->FilterIndex;
		switch(l){
			case 1: nm=nm+".bmp"; break;
			case 2:	nm=nm+".wmf"; break;
		} 
		if	(std::ifstream(nm.c_str(), ios::in | ios::nocreate)!=NULL)
			if (!OverWrFl(nm.c_str()))
				return;
		switch(l){
			case 1: chFRAPs1->SaveToBitmapFile(nm); break;
			case 2:	chFRAPs1->SaveToMetafile(nm); break;
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall TfmMain::OverWrFl(const char* _nm){
	AnsiString mes="File "+AnsiString(_nm)+" already exists. Do You want to overwrite it?";
	return (Application->MessageBox(mes.c_str(), "Warning!!!",MB_YESNO| MB_ICONWARNING | MB_DEFBUTTON1)==IDYES);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itDoubleNormClick(TObject*){
/*	if (Exper->ExpData[0]->mbleach==0){
		Application->MessageBox("Number of bleach time channel should be more than 1","Warning!!!", MB_OK);
		return;
	}                */

	if (1)
		FProj->RunNormE(doub,fexp);
	else
		FProj->RunNormD(doub, fexp, fdat);

	mssuccess("Double normalization");
	ChangeWindow();//showdata(FData);
	findcurnode();//plotrenew();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSingleNormClick(TObject*){
/*	if (Exper->ExpData[0]->mbleach==0){
		Application->MessageBox("Number of bleach time channel should be more than 1","Warning!!!", MB_OK);
		return;
	}  */
	if (1)
		FProj->RunNormE(sing,fexp);
	else
		FProj->RunNormD(sing, fexp, fdat);
	mssuccess("Single normalization");
	ChangeWindow();//showdata(FData);
	findcurnode();//plotrenew();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itDoubleNormFullClick(TObject*){
/*	if (Exper->ExpData[0]->mbleach==0){
		Application->MessageBox("Number of bleach time channel should be more than 1","Warning!!!", MB_OK);
		return;
	} */

	if (1) {
		FProj->RunNormE(doub,fexp);
		FProj->RunNormE(fulc,fexp);
	}
	else{
		FProj->RunNormD(doub, fexp, fdat);
		FProj->RunNormD(fulc, fexp, fdat);
	}

	mssuccess("Double normalization with Full Scale Calibration");
	ChangeWindow();//showdata(FData);
	findcurnode();//plotrenew();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSingleNormFullClick(TObject*){
	/*if (Exper->ExpData[0]->mbleach==0){
		Application->MessageBox("Number of bleach time channel should be more than 1","Warning!!!", MB_OK);
		return;
	} */
	if (1) {
		FProj->RunNormE(sing,fexp);
		FProj->RunNormE(fulc,fexp);
	}
	else{
		FProj->RunNormD(sing, fexp, fdat);
		FProj->RunNormD(fulc, fexp, fdat);
	}
	mssuccess("Single normalization with Full Scale Calibration");
	ChangeWindow();//showdata(FData);
	findcurnode();//plotrenew();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::mssuccess(char* method){
	mReport->Lines->Add("******************************");
	if(state==1)
		mReport->Lines->Add(AnsiString(method) + " was applied successfully");
	else
		mReport->Lines->Add("Program failed while tried to run" + AnsiString(method));
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itNoNormClick(TObject*){
	if (1)
		FProj->RunNormE(nonorm,fexp);
	else
		FProj->RunNormD(nonorm, fexp, fdat);

	if (state==1)
		mReport->Lines->Add("Data were copied without normalization");
	else
		mReport->Lines->Add("FRAP ROI was not defined correctly");
	ChangeWindow();//showdata(FData);
	findcurnode();//plotrenew();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itBeadNormClick(TObject*){
	if (1)
		FProj->RunNormE(beadn,fexp);
	else
		FProj->RunNormD(beadn, fexp, fdat);

	mssuccess("Bead normalization");
	ChangeWindow();//showdata(FData);
	findcurnode();//plotrenew();

}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itModelsClick(TObject*){
	if (fexp<0)
		return;
	if (FExper->RetDtNum()<1)
		return; //probably error

	CFRAPData* fd;
	FRAPMod* mdl;
	int mind;
	TComboBox* cmb;
	double* arr;
	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->SetNVPar(0,true,false,false);
	fmPr->SetNLPar(2);

	fmPr->Caption="Step 1: Choice of model";
	fmPr->fls[0]->lbName->Caption="Model name";
	//List of models
	cmb=fmPr->fls[0]->cmbMeth;
	cmb->Items->Clear();
	for (int i=0; i<nmodl; i++)
		cmb->Items->Add(retmodelname(i));
	cmb->ItemIndex=0;
	cmb->DropDownCount=nmodl;

	fmPr->fls[1]->lbName->Caption="Simulate for";
	cmb=fmPr->fls[1]->cmbMeth;
	cmb->Items->Add("All Data in Experiment");
	cmb->Items->Add("Single DataSet");
	cmb->ItemIndex=0;
	if (fdat<0)
		cmb->Enabled=false;


	if (fmPr->ShowModal()!=mrOk){
		fmPr->Free();
		return;
	}

	mind=fmPr->fls[0]->cmbMeth->ItemIndex;
	if (mind!=FExper->RetModTp())
		FExper->SetModl(mind);
	mind=fmPr->fls[1]->cmbMeth->ItemIndex;
	fmPr->Free();

	Application->CreateForm(__classid(TfmPr), &fmPr);
	mdl=FExper->RetModl();
	fmPr->SetNVPar(mdl->RetPrNum(),true,false,true);
	fmPr->Caption="Step 2: Determination of parameters";
	//loading of initial parameters
	fmPr->LoadParAttr(mdl);
	if (fdat>-1)
		fd=FData;
	else
		fd=FExper->RetD(0);
	fmPr->LoadParVal(fd->RetPPar());

	if (fmPr->ShowModal()!=mrOk){
		fmPr->Free();
		updatetab();
		return;
	}
	arr=new double[mdl->RetPrNum()];
	fmPr->RetPar(arr);
	if (mdl->InRange(arr))
		if (mind==1)
			FExper->RunSim(arr,fdat);//simulate for one dataset
		else
			FExper->RunSim(arr);		 //simulate for all datasets in experiment
	else
		errmes(2);

	delete [] arr;
	fmPr->Free();
	updatetab();
	plotrenew();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itOptimClick(TObject*){
	if (fexp<0)
		return;
	if (FExper->RetDtNum()<1)
		return; //probably error

	int i;
	FRAPMod* mdl=NULL;
	double* stpar=NULL;
	double* par=NULL;
	double* dpar=NULL;
	int* ipar=NULL;
	int par_num;
	TComboBox* cmb;

	Application->CreateForm(__classid(TfmFitMenu), &fmFitMenu);
	//List of models
	cmb=fmFitMenu->cmbFMod;
	cmb->Items->Clear();
	for (i=0; i<nmodl; i++)
		cmb->Items->Add(retmodelname(i));
	cmb->ItemIndex=fitpar.fmod;
	cmb->DropDownCount=nmodl;

	//List of fitting methods
	cmb=fmFitMenu->cmbFMeth;
	cmb->Items->Clear();
	for (i=0; i<nfitmeth; i++)
		cmb->Items->Add(retfitname(i));
	cmb->ItemIndex=fitpar.fmeth;


	fmFitMenu->edIterMax->Text=AnsiString(fitpar.iter_max);
	fmFitMenu->chbFitAll->Checked=fitpar.fit_all;
	fmFitMenu->edFirstPt->Text=AnsiString(fitpar.st_pt+1);
	if (fitpar.fin_pt==0) {
		if (fdat<0)
			fmFitMenu->edLastPt->Text=AnsiString(FExper->RetD(0)->RetPtNum());      //correct after
		else
			fmFitMenu->edLastPt->Text=AnsiString(FData->RetPtNum());
	}
	else
		fmFitMenu->edLastPt->Text=AnsiString(fitpar.fin_pt+1);

	if (fmFitMenu->ShowModal()!=mrOk){
		fmFitMenu->Free();
		return;
	}

	fitpar.fmod=fmFitMenu->cmbFMod->ItemIndex;
	if (fitpar.fmod!=FExper->RetModTp())
		mdl=FExper->SetModl(fitpar.fmod);
	else
		mdl=FExper->RetModl();
	if (mdl==NULL){ChState(-506);return;}														 				//fail
	fitpar.fmeth=fmFitMenu->cmbFMeth->ItemIndex;
	fitpar.iter_max=fmFitMenu->edIterMax->Text.ToDouble();
	fitpar.fit_all=	fmFitMenu->chbFitAll->Checked;
	if (!fitpar.fit_all) {
		fitpar.st_pt=fmFitMenu->edFirstPt->Text.ToDouble()-1;
		fitpar.fin_pt=fmFitMenu->edLastPt->Text.ToDouble()-1;
	}
	fmFitMenu->Free();

	par_num=mdl->RetPrNum();
	stpar=new double[par_num];
	par=new double[par_num];
	mdl->DefParVal(stpar);
	dpar=new double[par_num];
	ipar=new int[par_num];
	for (int i=0;i<par_num;i++)
		ipar[i]=1;

	//determination of MINIMAL parameters values
	//Application->CreateForm(__classid(TfmPr), &fmPr);

	//determination of MAXIMAL parameters values


	//determination of initial parameters
	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->SetNVPar(par_num,true,true);
	fmPr->Caption="Step 2: Determination of initial parameters";
	//loading of initial parameters
	fmPr->LoadParAttr(mdl);

	//Check if starting parameters are in range
	do{
		fmPr->LoadParVal(stpar, ipar);
		if(fmPr->ShowModal()==mrOk){
			fmPr->RetPar(par,ipar);
			if (mdl->InRange(par))
				break;
			else
				errmes(2);
		}
		else
			goto fin;
	}while(1);


	mReport->Lines->Add("******************************");
	mReport->Lines->Add("Fitting method started. Please wait.");
	fmMain->Refresh();
	if (fdat==-1)
		FExper->FitExper(par,ipar,fitpar.fmeth,fitpar.fit_all,fitpar.st_pt,fitpar.fin_pt,fitpar.iter_max);
	else
		FExper->FitDataS(par,ipar,fitpar.fmeth,fitpar.fit_all,fitpar.st_pt,fitpar.fin_pt,fitpar.iter_max,fdat);
	if (state==1)
		mReport->Lines->Add("Fitting method successfully finished");
	else
		mReport->Lines->Add("Probably errors occured during fitting");

fin:
	fmPr->Free();
	if (stpar!=NULL)delete [] stpar;
	if (par!=NULL) 	delete [] par;
	if (ipar!=NULL) delete [] ipar;
	if (dpar!=NULL) delete [] dpar;

	updatetab();
	plotrenew();
}

/*	do{
		*iter_num=0;
		fmPr->Free();
		//determination of initial parameters
		Application->CreateForm(__classid(TfmPr), &fmPr);
		fmPr->SetNVPar(par_num,true,true);
		fmPr->Caption="Analysis Step 2: Determination of initial parameters";
		//loading of initial parameters
		fmPr->LoadParAttr(mdl);
		do{
			fmPr->LoadParVal(stpar, ipar);
			if(fmPr->ShowModal()==mrOk){
				fmPr->RetPar(par,ipar);
				if (mdl->InRange(par))
					break;
				else
					errmes(2);
			}
			else
				goto fin;
		}while(1);
		ipar_num=0;
		for (int i=0;i<par_num;i++)
			ipar_num+=ipar[i];

		mReport->Lines->Add("******************************");
		mReport->Lines->Add("Fitting method started. Please wait.");
		fmMain->Refresh();
		switch (mind){
			case 0:{
				mrqrun(AnD,mdl,st_pt,fin_pt, par, ipar, dpar, &chif, iter_num, iter_max);
				break;
			}
			case 1:{
				Simpleks* Smp=new Simpleks(mdl,st_pt,fin_pt,AnD);
				Smp->poisk_optim(par,ipar,*iter_num,chif,dpar);
				delete Smp;
				break;
			}
		}
		if ((*iter_num)<iter_max)
			mReport->Lines->Add("Fitting method finished after "+ IntToStr(*iter_num)+" iterations");
		else
			mReport->Lines->Add("Fitting method reached maximum number of iterations ("+ IntToStr(iter_max)+")");
		mdl->DisplayPar(par, mReport, dpar);
		//mdl->evaltarray(AnD->time,AnD->point_num, par, AnD->Intens[AnD->imodl]);
		mReport->Lines->Add("final chi2 value "+FloatToStrF(chif/(fin_pt-st_pt-ipar_num-1),ffGeneral,4,4)/*FloatToStrF(chif,ffFixed,3,4)*);
		for (int i=0; i<par_num;i++)
			stpar[i]=par[i];
		itShowDataClick(this);
		//do we need to continue?
	}while (Application->MessageBox("Would you like to continue?", "Continue fitting?", MB_OKCANCEL)==IDOK);

 */
//---------------------------------------------------------------------------
void __fastcall TfmMain::ipShGoodClick(TObject*){
	ipShGood->Checked=!ipShGood->Checked;
	updatetab();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::ipShIterClick(TObject *){
	ipShIter->Checked=!ipShIter->Checked;
	updatetab();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//temporarily hindered
void __fastcall TfmMain::itSingExpClick(TObject*){
/*		if (simmod==NULL) {
		simmod=new SingExpMod(0);
		simdial();
	}
	else
		errmes(1);*/

}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itDoubExpClick(TObject*){
/*		if (simmod==NULL) {
		simmod=new DoubExpMod(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSoumpasisDiffClick(TObject*){
/*	if (simmod==NULL) {
		simmod=new SoumpDiffMod(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itEllenbergDiffClick(TObject*){
/*	if (simmod==NULL) {
		simmod=new EllDiffMod(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSetTmArrClick(TObject*){
/*
	//const int nsum=10000;
	//number of parameters
	Application->CreateForm(__classid(TfmModelParam), &fmModelParam);
	fmModelParam->setparnum(3);
	//parameters names
	fmModelParam->lbparam1->Caption="time step value";
	fmModelParam->lbparam2->Caption="number of time steps";
	fmModelParam->lbparam3->Caption="Number of intencity columns";
	//parameters units
	fmModelParam->lbun1->Caption="s";
	fmModelParam->lbun2->Caption="";
	fmModelParam->lbun3->Caption="";
	//predetermined parameters
	fmModelParam->edparam1->Text=FloatToStr(1);
	fmModelParam->edparam2->Text=IntToStr(100);
	fmModelParam->edparam3->Text=IntToStr(10);


	if (fmModelParam->ShowModal()==mrOk){
		Data1=new FRAPData(fmModelParam->edparam3->Text.ToInt());
		Data1->LoadTime(fmModelParam->edparam1->Text.ToDouble(),fmModelParam->edparam2->Text.ToInt());

		mReport->Lines->Add("Data were successfully initialized by time values");
		fmModelParam->Free();
		itShowData->Enabled=true;
		itShowDataClick(this);
		itSetInd->Enabled=true;
		itSetIndClick(this);
		itDelData->Enabled=true;
		itLoadData->Enabled=false;
		itTpData->Enabled=false;
		itSetTmArr->Enabled=false;

		itPlots->Enabled=true;
		itNormalization->Enabled=true;
		itModels->Enabled=true;
		itOptim->Enabled=true;
		itCombFit->Enabled=true;
		itShowDataClick(this);
	}
*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itTpDataClick(TObject*){
/*  Application->CreateForm(__classid(TfmModelParam), &fmModelParam);
	fmModelParam->Caption="Data table properties";
	//number of parameters
	fmModelParam->setparnum(1);
	//parameters names
	fmModelParam->lbparam1->Caption="Number of intencity columns";
	//parameters units
	fmModelParam->lbun1->Caption="";
	//predetermined parameters
	fmModelParam->edparam1->Text=IntToStr(10);
	if (fmModelParam->ShowModal()==mrOk){
		Data1 = new FRAPData(fmModelParam->edparam1->Text.ToInt());

		mReport->Lines->Add("Now you can insert your data");
		Application->CreateForm(__classid(TfmTextEdit), &fmTextEdit);
		fmModelParam->Free();
		fmTextEdit->Caption="Input FRAP data";
		fmTextEdit->mData->Clear();

		if (fmTextEdit->ShowModal()==mrOk){
			fmTextEdit->mData->Lines->SaveToFile("f1.txt");

			if(Data1->LoadData("f1.txt",0)==1){
				mReport->Lines->Add("Data were loaded successfully");
				itShowData->Enabled=true;
				itShowDataClick(this);
				itSetInd->Enabled=true;
				itSetIndClick(this);
				itDelData->Enabled=true;
				itLoadData->Enabled=false;
				itTpData->Enabled=false;
				itSetTmArr->Enabled=false;

				itPlots->Enabled=true;
				itNormalization->Enabled=true;
				itModels->Enabled=true;
				itOptim->Enabled=true;
				itCombFit->Enabled=true;
			}
			else{
				mReport->Lines->Add("Data loading is failed");
				delete Data1;
			}
		}
		else
			delete Data1;

		fmTextEdit->Free();
	}    */
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itOneBndClick(TObject*)
{
/*	if (simmod==NULL) {
		simmod=new OneBndMod(0);
		simdial();
	}
	else
		errmes(1);  */
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itTwoBndClick(TObject*)
{
/*	if (simmod==NULL) {
		simmod=new TwoBndMod(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itChColNmClick(TObject*){
	/*Application->CreateForm(__classid(TfmOptim), &fmOptim);

	//Choice of model
	//Application->CreateForm(__classid(TfrFitMeth),&(fmOptim->frFitMethOpt));
	fmOptim->frFitMethOpt->Visible=true;
	fmOptim->Caption="Renaming of columns";
	//List of models
	fmOptim->frFitMethOpt->cmbMeth->Items->Clear();
	for(int i=0;i<Data1->intens_num;i++)
		fmOptim->frFitMethOpt->cmbMeth->Items->Add(AnsiString(Exper->ExpIntNames[i]));
	fmOptim->frFitMethOpt->cmbMeth->ItemIndex=0;

	fmOptim->frFitMethOpt->lbMeth->Caption="Intensity columns";
	//index of data values
	fmOptim->frFitMethOpt->lbfmet1->Visible=true;
	fmOptim->frFitMethOpt->lbfmet1->Caption="new name";
	fmOptim->frFitMethOpt->edfmet1->Visible=true;
	fmOptim->frFitMethOpt->edfmet1->Text="data";

	if (fmOptim->ShowModal()==mrOk){
		int j=fmOptim->frFitMethOpt->cmbMeth->ItemIndex;
		StrCopy(Exper->ExpIntNames[j],fmOptim->frFitMethOpt->edfmet1->Text.c_str());
	}
	else{
		fmOptim->frFitMethOpt->Visible=false;
		goto fin;
	}
	fin:
	fmOptim->Free();
	itShowDataClick(this);
*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itWorkMClick(TObject*){
/*	if (simmod==NULL){
		simmod=new WrkMod(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itbnddiffClick(TObject*){
/*	if (simmod==NULL){
		simmod=new BndDiffMod(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itLoadFilesClick(TObject*)
{
/*	int i;
	AnsiString mes;
	AnsiString nm;
	char* cname;
	char* fname;

	cname=new char[200];
	fname=new char[80];

	dlOpen->Options << ofAllowMultiSelect << ofFileMustExist << ofHideReadOnly;
	dlOpen->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
	dlOpen->FilterIndex = 1; // start the dialog showing all files
	if(dlOpen->Execute()){
		fnum=dlOpen->Files->Count;
		mes=AnsiString(fnum)+" files were chosen:\n";
		for (i=0; i<fnum;i++)
			mes=mes+"\n"+dlOpen->Files->Strings[i];
		if (Application->MessageBox(mes.c_str(), "Files are chosen", MB_OKCANCEL)==IDOK){
			Exper=new FRAPExper(fnum);
			rgShow->Items->Clear();
			mReport->Lines->Add("******************************");
			for (i=0; i<fnum;i++){
				if(Exper->LoadFl(dlOpen->Files->Strings[i].c_str(),i)==1){
					mReport->Lines->Add("File # "+AnsiString(i+1)+" was loaded successfully");
					StrCopy(cname,dlOpen->Files->Strings[i].c_str());
					retname(cname,fname);
					nm=AnsiString(fname);
					rgShow->Items->Add(nm.SetLength(nm.Length()-4));
				}
				else{
					mReport->Lines->Add("File # "+AnsiString(i+1)+" loading is failed");
					Application->MessageBox("Problems with data loading", "Warning!!!", MB_OK);
					itDelDataClick(this);
					return;
				}
			}
			dloaded();
			rgShow->ItemIndex=0;
			itSetIndClick(this);
			itSetBleachClick(this);
			for (int j=0;j<Exper->sampnum;j++)
				StrCopy(Exper->ExpData[j]->ExpIntNames[Exper->ExpData[j]->inorm],"Normalized");
		}
	}

	delete [] fname;
	delete [] cname;*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::rgShowClick(TObject*)
{
/*	if (rgShow->ItemIndex>-1)
		itShowDataClick(this);   */
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itNewAnalClick(TObject*){
	/*char nname[80];
	int snum=Exper->sampnum;
	int* sind=new int[snum];
	if (Exper->analnum>9){
		Application->MessageBox("Before starting new analysis remove some old sequences",
														"Too many analyzed sequences", MB_OK);
		return;
	}
	StrCopy(nname,"");
	Application->CreateForm(__classid(TfmChooseFile), &fmChooseFile);
	fmChooseFile->Caption="New analysis";
	fmChooseFile->chbTitles->Visible=false;
	fmChooseFile->Label1->Caption="Choose name for new analysis";
	fmChooseFile->edflname->Text="Analysis "+AnsiString(Exper->analcnt+1);
	if (fmChooseFile->ShowModal()==mrOk){
		StrCopy(nname,fmChooseFile->edflname->Text.c_str());
		rgShow->Items->Add(AnsiString(nname));

		Application->CreateForm(__classid(TfmPr), &fmPr);
		fmPr->SetNVPar(snum,false,true,false);
		fmPr->Caption="Choose Experimental Datasets for averaging";
		for (int i=0;i<snum;i++)
			fmPr->fms[i]->lbName->Caption=rgShow->Items->Strings[i];
		if (fmPr->ShowModal()==mrOk)
			fmPr->RetVar(sind);
		else{
			fmPr->Free();
			fmChooseFile->Free();
			return;
		}
		Exper->NewAnls(sind);
		rgShow->ItemIndex=rgShow->Items->Count-1;
		fmPr->Free();
	}
	delete [] sind;
	fmChooseFile->Free();
	return;*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itDelAnalClick(TObject*){
	/*int anind;
	anind=rgShow->ItemIndex-Exper->sampnum;
	if ((anind<0)||(anind>Exper->analnum-1)){
		Application->MessageBox("Choose analysis dataset to apply this option","Warning!!!", MB_OK);
		return;
	}
	Exper->DelAnls(anind);
	rgShow->Items->Delete(rgShow->ItemIndex);
	rgShow->ItemIndex=rgShow->Items->Count-1;
	itShowDataClick(this);
	return;*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itpolymmodClick(TObject*){
	/*if (simmod==NULL) {
		simmod=new PolymerMod(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itpolymmod2Click(TObject*){
	/*if (simmod==NULL) {
		simmod=new PolymerMod2(0);
		simdial();
	}
	else
		errmes(1);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::simdial(bool _s){
	/*
	int anind=rgShow->ItemIndex-Exper->sampnum;
	if ((anind<0)||(anind>Exper->analnum-1)){
		Application->MessageBox("Choose analytical dataset for this option", "Warning!!!", MB_OK);
		return;
	}
	double* param=new double[simmod->RetPrNum()];
	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->SetNVPar(simmod->RetPrNum(),true,false);
	fmPr->LoadParAttr(simmod);
	do{
		simmod->DefParVal(param);
		fmPr->LoadParVal(param);
		if(fmPr->ShowModal()==mrOk){
			fmPr->RetPar(param);
			if (simmod->InRange(param)){
				if (_s)
					state=Exper->AnData[anind]->EvalMArr(simmod,param);
				break;
			}
			errmes(2);
		}
		else break;
	}while(1);

	fmPr->Free();

	delete [] param;
	delete simmod;
	simmod=NULL;
	itShowDataClick(this);*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itCombFitClick(TObject*){
/*	int i, j;
	int annum=Exper->analnum;
	FRAPAnData* AnD1;
	FRAPAnData* AnD2;
	int anft1, anft2;
	FRAPMod* mdl=NULL;
	int iter_max, mind;
	int st_pt, fin_pt;
	int par_num;
	TComboBox* cmb;
	double* stpar=NULL;
	double* par1=NULL;
	double* par2=NULL;
	double* dpar1=NULL;
	double* dpar2=NULL;
	int* ipar=NULL;
	int* lpar=NULL;
	int* iter_num=new int(0);
	int ipar_num;
	double chif;
	//int annumft=0;
	//bool anft[annum];

	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->SetNLPar(2);
	fmPr->Caption="Step 1: Choice of datasets";
	fmPr->fls[0]->lbName->Caption="Analysis dataset 1";
	fmPr->fls[1]->lbName->Caption="Analysis dataset 2";

	for (j=0;j<2;j++)
		for (i=0; i<annum; i++)
			fmPr->fls[j]->cmbMeth->Items->Add(rgShow->Items->Strings[i+Exper->sampnum]);

	if (fmPr->ShowModal()==mrOk){
		anft1=fmPr->fls[0]->cmbMeth->ItemIndex;
		anft2=fmPr->fls[1]->cmbMeth->ItemIndex;
	}
	else
		goto fin;
	fmPr->Free();

	AnD1=Exper->AnData[anft1];
	AnD2=Exper->AnData[anft2];

	Application->CreateForm(__classid(TfmPr), &fmPr);
	fmPr->SetNVPar(3,true,false,false);
	fmPr->SetNLPar(2);

	fmPr->Caption="Analysis Step 2: Choice of model";
	fmPr->fls[0]->lbName->Caption="Model";
	//List of models
	cmb=fmPr->fls[0]->cmbMeth;
	cmb->Items->Clear();
	cmb->Items->Add("One binding state");	 //0
	cmb->Items->Add("Two binding states");  //1
	cmb->Items->Add("Diffusion circular spot"); //2
	cmb->Items->Add("Diffusion rectangular spot"); //3
	cmb->Items->Add("Binding+diffusion circualr spot"); //4
	cmb->Items->Add("Polymerization"); //5
	cmb->Items->Add("Polymerization2"); //6
	cmb->ItemIndex=0;

	fmPr->fls[1]->lbName->Caption="Fitting method";
	//List of fitting methods
	cmb=fmPr->fls[1]->cmbMeth;
	cmb->Items->Clear();
	cmb->Items->Add("Levenberg-Marquardt (gradient)");
	cmb->Items->Add("Nelder-Mead (simplex)");
	cmb->ItemIndex=1;

	fmPr->fms[0]->lbName->Caption="Maximum number of iterations";
	fmPr->fms[0]->edVal->Text="200";
	fmPr->fms[1]->lbName->Caption="First time channel";
	fmPr->fms[1]->edVal->Text="1";
	fmPr->fms[2]->lbName->Caption="Last time channel";
	fmPr->fms[2]->edVal->Text=AnsiString(AnD1->point_num);


	if (fmPr->ShowModal()==mrOk){
		switch (fmPr->fls[0]->cmbMeth->ItemIndex){
			case 0: mdl=new OneBndMod(0);		break;
			case 1: mdl=new TwoBndMod(0);		break;
			case 2: mdl=new SoumpDiffMod(0);break;
			case 3: mdl=new EllDiffMod(0);	break;
			case 4: mdl=new BndDiffMod(0);	break;
			case 5:	mdl=new PolymerMod(0);	break;
			case 6:	mdl=new PolymerMod2(0);	break;
		}
		par_num=mdl->RetPrNum();
		stpar=new double[par_num];
		par1=new double[par_num];
		par2=new double[par_num];
		mdl->DefParVal(stpar);
		dpar1=new double[par_num];
		dpar2=new double[par_num];
		ipar=new int[par_num];
		lpar=new int[par_num];
		for (int i=0;i<par_num;i++){
			ipar[i]=1;
			lpar[i]=0;
		}
		mind=fmPr->fls[1]->cmbMeth->ItemIndex;
		iter_max=fmPr->fms[0]->edVal->Text.ToDouble();
		st_pt=fmPr->fms[1]->edVal->Text.ToDouble()-1;
		fin_pt=fmPr->fms[2]->edVal->Text.ToDouble();
	}
	else
		goto fin;

	do{
		*iter_num=0;
		fmPr->Free();
		//determination of initial parameters
		Application->CreateForm(__classid(TfmPr), &fmPr);
		fmPr->SetNVPar(par_num,true,true);
		fmPr->Caption="Analysis Step 3: Determination of initial parameters";
		//loading of initial parameters
		fmPr->LoadParAttr(mdl);
		do{
			fmPr->LoadParVal(stpar, ipar);
			if(fmPr->ShowModal()==mrOk){
				fmPr->RetPar(par1,ipar);
				if (mdl->InRange(par1)){
					for (j=0;j<par_num;j++)
						par2[j]=par1[j];
					break;
				}
				else
					errmes(2);
			}
			else
				goto fin;
		}while(1);
		fmPr->Caption="Analysis Step 4: Determination of links between parameters";
		fmPr->LoadParVal(par1,lpar);
		for (j =0; j<par_num;j++)
			fmPr->fms[j]->edVal->Enabled=false;
		fmPr->hcPar->Sections->Items[2]->Text="Link";
		if(fmPr->ShowModal()==mrOk)
			fmPr->RetPar(stpar,lpar);
		else
			goto fin;
		ipar_num=0;
		for (int i=0;i<par_num;i++)
			ipar_num+=ipar[i];

		mReport->Lines->Add("******************************");
		mReport->Lines->Add("Fitting method started. Please wait.");
		fmMain->Refresh();
		switch (mind){
			case 0:{
				//mrqrun(AnD,mdl,st_pt,fin_pt, par, ipar, dpar, &chif, iter_num, iter_max);
				break;
			}
			case 1:{
				Simpleks* Smp=new Simpleks(mdl,st_pt,fin_pt,AnD1,AnD2);
				Smp->poisk_optim_comb(par1,par2,ipar,lpar,*iter_num,chif,dpar1,dpar2);
				//delete Smp;
				break;
			}
		}
		if ((*iter_num)<iter_max)
			mReport->Lines->Add("Fitting method finished after "+ IntToStr(*iter_num)+" iterations");
		else
			mReport->Lines->Add("Fitting method reached maximum number of iterations ("+ IntToStr(iter_max)+")");
		mReport->Lines->Add("Parameters for dataset "+rgShow->Items->Strings[anft1+Exper->sampnum]);
		mdl->DisplayPar(par1, mReport, dpar1);
		mReport->Lines->Add("Parameters for dataset "+rgShow->Items->Strings[anft2+Exper->sampnum]);
		mdl->DisplayPar(par2, mReport, dpar2);
		//mdl->evaltarray(AnD->time,AnD->point_num, par, AnD->Intens[AnD->imodl]);
		mReport->Lines->Add("final chi2 value "+FloatToStrF(chif/(fin_pt-st_pt-ipar_num-1),ffGeneral,4,4)/*FloatToStrF(chif,ffFixed,3,4)*);
		for (int i=0; i<par_num;i++)
			stpar[i]=par1[i];
		itShowDataClick(this);
		//do we need to continue?
	}while (false/*Application->MessageBox("Would you like to continue?", "Continue fitting?", MB_OKCANCEL)==IDOK*);


fin:
	if (mdl!=NULL) 		delete mdl;
	if (stpar!=NULL) 	delete stpar;
	if (par1!=NULL) 	delete par1;
	if (par2!=NULL) 	delete par2;
	if (dpar1!=NULL) 	delete dpar1;
	if (dpar2!=NULL) 	delete dpar2;
	if (ipar!=NULL) 	delete ipar;
	if (lpar!=NULL) 	delete lpar;
	delete iter_num;
	fmPr->Free();*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itSvNormDtClick(TObject*){
/*	int i;

	dlSave->Options.Clear();
	dlSave->Filter = "Text files (*.txt)|*.txt";
	dlSave->FilterIndex=1; // start the dialog showing all files
	dlSave->DefaultExt="txt";
	dlSave->FileName="Normalized";

	if(dlSave->Execute()){
		ofstream o_stream;
		if	(std::ifstream(dlSave->FileName.c_str(), ios::in | ios::nocreate)!=NULL)
			if (!OverWrFl(dlSave->FileName.c_str()))
				return;
		o_stream.open((dlSave->FileName).c_str());

		for (i=0;i<Exper->sampnum;i++)
			o_stream<<rgShow->Items->Strings[i].c_str()<<'\t'<<'\t';
		o_stream<<'\n';

		for (i=0;i<Exper->sampnum;i++)
			o_stream<<"Time[s]"<<'\t'<<"Normalised"<<'\t';
		o_stream<<'\n';

		Exper->SvNormDt(&o_stream);
		o_stream.close();
		mReport->Lines->Add("Data were saved successfully");
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itPostToUnClick(TObject*){
/*	int s;
	state=10;

	for (int i=0;i<Exper->sampnum;i++){
		s=Exper->ExpData[i]->PostToUn();
		if (s<state) state=s;
	}

	mssuccess("Double normalization");
	itShowDataClick(this);
	*/
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::itPostSigUnClick(TObject*){
	//Exper->AnData[rgShow->ItemIndex-Exper->sampnum]->PostSigUn();
}
//---------------------------------------------------------------------------

