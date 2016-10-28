//---------------------------------------------------------------------------


#pragma hdrstop

#include "FRAPModel.h"
#include "mathappllib.h"
#include <complex.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

FRAPMod* createmodel (tmodl _ind){
	FRAPMod* _md;

	switch (_ind){
		case exp1: _md=new SingExpMod(0); break;
		case exp2: _md=new DoubExpMod(0); break;
		case bnd1: _md=new OneBndModM(0);	 break;
		case bnd2: _md=new TwoBndModM(0);	 break;
		case difc: _md=new SoumpDiffMod(0);break;
		case difr: _md=new EllDiffMod(0);	 break;
		case bndd: _md=new BndDiffMod(0);	 break;
		//case plac: _md=new PolymerMod(0);	 break;
		//case bnd1m: _md=new OneBndModM(0); break;
		//case bnd2m: _md=new TwoBndModM(0); break;
		default: {ChState(-252);_md=NULL;}								 							      //fail
	}
	return _md;
}

char* retmodelname(tmodl _ind){
	switch (_ind){
		case exp1: return "One Exponential Model";
		case exp2: return "Double Exponential Model";
		case bnd1: return "One Binding State";
		case bnd2: return "Two Binding States";
		case difc: return "Diffusion Circular Spot";
		case difr: return "Diffusion Rectangular Spot";
		case bndd: return "Binding+Diffusion";
		//case plac: return "Polymerisation";
		//case bnd1m: return "One Binding State (Mikalai)";
		//case bnd2m: return "Two Binding States (Mikalai)";
		default: return "Underfined";											 							      //fail
	}
}

///////////////////////FRAPMod//////////////////////////////////////////////////

FRAPMod::FRAPMod(double _tbl, int _npar, tmodl _tp):num_par(_npar),tp(_tp){
	tbleach=_tbl;
	minv=new double[num_par];
	maxv=new double[num_par];
	defv=new double[num_par];

}

FRAPMod::~FRAPMod(){
	delete [] minv;
	delete [] maxv;
	delete [] defv;
}

double FRAPMod::evaltpoint(double t, CModPar& _mp){
	if(_mp.RetNPar()!=num_par){ChState(-210);return 0;}	 							      //fail
	return evaltpoint(t,_mp.RetPPar());
}

void FRAPMod::evaltarray(double* tarr,int point_num, double* par, double* resarr){
	int b=InRange(par);
	if (b==1)
		for (int i=0; i<point_num; i++)
			resarr[i]=evaltpoint(tarr[i],par);
	else
		for (int i=0; i<point_num; i++)
			resarr[i]=0;
}

void FRAPMod::evaltarray(double* tarr,int point_num, CModPar& _mp, double* resarr){
	if(_mp.RetNPar()!=num_par){ChState(-211);return;}		 							      //fail
	evaltarray(tarr,point_num,_mp.RetPPar(),resarr);
}

void FRAPMod::numderr(double t, double* par, double* der){
	double* pr;
	double r;
	double prev;
	double next;
	double v1;
	double v2;
	double step;

	pr=new double[num_par];

	for (int i=0;i<num_par;i++){
		r=par[i];
		for(int j=0;j<num_par;j++)
			pr[j]=par[j];

		if(fabs(r)<epsil2)
			step=1;
		else
			step=r/10.0;

		pr[i]=r+step;
		if (!InRangeI(pr[i],i))
			pr[i]=maxv[i];
		v1=evaltpoint(t,pr);
		pr[i]=r-step;
		if (!InRangeI(pr[i],i))
			pr[i]=minv[i];
		v2=evaltpoint(t,pr);
		next=(v1-v2)/2.0/step;

		do{
			prev=next;
			step/=2.0;
			pr[i]=r+step;
			if (!InRangeI(pr[i],i))
				pr[i]=maxv[i];
			v1=evaltpoint(t,pr);
			pr[i]=r-step;
			if (!InRangeI(pr[i],i))
				pr[i]=minv[i];
			v2=evaltpoint(t,pr);
			next=(v1-v2)/2.0/step;
		}while((fabs(next-prev)>epsil)&&step>epsil);
		der[i]=next;
	}

	delete [] pr;
}

/*void FRAPMod::DisplayPar (double* _param, TMemo* _mRep, double* _dparam)const{
	_mRep->Lines->Add("Parameters of " + AnsiString(RetModName()));
	if (_dparam!=NULL)
		for (int i=0; i<num_par; i++)
			_mRep->Lines->Add(AnsiString(RetParLb(i))+"="+FloatToStrF(_param[i],ffGeneral,2,2)+"±"+FloatToStrF(_dparam[i],ffGeneral,2,2)+" "+AnsiString(RetParUn(i)));
	else
		for (int i=0; i<num_par; i++)
			_mRep->Lines->Add(AnsiString(RetParLb(i))+"="+FloatToStrF(_param[i],ffGeneral,2,2)+" "+AnsiString(RetParUn(i)));
} */

void FRAPMod::DefParVal(double* _par)const{
	for (int i=0; i<num_par; i++)
		_par[i]=defv[i];
}

CModPar FRAPMod::DefParVal()const{
	CModPar mpar;
	mpar.SetPar(num_par,defv);
	return mpar;
}

