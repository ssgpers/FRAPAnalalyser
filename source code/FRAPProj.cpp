//---------------------------------------------------------------------------


#pragma hdrstop

#include "FRAPProj.h"
#include "FRAPDefin.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
//------------------------class CFRAPProj------------------------------------
CFRAPProj::CFRAPProj(){
	expnum=0;
	expcnt=0;
	mexpnum=5;
	Exp=(CFRAPExper**)malloc(sizeof(CFRAPExper**)*mexpnum);
}
//---------------------------------------------------------------------------
CFRAPProj::~CFRAPProj(){
	for (int i=0;i<expnum;i++)
		delete Exp[i];
	free(Exp);
}
//---------------------------------------------------------------------------
CFRAPExper* CFRAPProj::AddExper(){
	char* s=new char[LenIntNm];
	char s1[5];
	itoa(expcnt+1,s1,10);
	StrCopy(s,exprnm);
	StrLCat(s,s1,LenIntNm);
	AddMem();
	Exp[expnum-1]=new CFRAPExper(s);
	delete [] s;
	return Exp[expnum-1];
}
//---------------------------------------------------------------------------
CFRAPExper* CFRAPProj::AddToList(CFRAPExper* _AExp){
	AddMem();
	Exp[expnum-1]=_AExp;
	return Exp[expnum-1];
}
//---------------------------------------------------------------------------
void CFRAPProj::DelExper(int _iexp){
	if (_iexp>expnum-1){ChState(-401);return;}															//fail
	delete Exp[_iexp];
	for (int i=_iexp+1;i<expnum;i++)
		Exp[i-1]=Exp[i];
	Exp[expnum-1]=NULL;
	expnum--;
}
//---------------------------------------------------------------------------
void CFRAPProj::CopyExpNms(char** _Nms)const{
	for (int i=0;i<expnum;i++)
		Exp[i]->CopyNm(_Nms[i]);
}
//---------------------------------------------------------------------------
void CFRAPProj::CopyDatNms(char** _Nms, int* _tp, int _iexp)const{
	CFRAPExper* E=Exp[_iexp];
	CFRAPData* D;
	int datan=E->RetDtNum();
	for (int i=0; i<datan; i++){
		D=E->RetD(i);
		D->CopyNm(_Nms[i]);
		_tp[i]=D->RetType();
	}
}
//---------------------------------------------------------------------------
char*	CFRAPProj::RetExpNm(int _iexp)const{
	return Exp[_iexp]->RetNm();
}
//---------------------------------------------------------------------------
char*	CFRAPProj::RetDatNm(int _iexp, int _idat)const{
	return Exp[_iexp]->RetD(_idat)->RetNm();
}
//---------------------------------------------------------------------------
void CFRAPProj::SvPar(char* _filenm,bool _biter, bool _bcrit)const{
	if (expnum<1){ChState(-402);return;}																		//fail

	int i;
	ofstream o_stream;
	tmodl imod;
	FRAPMod* Md;
	int parnum;

	o_stream.open(_filenm);
	if (!o_stream.good()){ChState(-403);return;} 											 			//fail
	imod=Exp[0]->RetModTp();
	for (i=1; i<expnum; i++)
		if (Exp[i]->RetModTp()!=imod){
			o_stream<<"The same model has to be used for all data";
			o_stream.close();
			return;
		}
	Md=Exp[0]->RetModl();
	parnum=Md->RetPrNum();

	//header line
	o_stream<<"Parameter label";
	for (i=0; i<parnum; i++)
		o_stream<<'\t'<<Md->RetParLb(i);
	if (_biter)
		o_stream<<'\t'<<"Fitting Iterations";
	if (_bcrit)
		o_stream<<'\t'<<"Goodness of Fit";
	o_stream<<'\t'<<"Condition Name";
	o_stream<<'\n'<<"Parameter unit";
	for (i=0; i<parnum; i++)
		o_stream<<'\t'<<Md->RetParUn(i);
	if (_biter)
		o_stream<<'\t'<<"";
	if (_bcrit)
		o_stream<<'\t'<<"";
	o_stream<<'\t'<<"";

	for (i=0; i<expnum; i++)
		Exp[i]->ParToStream(o_stream,-1,true,_biter,_bcrit);

	if (!o_stream.good()){ChState(-404);return;} 											 			//fail
	o_stream.close();
}
//---------------------------------------------------------------------------
bool CFRAPProj::SvDataS(char* _filenm, int _iexp, int _idat)const{
	if ((_iexp<0)||(_iexp>=expnum)){ChState(-405);return false;}            //fail
	return Exp[_iexp]->SvDataS(_filenm,_idat);
}
//---------------------------------------------------------------------------
void CFRAPProj::RunNormE(tnorm _meth, int _iexp){
	if (_iexp>=expnum){ChState(-406);return;}																//fail
	Exp[_iexp]->RunNorm(_meth);
}
//---------------------------------------------------------------------------
void CFRAPProj::RunNormD(tnorm _meth, int _iexp, int _idat){
	if (_iexp>=expnum){ChState(-407);return;}																//fail
	CFRAPExper* E=Exp[_iexp];
	if (_idat>=E->RetDtNum()){ChState(-408);return;}												//fail
	E->RetD(_idat)->RunNorm(_meth);
}
//---------------------------------------------------------------------------
int CFRAPProj::RunAverE(int _iexp){
	if (_iexp>=expnum){ChState(-409);return -1;}														//fail
	return Exp[_iexp]->AddAver();
}
//---------------------------------------------------------------------------
void CFRAPProj::AddMem(){
	//check, if expnum is in range of mexpnum, otherwise - resize mexpnum
	if ((expnum+1)>mexpnum){
		mexpnum+=5;
		Exp=(CFRAPExper**)realloc(Exp,sizeof(CFRAPExper**)*mexpnum);
		if (Exp==NULL){ChState(-410);return;}																	//fail
	}
	expnum++;
	expcnt++;
}
//---------------------------------------------------------------------------
