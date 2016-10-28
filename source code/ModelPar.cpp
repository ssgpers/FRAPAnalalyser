//---------------------------------------------------------------------------


#pragma hdrstop

#include "ModelPar.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
///////////////////////CModPar//////////////////////////////////////////////////
CModPar::CModPar(int _npar):num_par(_npar),dv(false),niter(0),crit(0){
	if (_npar<0){ChState(-201);return;}																			//fail
	if (num_par==0){
		par=NULL;
		dev=NULL;
	}
	else{
		par=new double[num_par];
		for (int i=0; i<num_par;i++){
			par[i]=0;
		}
		if (dv){
			dev=new double[num_par];
			for (int i=0; i<num_par;i++){
				dev[i]=0;
			}
		}
		else{
      dev=NULL;
    }
	}
}
//---------------------------------------------------------------------------
CModPar::CModPar(const CModPar& Source):num_par(Source.num_par),dv(Source.dv),niter(Source.niter),crit(Source.crit){
	if (Source.par!=NULL){
		par=new double[num_par];
		for (int i=0; i<num_par; i++)
			par[i]=Source.par[i];
	}
	else
		par=NULL;
	if (Source.dev!=NULL){
		dev=new double[num_par];
		for (int i=0; i<num_par; i++)
			dev[i]=Source.dev[i];
	}
	else
		dev=NULL;
}
//---------------------------------------------------------------------------
CModPar& CModPar::operator=(const CModPar& Source){
	if(this == &Source) return *this; // check for self-copying
	if (par!=NULL)
		delete [] par;
	if (dev!=NULL)
		delete [] dev;
	*const_cast<int*>(&num_par)=Source.num_par;
	dv=Source.dv;
	niter=Source.niter;
	crit=Source.crit;
	if (Source.par!=NULL){
		par=new double[num_par];
		for (int i=0; i<num_par; i++)
			par[i]=Source.par[i];
	}
	else
		par=NULL;
	if (Source.dev!=NULL){
		dev=new double[num_par];
		for (int i=0; i<num_par; i++)
			dev[i]=Source.dev[i];
	}
	else
		dev=NULL;
	return *this;
}
//---------------------------------------------------------------------------
CModPar::~CModPar(){
	if (par!=NULL)
		delete [] par;
	if (dev!=NULL)
		delete [] dev;
}
//---------------------------------------------------------------------------
void CModPar::SetPar(int _npar, double* _pr, double* _dev, int _iter, double _crit){
	if (_npar==0){ChState(-201);return;}																		//fail
	if (_dev==NULL)
		dv=false;
	else
		dv=true;

	if (_npar!=num_par){
		if (par!=NULL)
			delete [] par;
		if (dev!=NULL)
			delete [] dev;
		*const_cast<int*>(&num_par)=_npar;
		par=new double[num_par];
		if (dv)
			dev=new double[num_par];
		else
			dev=NULL;

	}
	else{//_npar==num_par
		if ((!dv)&&(dev!=NULL))
			delete dev;
		if ((dv)&&(dev==NULL))
			dev=new double[num_par];
	}
	for (int i=0; i<num_par; i++){
		par[i]=_pr[i];
		if (dv)
			dev[i]=_dev[i];
	}
	niter=_iter;
	crit=_crit;
}
//---------------------------------------------------------------------------
void CModPar::SetFitAttr(int _iter, double _crit){
	niter=_iter;
	crit=_crit;
}
//---------------------------------------------------------------------------
void CModPar::CpPar(double* _pr){
	for (int i=0; i<num_par; i++)
		_pr[i]=par[i];
}
//---------------------------------------------------------------------------
void CModPar::CpDev(double* _dev){
	for (int i=0; i<num_par; i++)
		_dev[i]=dev[i];
}