void FRAPMod::LimParVal(double* _min, double* _max)const{
	for (int i=0; i<num_par; i++){
		_min[i]=minv[i];
		_max[i]=maxv[i];
	}
}

void FRAPMod::SetLimParVal(double* _min, double* _max){
	for (int i=0; i<num_par; i++){
		minv[i]=_min[i];
		maxv[i]=_max[i];
	}
}

void FRAPMod::chlim(double* par){
	for (int i=0; i<num_par; i++)
		if (par[i]<minv[i])
			par[i]=minv[i];
		else if (par[i]>maxv[i])
			par[i]=maxv[i];
}

int FRAPMod::InRange(double* _par)const{
	for (int i=0; i<num_par; i++)
		if ((_par[i]<minv[i])||(_par[i]>maxv[i]))
			return 0;
	return 1;
}

int FRAPMod::InRangeI(double _pr, int _i)const{
	if ((_pr<minv[_i])||(_pr>maxv[_i]))
		return 0;
	return 1;
}

int FRAPMod::FindClosBod(double* _in_par, double* _res_par)const{
	int ret=1;
	for (int i=0; i<num_par; i++){
		if ((_in_par[i]>=minv[i])&&(_in_par[i]<=maxv[i])){ //parameter is within borders
			_res_par[i]=_in_par[i];
		}
		else{//parameter is not within borders
			ret=0;
			if (_in_par[i]<minv[i])
				_res_par[i]=minv[i];
			if (_in_par[i]>maxv[i])
				_res_par[i]=maxv[i];
		}
	}
	return ret;
}

double FRAPMod::FindPenalty(double* _in_par, double* _res_par)const{
	if((_in_par==NULL)||(_res_par==NULL)){ChState(-212);return infin;}      //fail
	//calculate if parameters are in range
	if (FindClosBod(_in_par,_res_par)) return 0;//if given parameters are in a pre-defined range answer is 0
	double otvet=0;
	double ratio;
	for (int i=0; i<num_par; i++){
		ratio=fabs((_in_par[i]-_res_par[i])/(maxv[i]-minv[i]));
		if (ratio>epsil2)
			otvet+=ratio*ratio;
	}
	return otvet;
}

double FRAPMod::FindPenalty(double* _in_par)const{
	if(_in_par==NULL){ChState(-213);return infin;}										      //fail
	if (InRange(_in_par)) return 0;//if given parameters are in a pre-defined range
	double* resp=new double[num_par];
	double otvet=FindPenalty(_in_par,resp);
	delete [] resp;
	return otvet;
}

void FRAPMod::SetTBl(double _tb){
if(_tb<0){ChState(-212);return;}		 							      									//fail
	tbleach=_tb;
}
///////////////////////FRAPLpMod///////////////////////////////////////////////
FRAPLpMod::FRAPLpMod(double _tbl, int _npar, tmodl _tp):FRAPMod(_tbl,_npar,_tp){
	//for Zakian method
	//alpha=new cdcompl[5];
	//Ka=new cdcompl[5];
	alpha[0]=cdcompl(1.283767675E1, 1.666063445);
	alpha[1]=cdcompl(1.222613209E1,5.012718792);
	alpha[2]=cdcompl(1.09343031E1, 8.40967312);
	alpha[3]=cdcompl(8.77643472, 1.19218539E1);
	alpha[4]=cdcompl(5.22545336, 1.57295290E1);
	Ka[0]=cdcompl(-3.69020821E4, 1.96990426E5);
	Ka[1]=cdcompl(6.12770252E4, -9.54086255E4);
	Ka[2]=cdcompl(-2.89165629E4, 1.81691853E4);
	Ka[3]=cdcompl(4.65536114E3, -1.90152864);
	Ka[4]=cdcompl(-1.18741401E2, -1.41303691E2);
}

FRAPLpMod::~FRAPLpMod(){
	//delete [] alpha;
	//delete [] Ka;
}

double FRAPLpMod::invlp2(double t,double* par,double T,double Er,double sig0, int k_sum){
	double sigm=sig0-log(Er)/2/T;
	cdcompl sigmc(sigm,0);
	cdcompl arg;
	cdcompl r=Lapl(sigmc,par);
	double res=0;
	for (int k=1;k<=k_sum;k++){
		arg=cdcompl(sigm,k*M_PI/T);
		res=res+(real(Lapl(arg,par)))*cos(k*M_PI*t/T)-(imag(Lapl(arg,par)))*sin(k*M_PI*t/T);
	}
	res=exp(sigm*t)/T*(1.0/2.0*real(r)+res);
	return res;
}

double FRAPLpMod::invlp3(double t,double* par){
	if (t==0) t=1e-8;
	double res=0;
	for (int i=0;i<5;i++)
		res+=real(Ka[i]*Lapl(1.0/t*alpha[i],par));
	res*=2.0/t;
	return res;
}

cdcompl FRAPLpMod::invs(cdcompl arg){
	const double epsilon=1e-20;
	double re=arg.real();
	double im=arg.imag();
	if((fabs(re)<epsilon)&&(fabs(im)<epsilon))
		return cdcompl(0);
	else
		return cdcompl (re/(re*re+im*im),-im/(re*re+im*im));

}
///////////////////////SingExpMod///////////////////////////////////////////////
SingExpMod::SingExpMod(double tbl):FRAPMod(tbl,3,exp1){
	//maximal, minimal and default values for parameters
	minv[0]=-0.3;
	minv[1]=-0.3;
	minv[2]=1e-4;
	maxv[0]=1.2;
	maxv[1]=1.2;
	maxv[2]=infin;
	defv[0]=0;
	defv[1]=1;
	defv[2]=10;
}

