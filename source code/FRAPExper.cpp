//---------------------------------------------------------------------------
#pragma hdrstop

#include "FRAPExper.h"
#include "math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//------------------------class CFRAPExper-----------------------------------
CFRAPExper::CFRAPExper(char* _nm){
	int i;
	if (_nm!=NULL)
		SetNm(_nm);
	else
		SetNm(exprnm);

	datnum=0;
	mdatnum=10;
	FDataS=(CFRAPData**)malloc(sizeof(CFRAPData**)*mdatnum);
	for (i=0;i<mdatnum;i++)
		FDataS[i]=NULL;
	Md=NULL;
}
//---------------------------------------------------------------------------
CFRAPExper::~CFRAPExper(){
	for (int i=0;i<datnum; i++)
		delete FDataS[i];
	free(FDataS);
	if (Md!=NULL)
		delete Md;
}
//---------------------------------------------------------------------------
bool CFRAPExper::SvDataS(char* _filenm,int _idat)const{
	if ((_idat<0)||(_idat>=datnum)){ChState(-301); return false;}						//fail
	return FDataS[_idat]->SvData(_filenm);
}
//---------------------------------------------------------------------------
int CFRAPExper::AddNewD(){
	AddMem();
	if (state<1) return -1;
	FDataS[datnum-1]=new CExpData();
	if(Md!=NULL){
		CModPar mpar=Md->DefParVal();
		FDataS[datnum-1]->SetPar(mpar);
	}
	return (datnum-1);
}
//---------------------------------------------------------------------------
int CFRAPExper::AddNewDFl(char* _filenm){
	AddMem();
	//if (state<1) return -1;
	FDataS[datnum-1]=new CExpData(_filenm);
	if(Md!=NULL){
		CModPar mpar=Md->DefParVal();
		FDataS[datnum-1]->SetPar(mpar);
	}
	return (datnum-1);
}
//---------------------------------------------------------------------------
int CFRAPExper::AddD(CFRAPData* _dt){
	AddMem();
	//if (state<1) return -1;
	FDataS[datnum-1]=_dt;
	return (datnum-1);
}
//---------------------------------------------------------------------------
void CFRAPExper::DelD(int _pos){
	delete FDataS[_pos];
	for (int i=_pos+1;i<datnum;i++)
		FDataS[i-1]=FDataS[i];
	FDataS[datnum-1]=NULL;
	datnum--;
}
//---------------------------------------------------------------------------
void CFRAPExper::SetInd(int _fr, int _ref, int _base){
	for (int i=0; i<datnum; i++)
		if (FDataS[i]->RetType()==expd){
			FDataS[i]->SetInd(ifrap,_fr);
			FDataS[i]->SetInd(iref,_ref);
			FDataS[i]->SetInd(ibase,_base);
		}
}
//---------------------------------------------------------------------------
void CFRAPExper::SetTBl(double _t){
	for (int i=0; i<datnum; i++)
		if (FDataS[i]->RetType()==expd)
			FDataS[i]->SetTBl(_t);
}
//---------------------------------------------------------------------------
void CFRAPExper::SetMBl(int _pt){
	for (int i=0; i<datnum; i++)
		if (FDataS[i]->RetType()==expd)
			FDataS[i]->SetMBl(_pt);
}
//---------------------------------------------------------------------------
void CFRAPExper::RunNorm(tnorm _meth){
	for (int i=0;i<datnum;i++)
		if (FDataS[i]->RetType()==expd)
			FDataS[i]->RunNorm(_meth);
}
//---------------------------------------------------------------------------
int CFRAPExper::AddAver(void){
	int npar=0;
	int iaver=0;
	doubleptr* IArr=new double*[datnum];
	CFRAPData* D;
	CAvData* Da;
	double* time=NULL;
	int points=INT_MAX;
	double tbleach;
	//check indexes and calculate number of averages
	for (int i=0; i<datnum;i++){
		if (FDataS[i]->RetType()!=expd)
			continue;
		D=FDataS[i];
		IArr[iaver]=D->RetIntArr(D->RetInd(inorm));
		if (D->RetPtNum()<points)
			points=D->RetPtNum();
		if (time==NULL){
			time=D->RetTm();
			tbleach=D->RetTBl();
		}
		iaver++;
	}
	//create and fill dataset
	if (iaver<1){
		delete [] IArr;
		return -1;
	}
	if (Md!=NULL)
    npar=Md->RetPrNum();
	Da=new CAvData(IArr,iaver, points, time, tbleach,npar);
	delete [] IArr;
	if (state<1)
		return -1;																														//fail
  Da->SetNm("AVERAGE");
	//put dataset to the experiment
	AddMem();
	FDataS[datnum-1]=Da;
	return (datnum-1);
}
//---------------------------------------------------------------------------
tmodl CFRAPExper::RetModTp(){
	if (Md==NULL)
		return undef;
	else
		return Md->RetModTp();
}
//---------------------------------------------------------------------------
FRAPMod* CFRAPExper::SetModl(tmodl _ind){
	int i;
	if (Md!=NULL)
		delete Md;
	Md=createmodel (_ind);
	if (Md==NULL){ChState(-302); return NULL;}					 				 						//fail
	int n= Md->RetPrNum();
	double* parz=new double[n];
	for (i=0;i<n;i++)
		parz[i]=0;

	for (int i=0; i<datnum; i++)
		FDataS[i]->SetCPar(n,parz);

	delete parz;
	return Md;
}
//---------------------------------------------------------------------------
void CFRAPExper::RunSim(double* _par, int _idat){
	if ((Md==NULL)||(datnum<1)){ChState(-303); return;}					 						//fail
	if (_idat==-1)
		for (int i=0;i<datnum;i++){
			Md->SetTBl(FDataS[i]->RetTBl());
			Md->evaltarray(FDataS[i]->RetTm(),FDataS[i]->RetPtNum(),_par,FDataS[i]->RetIntArr(FDataS[i]->RetInd(isiml)));
			FDataS[i]->SetCPar(Md->RetPrNum(),_par);
			FDataS[i]->EvalResid();
		}
	else{
		Md->SetTBl(FDataS[_idat]->RetTBl());
		Md->evaltarray(FDataS[_idat]->RetTm(),FDataS[_idat]->RetPtNum(),_par,FDataS[_idat]->RetIntArr(FDataS[_idat]->RetInd(isiml)));
		FDataS[_idat]->SetCPar(Md->RetPrNum(),_par);
		FDataS[_idat]->EvalResid();
	}
}
//---------------------------------------------------------------------------
void CFRAPExper::SvParam(char* _filenm, int _idat){
	if (Md==NULL){ChState(-304);return;}				 											 			//fail
	ofstream o_stream;
	int i,j;
	const int parnum=Md->RetPrNum();
	double* parr;
	o_stream.open(_filenm);
	if (!o_stream.good()){ChState(-305);return;} 											 			//fail

	//header line
	o_stream<<"Data Name";
	for (j=0; j<parnum; j++)
		o_stream<<'\t'<<Md->RetParLb(j)<<", "<<Md->RetParUn(j);

	//data lines
	ParToStream(o_stream,_idat,false);
	if (!o_stream.good()){ChState(-306);return;} 											 			//fail
	o_stream.close();
}
//---------------------------------------------------------------------------
void CFRAPExper::ParToStream(ofstream & _ostr, int _idat, bool _condnm,
														 bool _biter, bool _bcrit){
	int i,j;
	const int parnum=Md->RetPrNum();
	double* parr;
	//data lines
	if (_idat==-1)
		for (i=0;i<datnum;i++){
			_ostr<<'\n'<<FDataS[i]->RetNm();
			parr=FDataS[i]->RetPPar();
			for (j=0; j<parnum; j++)
				_ostr<<'\t'<<parr[j];
			if (_biter)
				_ostr<<'\t'<<FDataS[i]->RetIter() ;
			if (_bcrit)
				_ostr<<'\t'<<FDataS[i]->RetCrit();
			if (_condnm)
				_ostr<<'\t'<<RetNm();
		}
	else{
		_ostr<<'\n'<<FDataS[_idat]->RetNm();
		parr=FDataS[_idat]->RetPPar();
		for (j=0; j<parnum; j++)
			_ostr<<'\t'<<parr[j];
			if (_biter)
				_ostr<<'\t'<<FDataS[_idat]->RetIter() ;
			if (_bcrit)
				_ostr<<'\t'<<FDataS[_idat]->RetCrit();
		if (_condnm)
			_ostr<<'\t'<<RetNm();
	}
}
//---------------------------------------------------------------------------
void CFRAPExper::FitExper(double* _par,int* _vpar,tfitmeth _fmet, bool _fitall,
													int	_stp, int _finp,int _miternum){
	if ((Md==NULL)||(datnum<1)){ChState(-307); return;}					 						//fail
	for (int i=0;i<datnum;i++)
		FitDataS(_par,_vpar,_fmet,_fitall,_stp,_finp,_miternum,i);
}
//---------------------------------------------------------------------------
void CFRAPExper::FitDataS(double* _par,int* _vpar,tfitmeth _fmet, bool _fitall,
													int	_stp, int _finp,int _miternum, int _idat){
	if ((Md==NULL)||(datnum<1)){ChState(-308); return;}					 						//fail
	if ((_idat<0)||(_idat>=datnum)){ChState(-309); return;}			 						//fail
 //!!!!check for correct point indexes _stp     _finp
	CFRAPData* fd;
	const int par_num=Md->RetPrNum();
	double fval(0);
	int iter_num(0);

	fd=FDataS[_idat];
	if (_fitall){
		_stp=0;
		_finp=fd->RetPtNum()-1;
	}
	Md->SetTBl(fd->RetTBl());
	SetFitDat(_stp,_finp,fd->RetTm(),fd->RetIntArr(fd->RetInd(inorm)));
	fd->SetCPar(Md->RetPrNum(),_par);//fill model parameters associated with a dataset with initial guesses
	RunFit(_fmet,par_num,fd->RetPPar(),_vpar,iter_num,_miternum,fval,NULL);//use parameters from the dataset
	fd->SetFitAttr(iter_num,fval); //fd->SetCPar(Md->RetPrNum(),_par, NULL, iter_num, fval);
	Md->evaltarray(fd->RetTm(),fd->RetPtNum(),fd->RetPPar(),fd->RetIntArr(fd->RetInd(isiml)));
	fd->EvalResid();
}
//---------------------------------------------------------------------------
double CFRAPExper::FitFunc(double _x, double* _par){
	if (Md==NULL){ChState(-310); return 0;}											 						//fail
	return Md->evaltpoint(_x,_par);
}
//---------------------------------------------------------------------------
void CFRAPExper::FitFunc(double* _par, int _point_num, double* _x, double* _res){
	if (Md==NULL){ChState(-311); return;}												 						//fail
	Md->evaltarray(_x,_point_num,_par,_res);
}
//---------------------------------------------------------------------------
int CFRAPExper::InRange(double* _par)const{
	if (Md==NULL){ChState(-312); return -1;}										 						//fail
	return Md->InRange(_par);
}
//---------------------------------------------------------------------------
double CFRAPExper::FindPenalty(double* _par,double* _bod_par)const{
	if (Md==NULL){ChState(-313); return infin;}									 						//fail
	return Md->FindPenalty(_par,_bod_par);
}
//---------------------------------------------------------------------------
void CFRAPExper::EvDerr(double _x, double* _par, double* _derr){
	if (Md==NULL){ChState(-314); return;}												 						//fail
	if (Md->evalderr(_x,_par,_derr)==0)
			Md->numderr(_x,_par,_derr);
}
//---------------------------------------------------------------------------
void CFRAPExper::AddMem(){
	//check, if datnum is in range of mdatnum, otherwise - resize mdatnum
	if ((datnum+1)>mdatnum){
		mdatnum+=10;
		FDataS=(CFRAPData**)realloc(FDataS,sizeof(CFRAPData**)*mdatnum);
		if (FDataS==NULL){ChState(-315); return;}												 			//fail
	}
	datnum++;
}
