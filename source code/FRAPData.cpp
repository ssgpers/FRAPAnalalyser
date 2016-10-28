//---------------------------------------------------------------------------
#pragma hdrstop

#include "FRAPData.h"
#include <math.h>
#include <fstream.h>                                
#include "mathappllib.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
//-----------------------class CFRAPData-------------------------------------
CFRAPData::CFRAPData(int _inum, int _pnum, int _npar):MPar(_npar){
	if ((_inum>0)&&(_pnum>0))
		AllocMem( _inum, _pnum);
	else{
		point_num=0;
		intens_num=0;
		time= NULL;
		IntNames=NULL;
		Intens=NULL;
	}

	tbl=0;
	mbl=0;
	cdat=undefd;

	cnorm=-1;
	csiml=-1;
	cresd=-1;

}
//---------------------------------------------------------------------------
CFRAPData::CFRAPData(char* _filenm, int _adint, int _npar):MPar(_npar){
	if (_filenm==""){ChState(-102);return;}														 			//fail

	bool bltitle;
	int pnum=0;
	int inum=0;
	double dnum;
	const int stlen=1000;
	int i;
	int j;
	char ch;
	char st[stlen];

	ifstream expfile(_filenm);

	//find object dimension
	pnum=0;
	expfile.clear();
	if (!expfile.good()){ChState(-103);return;}															//fail

	expfile>>dnum;
	bltitle=!expfile.good();
	if (bltitle){
		expfile.clear();
		expfile.getline(st,stlen,'\n');
	}
	expfile.getline(st,stlen,'\n');
	i=0;
	do{
		ch=st[i];
		i++;
		if ((ch==' ')||(ch=='\t'))
			inum++;
	}while (ch!='\0');

	pnum++;
	st[0]='\b';
	while ((!expfile.eof())&&(st[0]!='\0')&&(st[0]!='\t')){
		expfile.getline(st,stlen,'\n');
		pnum++;
	}
	if((st[0]=='\0')||(st[0]==32)||(st[0]=='\t'))
		pnum--;

	//create structures
	AllocMem(inum+_adint,pnum);       //reserve adint additional columns

	//load values from file
	expfile.clear();
	expfile.seekg(0);
	if (bltitle){
		expfile.getline(st,stlen,'\n');
		SetNames(st);
	}
	else
		SetNames();

	for(i=0; i<point_num;i++){
		expfile>>time[i];
		for(j=0;j<inum;j++)
			expfile>>Intens[j][i];
	}
	SetPNm(_filenm);
	if (!(expfile.good()||expfile.eof())){ChState(-104);return;}						//fail
	expfile.close();

	tbl=0;
	mbl=0;
}
//---------------------------------------------------------------------------
CFRAPData::CFRAPData(const CFRAPData& Source){
	if ((Source.intens_num>0)&&(Source.point_num>0)){
		AllocMem(Source.intens_num, Source.point_num);
		time=new double[point_num];
		for (int i=0; i<point_num; i++)
			time[i]=Source.time[i];
		for (int i=0;i<intens_num;i++){
			StrLCopy(IntNames[i],Source.IntNames[i],LenIntNm);
			for (int j=0;j<intens_num;j++)
				Intens[i][j]=Source.Intens[i][j];
		}
	}

	RenCopy(Source);

	tbl=Source.tbl;
	mbl=Source.mbl;
	cdat=Source.cdat;
	cnorm=Source.cnorm;
	csiml=Source.csiml;
	cresd=Source.cresd;
	MPar=Source.MPar;
}
//---------------------------------------------------------------------------
CFRAPData::~CFRAPData(){
	if (time!=NULL)
		delete [] time;
	for(int i=0; i<intens_num;i++){
		delete [] Intens[i];
		delete [] IntNames[i];
	}
	delete [] Intens;
	delete [] IntNames;
}
//---------------------------------------------------------------------------
void CFRAPData::ChangeDim(int _inum, int _pnum){
	int i,j;

	if (_inum!=intens_num) {
		doubleptr* temp=new double*[intens_num];
		if (_inum<intens_num){
			for (int i=0; i < _inum; i++)
				temp[i]=Intens[i];
		}
		else{
			for (int i=0; i<intens_num; i++)
				temp[i]=Intens[i];
			for (int i=intens_num; i < _inum; i++){
				temp[i]=new double[point_num];
				for (j=0; j<point_num; j++)
					temp[i][j]=0;
			}
		}
		delete [] Intens;
		Intens=temp;
		intens_num=_inum;
	}
	if (_pnum!=point_num){
		if (_pnum<point_num){
			doubleptr temp=new double[_pnum];
			doubleptr temp2;
			for (j=0; j<_pnum; j++)
				temp[j]=time[j];
			delete [] time;
			time=temp;
			for (int i=0; i<intens_num; i++){
				temp=new double[_pnum];
				temp2=Intens[i];
				for (j=0; j<_pnum; j++)
					temp[j]=temp2[j];
			delete [] temp2;
			Intens[i]=temp;
			}
			point_num=_pnum;
		}
		else {ChState(-105);return;}																					//fail
	}
}
//---------------------------------------------------------------------------
void CFRAPData::SetTBl(double _t){
	if (time==NULL){ChState(-106);return;}																	//fail
	if ((_t<time[0])||(_t>time[point_num-1])){ChState(-107);return;}				//fail

	tbl=_t;
	int i=0;
	while (time[i]<tbl)
		i++;
	mbl=i;
}
//---------------------------------------------------------------------------
void CFRAPData::SetMBl(int _pt){
	if (time==NULL){ChState(-108);return;}																	//fail
	if ((_pt<0)||(_pt>point_num-1)){ChState(-109);return;}									//fail

	mbl=_pt;
	tbl=time[mbl];
}
//---------------------------------------------------------------------------
void CFRAPData::SetCPar(int _n, double* _par, double* _var, int _iter, double _crit){
	MPar.SetPar(_n,_par,_var,_iter,_crit);
}
//---------------------------------------------------------------------------
void CFRAPData::SetFitAttr(int _iter, double _crit){
	MPar.SetFitAttr(_iter,_crit);
}
//---------------------------------------------------------------------------
void CFRAPData::CopyTmArr(double* _tm) const{
	if (_tm==NULL){ChState(-110);return;}																		//fail
	if (time==NULL){ChState(-111);return;}																 	//fail

	for (int i=0; i<point_num;i++)
		_tm[i]=time[i];
}
//---------------------------------------------------------------------------
void CFRAPData::CopyIntArr(double* _dest, int _iind) const{
	if (_dest==NULL){ChState(-112);return;}											 						//fail
	if ((_iind<0)||(_iind>=intens_num)){ChState(-113);return;}          		//fail

	double* sour=Intens[_iind];
	for (int i=0; i<point_num;i++)
		_dest[i]=sour[i];
}
//---------------------------------------------------------------------------
double* CFRAPData::RetIntArr(int _iind) const{
	if ((_iind<0)||(_iind>=intens_num)){
		ChState(-114);return NULL;
	}					//fail
	return Intens[_iind];
}
//---------------------------------------------------------------------------
char* CFRAPData::RetIntNm(int _iind) const{
	if ((_iind>=intens_num)){ChState(-115);return NULL;}										//fail
	return IntNames[_iind];
}
//---------------------------------------------------------------------------
bool CFRAPData::SvData(char* _filenm){
	ofstream o_stream;
	int i,j;
	o_stream.open(_filenm);
	if (!o_stream.good()){ChState(-116);return false;}								 			//fail

	//header line
	o_stream<<"Time,s";
	for (j=0; j<intens_num; j++)
		o_stream<<'\t'<<IntNames[j];

	//data lines
	for (i=0;i<point_num;i++){
		o_stream<<'\n'<<time[i];
		for (j=0; j<intens_num; j++)
			o_stream<<'\t'<<Intens[j][i];
	}

	if (!o_stream.good()){ChState(-117);return false;}								 			//fail
	o_stream.close();
	return true;
}
//---------------------------------------------------------------------------
void CFRAPData::EvalResid(){
	if ((cnorm<0)||(cnorm>=intens_num)||(csiml<0)||(csiml>=intens_num)||(cresd<0)||(cresd>=intens_num))
		{ChState(-118);return;}																								//fail

	const double*	FSig=Intens[cnorm];
	double*	FRes=Intens[cresd];
	const double*	ISig=Intens[csiml];

	for (int i=0;i<point_num;i++)
		FRes[i]=FSig[i]-ISig[i];
}
//---------------------------------------------------------------------------
void CFRAPData::SetNames(char* _st){
	int i,j=0;
	char* bg=_st;
	char bufnm[LenIntNm];
	if (_st!=NULL){
		while (bg[j]!='\t')
			j++;
		for (i=0; i<intens_num-nfit-1; i++){
			bg+=j+1;
			j=0;
			while ((bg[j]!='\t')&&(bg[j]!='\0'))
				j++;
			StrLCopy(IntNames[i],bg,j);
		}
	}
	else
		for (i=0; i<intens_num-nfit-1; i++){
			strcpy(IntNames[i],mesrnm);
			strcat(IntNames[i],itoa(i+1,bufnm,10));
		}
	strcpy(IntNames[intens_num-nfit-1],normnm);
	return;
}
//---------------------------------------------------------------------------
void CFRAPData::AllocMem(int _inum, int _pnum){
	intens_num=_inum;
	point_num=_pnum;

	if((_inum<1)||(_pnum<1)){ChState(-119);return;}										      //fail

	IntNames=new char*[intens_num];
	Intens=new double*[intens_num];
	time=new double[point_num];
	for (int i=0;i<intens_num;i++){
		 IntNames[i]=new char[LenIntNm];
		 Intens[i]=new double[point_num];
	}
}
//---------------------------------------------------------------------------
//------------------------class CExpData-------------------------------------
CExpData::CExpData(void):CFRAPData(){
	eint_num=0;
	cdat=expd;

	cfrap=-1;
	cref=-1;
	cbase=-1;
}
//---------------------------------------------------------------------------
CExpData::CExpData(char* _filenm):CFRAPData(_filenm, nfit+1){

	cfrap=-1;
	cref=-1;
	cbase=-1;

	cdat=expd;
	eint_num=intens_num-nfit-1;
	cnorm=eint_num;
	csiml=eint_num+1;
	cresd=eint_num+2;

	StrLCopy(IntNames[cnorm],normnm,LenIntNm);
	StrLCopy(IntNames[csiml],simlnm,LenIntNm);
	StrLCopy(IntNames[cresd],resdnm,LenIntNm);

	strcpy(IntNames[csiml],simlnm);
	strcpy(IntNames[cresd],resdnm);
	for (int i=0; i<point_num; i++){
		Intens[cnorm][i]=0;
		Intens[csiml][i]=0;
		Intens[cresd][i]=0;
	}
}
//---------------------------------------------------------------------------
CExpData::CExpData(const CExpData& Source):CFRAPData(Source){
	if (Source.cdat!=expd){ChState(-120);return;}									 					//fail
	cdat=expd;
	eint_num=Source.eint_num;
	cfrap=Source.cfrap;
	cref=Source.cref;
	cbase=Source.cbase;
}
//---------------------------------------------------------------------------
CExpData::~CExpData(){}
//---------------------------------------------------------------------------
void CExpData::RunNorm(tnorm _meth){
	if (cdat!=expd){ChState(-121);return;}																	//fail

	switch (_meth){
		case doub: DoubleNorm(); break;
		case sing: SingleNorm(); break;
		case fulc: FullScaleCalibr();	break;
		case nonorm: NoNorm(); break;
		case beadn: BeadNorm(); break;
		default:{ChState(-122);return;}																				//fail
	}
	//EvalResid();
}
//---------------------------------------------------------------------------
void CExpData::SetInd(int _roi, int _ind){
	if (cdat!=expd){ChState(-123);return;}																	//fail
	if (_ind==-1)	return;					//roi is underfined
	if ((_ind<0)||(_ind>=eint_num)){ChState(-124);return;} 									//fail

	switch (_roi){
		case ifrap: cfrap=_ind;break;
		case iref:	cref=_ind; break;
		case ibase: cbase=_ind;break;
		default: {ChState(-125);return;}
	}
}
//---------------------------------------------------------------------------
int CExpData::RetInd(tcol _roi)const{
	switch (_roi){
		case ifrap: return cfrap;
		case iref: return cref;
		case ibase: return cbase;
		case inorm: return cnorm;
		case isiml: return csiml;
		case iresd: return cresd;
		default: {ChState(-126);return -1;}
	}
}
//---------------------------------------------------------------------------
void CExpData::DoubleNorm(void){
	if((cfrap<0)||/*(cbase<0)||*/(cref<0)){ChState(-127);return;}						//fail
	if (mbl<1){ChState(-128);return;}																				//fail

	double whole_pre=PreIntens(cref);
	double frap_pre=PreIntens(cfrap);
	int i;
	double* InNorm=Intens[cnorm];
	double* InFrap=Intens[cfrap];
	double* InRef=Intens[cref];

	if (cbase<0)
		for(i=0; i<point_num; i++)
			InNorm[i]=whole_pre*InFrap[i]/InRef[i]/frap_pre;
	else{
		double* InBase=Intens[cbase];
		for(i=0; i<point_num; i++)
			InNorm[i]=whole_pre*(InFrap[i]-InBase[i])/(InRef[i]-InBase[i])/frap_pre;
	}
}
//---------------------------------------------------------------------------
void CExpData::SingleNorm(void){
	if((cfrap<0)/*||(cbase<0)*/){ChState(-129);return;}											//fail
	if (mbl<1){ChState(-130);return;} 																			//fail
	double frap_pre=PreIntens(cfrap);
	int i;
	double* InNorm=Intens[cnorm];
	double* InFrap=Intens[cfrap];

	if (cbase<0)
		for(i=0; i<point_num; i++)
			InNorm[i]=InFrap[i]/frap_pre;
	else{
		double* InBase=Intens[cbase];
		for(i=0; i<point_num; i++)
			InNorm[i]=(InFrap[i]-InBase[i])/frap_pre;
	}
}
//---------------------------------------------------------------------------
void CExpData::NoNorm(void){
	if(cfrap<0){ChState(-131);return;} 																			//fail

	double* InNorm=Intens[cnorm];
	double* InFrap=Intens[cfrap];

	for(int i=0; i<point_num; i++)
		InNorm[i]=InFrap[i];
}
//---------------------------------------------------------------------------
void CExpData::FullScaleCalibr(void){
	int i;
	double* InNorm=Intens[cnorm];
	const double raw_bleach=InNorm[mbl];
	double zn=0;

	for(i=0; i<mbl; i++)
		zn+=InNorm[i];
	zn=zn/mbl-raw_bleach;

	for(i=0; i<point_num; i++)
		InNorm[i]=(InNorm[i]-raw_bleach)/zn;
}
//---------------------------------------------------------------------------
void CExpData::PostToUn(void){
	int i;
	double* InNorm=Intens[cnorm];
	double s=0;
	for (i=point_num-10; i<point_num;i++)
		s+=InNorm[i];
	if (s<=0){ChState(-132);return;} 																				//fail

	s/=10;
	for (i=mbl; i<point_num; i++)
		InNorm[i]/=s;
}
//---------------------------------------------------------------------------
void CExpData::BeadNorm(void){
	if((cfrap<0)||/*(cbase<0)||*/(cref<0)){ChState(-127);return;}						//fail
	if (mbl<1){ChState(-128);return;}																				//fail

	double ref_pre=PreIntens(cref);
	double frap_pre=PreIntens(cfrap);
	int i;
	double* InNorm=Intens[cnorm];
	double* InFrap=Intens[cfrap];
	double* InRef=Intens[cref];

	//if (cbase<0)
		for(i=0; i<point_num; i++)
			InNorm[i]= 1-InRef[i]/ref_pre+InFrap[i]/frap_pre;
	//else{
		//double* InBase=Intens[cbase];
		//for(i=0; i<point_num; i++)
			//InNorm[i]=whole_pre*(InFrap[i]-InBase[i])/(InRef[i]-InBase[i])/frap_pre;
	//}

}
//---------------------------------------------------------------------------
double CExpData::PreIntens(int _croi){
	int i;
	double res=0;
	double* InRoi=Intens[_croi];
	if (cbase<0)
		for (i=0; i<mbl; i++)
			res+=InRoi[i];
	else{
		double* InBase=Intens[cbase];
		for (int i=0; i<mbl; i++)
			res+=InRoi[i]-InBase[i];
	}
	res=res/mbl;
	return res;
}
//---------------------------------------------------------------------------
//-------------------------class CAvData-------------------------------------
CAvData::CAvData(doubleptr* _EInt,int _inum, int _pnum, double* _tm, double _tbl, int _npar):CFRAPData(naver+nfit,_pnum, _npar){
	cdat=avrd;
	cnorm=0;
	csdev=1;
	cserr=2;
	csiml=naver;
	cresd=naver+1;
	StrLCopy(IntNames[cnorm],signnm,LenIntNm);
	StrLCopy(IntNames[csdev],sdevnm,LenIntNm);
	StrLCopy(IntNames[cserr],serrnm,LenIntNm);
	StrLCopy(IntNames[csiml],simlnm,LenIntNm);
	StrLCopy(IntNames[cresd],resdnm,LenIntNm);

	EvalObj(_EInt,_inum,_pnum);

	for (int i=0; i<point_num; i++)
		time[i]=_tm[i];
	SetTBl(_tbl);
}
//---------------------------------------------------------------------------
CAvData::CAvData(const CAvData& Source):CFRAPData(Source){
	if (Source.cdat!=avrd){ChState(-133);return;}									 					//fail
	cdat=avrd;
	csdev=Source.csdev;
	cserr=Source.cserr;
	avnum=Source.avnum;

}
//---------------------------------------------------------------------------
CAvData::~CAvData(void){}
//---------------------------------------------------------------------------
int CAvData::RetInd(tcol _roi)const{
	switch (_roi){
		case inorm: return cnorm;
		case isdev: return csdev;
		case iserr: return cserr;
		case isiml: return csiml;
		case iresd: return cresd;
		default: {ChState(-134);return -1;}
	}
}
//---------------------------------------------------------------------------
void CAvData::EvalObj(doubleptr* _EInt,int _inum,int _pnum){
	if (_inum<1){ChState(-135);return;}																      //fail

	avnum=_inum;
	double* ASig=Intens[cnorm];
	double* ADev=Intens[csdev];
	double* AErr=Intens[cserr];
	double* ASim=Intens[csiml];
	double* ARes=Intens[cresd];
	double* ExpD;
	int i,j,k,mbo;
	for (i=0;i<_pnum;i++){
		ASig[i]=ADev[i]=ASim[i]=ARes[i]=0;
		for (k=0;k<avnum;k++)
			ASig[i]+=_EInt[k][i];
		ASig[i]/=double(avnum);
		for (k=0;k<avnum;k++){
			ExpD=_EInt[k];
			ADev[i]+=(ExpD[i]-ASig[i])*(ExpD[i]-ASig[i]);
		}
		if (avnum>1)
			ADev[i]/=double(avnum-1);
		ADev[i]=sqrt(ADev[i]);
		AErr[i]=ADev[i]/sqrt(double(avnum));
	}
}
//---------------------------------------------------------------------------