SingExpMod::~SingExpMod(){}

double SingExpMod::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);

	if (t<tbleach)
		return 1;
	return a0+a1*(1-gexp(-(t-tbleach)/tau));  //!!!the equation is here
}

int SingExpMod::evalderr(double t, double* par,double* derr){
	if (t<tbleach)
		for(int i=0;i<num_par;i++)
			derr[i]=0;
	else{																	//!!the equations are here
		par_val(par);
		derr[0]=1;
		derr[1]=1-gexp(-(t-tbleach)/tau);
		derr[2]=-a1*gexp(-(t-tbleach)/tau)*(t-tbleach)/tau/t_half;
	}
	return 1;
}

int SingExpMod::par_val(double* pr){
	a0=pr[0];
	a1=pr[1];
	t_half=pr[2];
	tau=t_half/lg2frap;
	return 1;
}

/*int SingExpMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){
	double sm=0;
	const int aver_num=5;
	int i;
	const double bl_intens=intens[bl_ind];

	for (i=point_num-1; i>point_num-aver_num-1; i--)
		sm+=intens[i];
	a0=sm/aver_num;
	a1=bl_intens-a0;

	i=bl_ind;
	do{
		i++;
	}while(intens[i]<((bl_intens+a0)/2));
	tau1=log(2.0)/(tm[i]-tm[bl_ind]);

	pr[0]=a0;
	pr[1]=a1;
	pr[2]=tau1;
	return 1;
}*/

char* SingExpMod::RetParName(int ind)const{
	switch (ind){
		case 0: return "Unbleached fraction";
		case 1: return "Recovery amplitude";
		case 2: return "Recovery half-time";
		default: return "unidentified";
	}
}

char* SingExpMod::RetParLb(int ind)const{
	switch (ind){
		case 0: return "I[0]";
		case 1: return "I[1]";
		case 2: return "t[1/2]";
		default: return "unidentified";;
	}
}

char* SingExpMod::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "";
		case 2: return "s";
		default: return "unidentified";
	}
}
////////////////////////OneBndMod////////////////////////////////////////////////

OneBndMod::OneBndMod(double tbl):FRAPMod(tbl,3,bnd1){
	//maximal, minimal and default values for parameters
	minv[0]=0;
	minv[1]=0;
	minv[2]=1e-3;
	maxv[0]=1;
	maxv[1]=1;
	maxv[2]=infin;
	defv[0]=0;
	defv[1]=1;
	defv[2]=0.05;
}

OneBndMod::~OneBndMod(){}

double OneBndMod::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);
	if (t<tbleach)
		return 1;
	return (1-r)*(1-c_eq*gexp(-k_off*(t-tbleach)));  //!!!the equation is here
}

int OneBndMod::evalderr(double t, double* par,double* derr){
	if (t<tbleach)
		for(int i=0;i<num_par;i++)
			derr[i]=0;
	else{																	//!!the equations are here
		par_val(par);
		derr[0]=-(1-c_eq*gexp(-k_off*(t-tbleach)));
		derr[1]=-(1-r)*gexp(-k_off*(t-tbleach));
		derr[2]=(1-r)*c_eq*(t-tbleach)*gexp(-k_off*(t-tbleach));
	}
	return 1;
}

int OneBndMod::par_val(double* pr){
	r=pr[0];
	c_eq=pr[1];
	k_off=pr[2];
	return 1;
}

//int OneBndMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* OneBndMod::RetParName(int ind)const{
	switch (ind){
		case 0: return "Size ratio";
		case 1: return "Normalized concentration";
		case 2: return "Unbinding rate";
		default: return "unidentified";
	}
}


char* OneBndMod::RetParLb(int ind)const{
	switch (ind){
		case 0: return "r";
		case 1: return "c[eq]";
		case 2: return "k[off]";
		default: return "unidentified";
	}
}

char* OneBndMod::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "";
		case 2: return "s^-1";
		default: return "unidentified";;
	}
}
////////////////////////OneBndModM//////////////////////////////////////////////

OneBndModM::OneBndModM(double tbl):FRAPMod(tbl,3,bnd1){
	//maximal, minimal and default values for parameters
	minv[0]=-0.3;
	minv[1]=1e-4;
	minv[2]=1e-4;
	maxv[0]=1.2;
	maxv[1]=infin;
	maxv[2]=infin;
	defv[0]=0.3;
	defv[1]=0.1;
	defv[2]=0.5;
}

OneBndModM::~OneBndModM(){}

double OneBndModM::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);

	if (t<tbleach)
		return 1;
	return F_eq*(1+k_on/k_off*(1-gexp(-k_off*(t-tbleach))));  //!!!the equation is here
}

int OneBndModM::evalderr(double t, double* par,double* derr){
	if (t<tbleach)
		for(int i=0;i<num_par;i++)
			derr[i]=0;
	else{																	//!!the equations are here
		par_val(par);
		derr[0]=1+k_on/k_off*(1-gexp(-k_off*(t-tbleach)));
		derr[1]=F_eq/k_off*(1-gexp(-k_off*(t-tbleach)));
		derr[2]=F_eq*k_on/k_off/k_off*((k_off*(t-tbleach)+1)*gexp(-k_off*(t-tbleach))-1);
	}
	return 1;
}

int OneBndModM::par_val(double* pr){
	F_eq=pr[0];
	k_on=pr[1];
	k_off=pr[2];
	return 1;
}

//int OneBndModM::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* OneBndModM::RetParName(int ind)const{
	switch (ind){
		case 0: return "Fraction of monomers";
		case 1: return "Pseudo-binding rate";
		case 2: return "Unbinding rate";
		default: return "unidentified";
	}
}


char* OneBndModM::RetParLb(int ind)const{
	switch (ind){
		case 0: return "F[eq]";
		case 1: return "k[on]";
		case 2: return "k[off]";
		default: return "unidentified";
	}
}

char* OneBndModM::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "s^-1";
		case 2: return "s^-1";
		default: return "unidentified";;
	}
}
///////////////////////DoubleExpMod/////////////////////////////////////////////
DoubExpMod::DoubExpMod(double tbl):FRAPMod(tbl,5,exp2){
	//maximal, minimal and default values for parameters
	minv[0]=-0.3;
	minv[1]=-0.3;
	minv[2]=-0.3;
	minv[3]=1e-4;
	minv[4]=1e-4;
	maxv[0]=1.2;
	maxv[1]=1.2;
	maxv[2]=1.2;
	maxv[3]=infin;
	maxv[4]=infin;
	defv[0]=0;
	defv[1]=0.5;
	defv[2]=0.5;
	defv[3]=50;
	defv[4]=100;
}

DoubExpMod::~DoubExpMod(){}

double DoubExpMod::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);

	if (t<tbleach)
		return 1;
	return a0+a1*(1-gexp(-(t-tbleach)/tau1))+a2*(1-gexp(-(t-tbleach)/tau2));  //!!!the equation is here
}

int DoubExpMod::evalderr(double t, double* par,double* derr){
	if (t<tbleach)
		for(int i=0;i<num_par;i++)
			derr[i]=0;
	else{																	//!!the equations are here
		par_val(par);
		derr[0]=1;
		derr[1]=1-gexp(-(t-tbleach)/tau1);
		derr[2]=1-gexp(-(t-tbleach)/tau2);
		derr[3]=-a1*gexp(-(t-tbleach)/tau1)*(t-tbleach)/tau1/t_half1;
		derr[4]=-a2*gexp(-(t-tbleach)/tau2)*(t-tbleach)/tau2/t_half2;
	}
	return 1;
}

int DoubExpMod::par_val(double* pr){
	a0=pr[0];
	a1=pr[1];
	a2=pr[2];
	t_half1=pr[3];
	t_half2=pr[4];
	tau1=t_half1/lg2frap;
	tau2=t_half2/lg2frap;
	return 1;
}

/*nt DoubExpMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){
	double sm=0;
	const int aver_num=5;
	int i;
	const double bl_intens=intens[bl_ind];

	for (i=point_num-1; i>point_num-aver_num-1; i--)
		sm+=intens[i];
	a0=sm/aver_num;
	a1=a2=(bl_intens-a0)/2;

	i=bl_ind;
	do{
		i++;
	}while(intens[i]<((bl_intens+a0)/2));
	tau1=log(2.0)/(tm[i]-tm[bl_ind]);
	tau2=tau1/3;

	pr[0]=a0;
	pr[1]=a1;
	pr[2]=a2;
	pr[3]=tau1;
	pr[4]=tau2;
	return 1;
}*/

char* DoubExpMod::RetParName(int ind)const{
	switch (ind){
		case 0: return "Unbleached fraction";
		case 1: return "Recovery amplitude 1";
		case 2: return "Recovery amplitude 2";
		case 3: return "Recovery half-time 1";
		case 4: return "Recovery half-time 2";
		default: return "unidentified";
	}
}

char* DoubExpMod::RetParLb(int ind)const{
	switch (ind){
		case 0: return "I[0]";
		case 1: return "I[1]";
		case 2: return "I[2]";
		case 3: return "t[1/2,1]";
		case 4: return "t[1/2,2]";
		default: return "unidentified";
	}
}

char* DoubExpMod::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "";
		case 2: return "";
		case 3: return "s";
		case 4: return "s";
		default: return "unidentified";
	}
}
/////////////////////////TwoBndMod//////////////////////////////////////////////
TwoBndMod::TwoBndMod(double tbl):FRAPMod(tbl,5,bnd2){
	//maximal, minimal and default values for parameters
	minv[0]=0;
	minv[1]=0;
	minv[2]=0;
	minv[3]=0;
	minv[4]=0;
	maxv[0]=1;
	maxv[1]=1;
	maxv[2]=1;
	maxv[3]=infin;
	maxv[4]=infin;
	defv[0]=0;
	defv[1]=0.2;
	defv[2]=0.4;
	defv[3]=0.05;
	defv[4]=0.07;
}

TwoBndMod::~TwoBndMod(){}

double TwoBndMod::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);
	if (t<tbleach)
		return 1;
	return (1-r)*(1-c_eq1*gexp(-k_off1*(t-tbleach))-c_eq2*gexp(-k_off2*(t-tbleach)));  //!!!the equation is here
}

int TwoBndMod::evalderr(double t, double* par,double* derr){
	if (t<tbleach)
		for(int i=0;i<num_par;i++)
			derr[i]=0;
	else{																	//!!the equations are here
		par_val(par);

		derr[0]=-(1-c_eq1*gexp(-k_off1*(t-tbleach))-c_eq2*gexp(-k_off2*(t-tbleach)));
		derr[1]=-(1-r)*gexp(-k_off1*(t-tbleach));
		derr[2]=-(1-r)*gexp(-k_off2*(t-tbleach));
		derr[3]=(1-r)*c_eq1*(t-tbleach)*gexp(-k_off1*(t-tbleach));
		derr[4]=(1-r)*c_eq2*(t-tbleach)*gexp(-k_off2*(t-tbleach));
	}
	return 1;
}

int TwoBndMod::par_val(double* pr){
	r=pr[0];
	c_eq1=pr[1];
	c_eq2=pr[2];
	k_off1=pr[3];
	k_off2=pr[4];
	return 1;
}

//int TwoBndMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* TwoBndMod::RetParName(int ind)const{
	switch (ind){
		case 0: return "Size ratio";
		case 1: return "Normalized concentration1";
		case 2: return "Normalized concentration2";
		case 3: return "Unbinding rate 1";
		case 4: return "Unbinding rate 2";
		default: return "unidentified";
	}
}

char* TwoBndMod::RetParLb(int ind)const{
	switch (ind){
		case 0: return "r";
		case 1: return "c[eq1]";
		case 2: return "c[eq2]";
		case 3: return "k[off1]";
		case 4: return "k[off2]";
		default: return "unidentified";
	}
}

char* TwoBndMod::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "";
		case 2: return "";
		case 3: return "s^-1";
		case 4: return "s^-1";
		default: return "unidentified";
	}
}

/////////////////////////TwoBndModM/////////////////////////////////////////////
TwoBndModM::TwoBndModM(double tbl):FRAPMod(tbl,5,bnd2){
	//maximal, minimal and default values for parameters
	minv[0]=-0.3;
	minv[1]=0;
	minv[2]=0;
	minv[3]=1e-4;
	minv[4]=1e-4;
	maxv[0]=1.2;
	maxv[1]=infin;
	maxv[2]=infin;
	maxv[3]=infin;
	maxv[4]=infin;
	defv[0]=0.4;
	defv[1]=0.001;
	defv[2]=0.005;
	defv[3]=0.05;
	defv[4]=0.07;
}

TwoBndModM::~TwoBndModM(){}

double TwoBndModM::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);

	if (t<tbleach)
		return 1;
	return F_eq*(1+k_on1/k_off1*(1-gexp(-k_off1*(t-tbleach)))+k_on2/k_off2*(1-gexp(-k_off2*(t-tbleach))));;  //!!!the equation is here
}

int TwoBndModM::evalderr(double t, double* par,double* derr){
	if (t<tbleach)
		for(int i=0;i<num_par;i++)
			derr[i]=0;
	else{																	//!!the equations are here
		par_val(par);

		derr[0]=1+k_on1/k_off1*(1-gexp(-k_off1*(t-tbleach)))+k_on2/k_off2*(1-gexp(-k_off2*(t-tbleach)));
		derr[1]=F_eq/k_off1*(1-gexp(-k_off1*(t-tbleach)));
		derr[2]=F_eq/k_off2*(1-gexp(-k_off2*(t-tbleach)));
		derr[3]=F_eq*k_on1/k_off1/k_off1*((k_off1*(t-tbleach)+1)*gexp(-k_off1*(t-tbleach))-1);
		derr[4]=F_eq*k_on2/k_off2/k_off2*((k_off2*(t-tbleach)+1)*gexp(-k_off2*(t-tbleach))-1);
	}
	return 1;
}

int TwoBndModM::par_val(double* pr){
	F_eq=pr[0];
	k_on1=pr[1];
	k_on2=pr[2];
	k_off1=pr[3];
	k_off2=pr[4];
	return 1;
}

//int TwoBndMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* TwoBndModM::RetParName(int ind)const{
	switch (ind){
		case 0: return "Fraction of monomers";
		case 1: return "Pseudo-binding rate1";
		case 2: return "Pseudo-binding rate2";
		case 3: return "Unbinding rate 1";
		case 4: return "Unbinding rate 2";
		default: return "unidentified";
	}
}

char* TwoBndModM::RetParLb(int ind)const{
	switch (ind){
		case 0: return "F[eq]";
		case 1: return "k[on1]";
		case 2: return "k[on2]";
		case 3: return "k[off1]";
		case 4: return "k[off2]";
		default: return "unidentified";
	}
}

char* TwoBndModM::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "s^-1";
		case 2: return "s^-1";
		case 3: return "s^-1";
		case 4: return "s^-1";
		default: return "unidentified";
	}
}
///////////////////////SoumpDiffMod/////////////////////////////////////////////
SoumpDiffMod::SoumpDiffMod(double tbl):FRAPMod(tbl,4,difc){
	//maximal, minimal and default values for parameters
	minv[0]=-0.3;
	minv[1]=-0.3;
	minv[2]=1e-4;;//1e-7;
	minv[3]=1e-3;//1e-3;
	maxv[0]=1.2;
	maxv[1]=1.2;
	maxv[2]=1e3;
	maxv[3]=100;
	defv[0]=0;
	defv[1]=1;
	defv[2]=30;
	defv[3]=5;
}

SoumpDiffMod::~SoumpDiffMod(){}

double SoumpDiffMod::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);

	if (t<tbleach)
		return 1;
	else if (t>tbleach){
		tau=w*w/4/D;
		return a0+a1*gexp(-2*tau/(t-tbleach))*(bessi0(2*tau/(t-tbleach))+bessi1(2*tau/(t-tbleach)));  //!!!the equation is here
	}else
		return a0;
}

int SoumpDiffMod::evalderr(double t, double* par,double* derr){
	return 0;
}

int SoumpDiffMod::par_val(double* pr){
	a0=pr[0];
	a1=pr[1];
	D=pr[2];
	w=pr[3];
	tau=w*w/4/D;
	return 1;
}

//int SoumpDiffMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* SoumpDiffMod::RetParName(int ind)const{
	switch (ind){
		case 0: return "Unbleached fraction";
		case 1: return "Mobile fraction";
		case 2: return "Diffusion coefficient";
		case 3: return "ROI radius";
		default: return "unidentified";
	}
}

char* SoumpDiffMod::RetParLb(int ind)const{
	switch (ind){
		case 0: return "I[unbl]";
		case 1: return "I[mob]";
		case 2: return "D";
		case 3: return "r";
		default: return "unidentified";
	}
}

char* SoumpDiffMod::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "";
		case 2: return "um^2*s^-1";
		case 3: return "um";
		default: return "unidentified";
	}
}

///////////////////////EllDiffMod/////////////////////////////////////////////
EllDiffMod::EllDiffMod(double tbl):FRAPMod(tbl,4,difr){
	//maximal, minimal and default values for parameters
	minv[0]=0;
	minv[1]=0;
	minv[2]=1e-4;//-1e-1;
	minv[3]=0;//1e-3;
	maxv[0]=1;
	maxv[1]=1;
	maxv[2]=100;
	maxv[3]=1e3;
	defv[0]=0;
	defv[1]=1;
	defv[2]=3;
	defv[3]=5;
}

EllDiffMod::~EllDiffMod(){}

double EllDiffMod::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);
	if (t<tbleach)
		return 1;
	else{
		double arg=w*w/(w*w+4*M_PI*D*(t-tbleach));
		if (arg>=0)
			return a0+a1*(1-sqrt(arg));            //!!!the equation is here
		else
			return infin;
	}
}

int EllDiffMod::evalderr(double t, double* par,double* derr){
	if (t<tbleach)
		for(int i=0;i<num_par;i++)
			derr[i]=0;
	else{																	//!!the equations are here
		par_val(par);
		double arg=1/(w*w+4*M_PI*D*(t-tbleach));
		derr[0]=1.0;
		derr[1]=1.0-w*sqrt(arg);
		derr[2]=2.0*a1*M_PI*(t-tbleach)*w*arg*sqrt(arg);
		derr[3]=a1*sqrt(arg)*(w*arg-1.0);
	}
	return 1;
}

int EllDiffMod::par_val(double* pr){
	a0=pr[0];
	a1=pr[1];
	D=pr[2];
	w=pr[3];
	return 1;
}

//int EllDiffMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* EllDiffMod::RetParName(int ind)const{
	switch (ind){
		case 0: return "Unbleached fraction";
		case 1: return "Mobile fraction";
		case 2: return "Diffusion coefficient";
		case 3: return "ROI width";
		default: return "unidentified";
	}
}

char* EllDiffMod::RetParLb(int ind)const{
	switch (ind){
		case 0: return "I[unbl]";
		case 1: return "I[mob]";
		case 2: return "D";
		case 3: return "w";
		default: return "unidentified";
	}
}

char* EllDiffMod::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "";
		case 2: return "um^2*s^-1";
		case 3: return "um";
		default: return "unidentified";
	}
}

//////////////////////////WrkMod////////////////////////////////////////////////
WrkMod::WrkMod(double tbl):FRAPLpMod(tbl,0,undef){
	//maximal, minimal and default values for parameters
}

WrkMod::~WrkMod(){}

double WrkMod::evaltpoint(double t, double* par){
	return invlp3(t,par);
}

int WrkMod::evalderr(double t, double* par,double* derr){
	return 0;
}

int WrkMod::par_val(double* pr){
	return 1;
}

//int WrkMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* WrkMod::RetParName(int ind)const{
	switch (ind){
		default: return "unidentified";
	}
}

char* WrkMod::RetParLb(int ind)const{
	switch (ind){
		default: return "unidentified";
	}
}

char* WrkMod::RetParUn(int ind)const{
	switch (ind){
		default: return "unidentified";
	}
}

cdcompl WrkMod::Lapl(cdcompl s, double* par){
	return 1.0/(s*s+1.0);
}

////////////////////////BndDiffMod//////////////////////////////////////////////
BndDiffMod::BndDiffMod(double tbl):FRAPLpMod(tbl,5,bndd){
	//maximal, minimal and default values for parameters
	minv[0]=0;
	minv[1]=0;
	minv[2]=0;
	minv[3]=0;
	minv[4]=0;
	maxv[0]=1;
	maxv[1]=1e3;
	maxv[2]=1e3;
	maxv[3]=1e3;
	maxv[4]=1e5;
	defv[0]=0;
	defv[1]=0.5;
	defv[2]=0.1;
	defv[3]=30;
	defv[4]=2.0;
}

BndDiffMod::~BndDiffMod(){}

double BndDiffMod::evaltpoint(double t, double* par){
	if (!InRange(par))
		return 0;
	par_val(par);

	if (t<tbleach)
		return 1;
	if (t==tbleach)
		return (1-r)*invlp3(0.001,par);
	return (1-r)*invlp3(t-tbleach,par);             //!!!the equation is here
}

int BndDiffMod::evalderr(double t, double* par,double* derr){
	return 0;
}

int BndDiffMod::par_val(double* pr){
	r=pr[0];
	kons=pr[1];
	koff=pr[2];
	D=pr[3];
	w=pr[4];
	f_eq=koff/(kons+koff);
	c_eq=1-f_eq;

	return 1;
}

//int BndDiffMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}

char* BndDiffMod::RetParName(int ind)const{
	switch (ind){
		case 0: return "Immobile fraction";
		case 1: return "pseudo binding rate";
		case 2: return "unbinding rate";
		case 3: return "Diffusion coefficient";
		case 4: return "ROI radius";
		default: return "unidentified";
	}
}

char* BndDiffMod::RetParLb(int ind)const{
	switch (ind){
		case 0: return "I[immob]";
		case 1: return "k[ons]";
		case 2: return "k[off]";
		case 3: return "D";
		case 4: return "r";
		default: return "unidentified";
	}
}

char* BndDiffMod::RetParUn(int ind)const{
	switch (ind){
		case 0: return "";
		case 1: return "s^-1";
		case 2: return "s^-1";
		case 3: return "um^2*s^-1";
		case 4: return "um";
		default: return "unidentified";
	}
}

cdcompl BndDiffMod::Lapl(cdcompl s, double* par){
	cdcompl res;
	cdcompl arg;
	cdcompl ci0,ci1,ck0,ck1,ci0p,ci1p,ck0p,ck1p;

	par_val(par);
	arg=w*sqrt((s/D)*(1.0+(kons*invs(s+koff))));
	cbessik01(arg,ci0,ci1,ck0,ck1,ci0p,ci1p,ck0p,ck1p);
	res=(f_eq*invs(s))*(1.0-(2.0*ci1*ck1))*(1.0+(kons*invs(s+koff)));
	res=(1.0*invs(s))-res-(c_eq*invs(s+koff));
	return res;
}
///////////////////////PolymerMod///////////////////////////////////////////////
//PolymerMod::PolymerMod(double tbl):FRAPMod(tbl,6,plac){
//	//maximal and minimal values for parameters
//	minv[0]=0;//1e-5;
//	minv[1]=0;//1e-5;
//	minv[2]=0;//1e-5;
//	minv[3]=0;//1e-5;
//	minv[4]=1;
//	minv[5]=0;//1e-4;
//	maxv[0]=1e5;
//	maxv[1]=1e5;
//	maxv[2]=1e5;
//	maxv[3]=1e5;
//	maxv[4]=1000;
//	maxv[5]=1e3;
//	defv[0]=11.5;
//	defv[1]=1.3;
//	defv[2]=1.4;
//	defv[3]=8.0;
//	defv[4]=500;
//	defv[5]=1;
//}
//
//PolymerMod::~PolymerMod(){}
//
//double PolymerMod::evaltpoint(double t, double* par){
//	if (!InRange(par))
//		return 0;
//	par_val(par);
//
//	if (t<tbleach)
//		return 1;
//	if (t==tbleach)
//		return cm/(cm+cf);
//
//	resb=0;
//	resp=0;
//	res=0;
//	tau=t-tbleach;
//
//	for (int m=0; m<L;m++){
//		if (dib!=0)
//			resb=normcdf((L-m+v*tau)/sqrt(dib*2*tau))-gexp(-(L-m)*v/dib)*normcdf((m-L+v*tau)/sqrt(dib*2.0*tau));
//		else
//			resb=1;
//		if (dip!=0)
//			resp=normcdf((m+1-v*tau)/sqrt(dip*2*tau))-gexp((m+1)*v/dip)*normcdf((-(m+1)-v*tau)/sqrt(dip*2.0*tau));
//		else
//			resp=1;
//		res+=(resb/L)*resp;
//	}
//	return 1.0-cf/(cm+cf)*res;
//}
//
//int PolymerMod::evalderr(double t, double* par,double* derr){
//	return 0;
//}
//
//int PolymerMod::par_val(double* pr){
//
//	konb=pr[0];
//	konp=pr[1];
//	koffb=pr[2];
//	koffp=pr[3];
//	L=pr[4];
//	cf=pr[5];
//
//	if ((konb+konp)<epsil2)
//		return 0;
//
//	cm=(koffb+koffp)/(konb+konp);
//	v=(konb*koffp-koffb*konp)/(konb+konp);
//	dib=(2.0*konb*koffb+konb*koffp+koffb*konp)/(konb+konp);
//	dip=(2.0*konp*koffp+konb*koffp+koffb*konp)/(konb+konp);
//
//	return 1;
//}
//
////int PolymerMod::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}
//
//char* PolymerMod::RetParName(int ind)const{
//	switch (ind){
//		case 0: return "barbed association rate";
//		case 1: return "pointed association rate";
//		case 2: return "barbed disassociation rate";
//		case 3: return "pointed disassociation rate";
//		case 4: return "filament length";
//		case 5: return "F-actin concentration";
//		default: return "unidentified";
//	}
//}
//
//char* PolymerMod::RetParLb(int ind)const{
//	switch (ind){
//		case 0: return "k[+]^b";
//		case 1: return "k[+]^p";
//		case 2: return "k[-]^b";
//		case 3: return "k[-]^p";
//		case 4: return "L";
//		case 5: return "c[f]";
//		default: return "unidentified";
//	}
//}
//
//char* PolymerMod::RetParUn(int ind)const{
//	switch (ind){
//		case 0: return "uM^-1*s^-1";
//		case 1: return "uM^-1*s^-1";
//		case 2: return "s^-1";
//		case 3: return "s^-1";
//		case 4: return "sub";
//		case 5: return "uM";
//		default: return "unidentified";
//	}
//}
/////////////////////////PolymerMod2//////////////////////////////////////////////
//PolymerMod2::PolymerMod2(double tbl):FRAPMod(tbl,6,undef){
//	//maximal and minimal values for parameters
//	minv[0]=0;//1e-5;
//	minv[1]=0;//1e-5;
//	minv[2]=0;//1e-5;
//	minv[3]=0;//1e-5;
//	minv[4]=1;
//	minv[5]=0;//1e-4;
//	maxv[0]=1e5;
//	maxv[1]=1e5;
//	maxv[2]=1e5;
//	maxv[3]=1e5;
//	maxv[4]=1000;
//	maxv[5]=1e5;
//	defv[0]=3.0;
//	defv[1]=4.0;
//	defv[2]=1.;
//	defv[3]=8.0;
//	defv[4]=500;
//	defv[5]=10.;
//}
//
//PolymerMod2::~PolymerMod2(){}
//
//double PolymerMod2::evaltpoint(double t, double* par){
//	if (!InRange(par))
//		return 0;
//	par_val(par);
//
//
//
//	if (t<tbleach)
//		return 1;
//	if (t==tbleach)
//		return cm/(cm+cf);
//
//	resb=0;
//	resp=0;
//	res=0;
//	tau=t-tbleach;
//
//	if ((2*dib*tau)<0)
//		Application->MessageBox(AnsiString(dib).c_str(), "Negative Db", MB_OK);
//	if ((2*dip*tau)<0)
//		Application->MessageBox(AnsiString(dip).c_str(), "Negative Dp", MB_OK);
//	//double a=sqrt (2*dib*tau);
//	//double b=sqrt (2*dip*tau);
//
//	for (int m=0; m<L;m++){
//		if (dib!=0)
//			resb=normcdf((L-m+v*tau)/sqrt(dib*2*tau))-gexp(-(L-m)*v/dib)*normcdf((m-L+v*tau)/sqrt(dib*2.0*tau));
//		else
//			resb=1;
//		if (dip!=0)
//			resp=normcdf((m+1-v*tau)/sqrt(dip*2*tau))-gexp((m+1)*v/dip)*normcdf((-(m+1)-v*tau)/sqrt(dip*2.0*tau));
//		else
//			resp=1;
//		res+=(resb/L)*resp;
//	}
//	return 1.0-cf/(cm+cf)*res;
//}
//
//int PolymerMod2::evalderr(double t, double* par,double* derr){
//	return 0;
//}
//
//int PolymerMod2::par_val(double* pr){
//
//	dib=pr[0];
//	dip=pr[1];
//	v=pr[2];
//	cm=pr[3];
//	L=pr[4];
//	cf=pr[5];
//
//	return 1;
//}
//
////int PolymerMod2::threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr){}
//
//char* PolymerMod2::RetParName(int ind)const{
//	switch (ind){
//		case 0: return "barbed diffusivity coefficient";
//		case 1: return "pointed diffusivity coefficient";
//		case 2: return "polymerisation rate";
//		case 3: return "G-actin concentration";
//		case 4: return "filament length";
//		case 5: return "F-actin concentration";
//		default: return "unidentified";
//	}
//}
//
//char* PolymerMod2::RetParLb(int ind)const{
//	switch (ind){
//		case 0: return "D[b]";
//		case 1: return "D[p]";
//		case 2: return "v";
//		case 3: return "c[m]";
//		case 4: return "L";
//		case 5: return "c[f]";
//		default: return "unidentified";
//	}
//}
//
//char* PolymerMod2::RetParUn(int ind)const{
//	switch (ind){
//		case 0: return "sub^2*s^-1";
//		case 1: return "sub^2*s^-1";
//		case 2: return "sub*s^-1";
//		case 3: return "uM";
//		case 4: return "sub";
//		case 5: return "uM";
//		default: return "unidentified";
//	}
//}
