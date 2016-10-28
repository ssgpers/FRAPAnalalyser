//---------------------------------------------------------------------------

#ifndef FRAPModelH
#define FRAPModelH
#include <StdCtrls.hpp>
#include <complex>
//#include "dcompl.h"
#include"bessel.h"
#include"FRAPDefin.h"
#include "ModelPar.h"
#include <math.h>
//---------------------------------------------------------------------------
#define stepnum 1000

//typedef double(*fnmod)(double,double*); //fnmod;
//very small number for numerical derivatives parameter
const double epsil=1e-3;
const double epsil2=1e-9;

const double lg2frap=log(2.0); //natural logarithm of 2

const int nmodl=7;				 //number of FRAP models in use
 enum tmodl{
	undef=-1,	  				 //non-defined model
	exp1,                        //1 exponential model (abstract parameters)
	exp2,                        //2 exponential model (abstract parameters)
	bnd1, 						 //1 binding state (fast diffusion)
	bnd2,						 //2 binding states (fast diffusion)
	difc,						 //diffusion circular spot
	difr,       			     //diffusion rectangular spot
	bndd,                 		 //binding + diffusion in cirlular spot
	//plac,					     //actin polymerisation (fast diffusion)
	//bnd1m,						 //1 binding state (fast diffusion) - Mikalai variant
	//bnd2m,						 //2 binding states (fast diffusion) - Mikalai variant
};


/*const int nmodl=10;							 //number of FRAP models in use
 enum tmodl{
	undef=-1,											 //non-defined model
	bnd1,													 //1 binding state (fast diffusion)
	bnd2,													 //2 binding states (fast diffusion)
	difc,													 //diffusion circular spot
	difr,          					       //diffusion rectangular spot
	bndd,                 				 //binding + diffusion in cirlular spot
	plac,					                 //actin polymerisation (fast diffusion)
	bnd1m,												 //1 binding state (fast diffusion) - Mikalai variant
	bnd2m,												 //2 binding states (fast diffusion) - Mikalai variant
	exp1,                          //1 exponential model (abstract parameters)
	exp2                           //2 exponential model (abstract parameters)
}; */

//abstract FRAP model
class FRAPMod{
protected:
	double tbleach;
	//number of model parameters
	const int num_par;
	//model type
	const tmodl tp;
	//minimal values for parameters
	double* minv;
	//maximal values for parameters
	double* maxv;
	//default values for parameters
	double* defv;
	//evaluation of parameters
	virtual int par_val(double* pr)=0;
public:
	//constructor
	FRAPMod(double _tbl, int _npar, tmodl _tp);
	//destructor
	virtual ~FRAPMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par)=0;
	double evaltpoint(double t, CModPar& _mp);																		//for model parameters object
	//model evaluation for the array of time points
	void evaltarray(double* tarr,int point_num, double* par, double* resarr);
	void evaltarray(double* tarr,int point_num, CModPar& _mp, double* resarr);     //for model parameters abject
	//partial first derivatives for parameters for the definite argument
	virtual int evalderr(double t, double* par,double* derr)=0;
	//numerical calculation of the partial derivatives; derr is output array
	void numderr(double t, double* par, double* der);
	//evaluation of parameters using three-point method
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr)=0;
	//return number of parameters
	virtual int RetPrNum()const	{return num_par;}
	//return model type
	virtual tmodl RetModTp()const	{return tp;}
	//display parameters in the TMemo field
	//void DisplayPar (double* _param, TMemo* _mRep, double* _dparam=NULL) const;
	//model name
	virtual char* RetModName(void) const =0;
	//parameter name for index ind
	virtual char* RetParName(int ind) const =0;
	//parameter label for index ind
	virtual char* RetParLb(int ind) const =0;
	//parameter unit for index ind
	virtual char* RetParUn(int ind) const =0;
	//default values of parameters
	void DefParVal(double* _par)const;
	CModPar DefParVal()const;																											//return model parameters object
	//return limit values of parameters
	void LimParVal(double* _min, double* _max)const;
	//set limit values of parameters
	void SetLimParVal(double* _min, double* _max);
	//check for parameters limits
	void chlim(double* par);
	//are parameters in range
	int InRange(double* _par)const;
	//is i-th parameter in range
	int InRangeI(double _pr, int _i)const;
	//find the closest parameters to the given which are in the pre-defined limits. Returns 1 if all given parameters are initially in range
	int FindClosBod(double* _in_par, double* _res_par)const;
	//Penalty function. Returns 0 if given parameters are in range or positive number that increases with the distance to the closest border
	//closest border is also returned if second parameter is not null
	double FindPenalty(double* _in_par, double* _res_par)const;
	double FindPenalty(double* _in_par)const;
	//set bleached time value
	void SetTBl(double _tb);
};

//create model object for the given index
FRAPMod* createmodel (tmodl _ind);
//return model name for the defined index
char* retmodelname(tmodl _ind);

//abstract FRAP model, when the calculation requires inverse Laplace transform
class FRAPLpMod:public FRAPMod{
protected:
	// for Zakian method of inverse Laplace transform
	cdcompl alpha[5];
	cdcompl Ka[5];
public:
	//constructor
	FRAPLpMod(double _tbl, int _npar, tmodl _tp);
	//destructor
	virtual ~FRAPLpMod();
	//Inverse Laplastransform
	//Fourier Series Approximation - Translated Mathematica code
	double invlp2(double t,double* par,double T=50,double Er=1e-8, double sig0=0.001,int k_sum=2000);
	//Inverse Laplastransform
	//Zakian Method - Translated Mathematica code
	double invlp3(double t,double* par);
	//function in Laplace space
	virtual cdcompl Lapl(cdcompl s, double* par)=0;
	cdcompl invs(cdcompl arg);
};



//single exponential FRAP model
//3 parameters: a0, a1, t_half
//intens=a0-a1*exp(-(t-tbleach)/tau)      t_half=tau*ln2       tau=t_half/ln2
class SingExpMod:public FRAPMod{
protected:
	double a0;
	double a1;
	double t_half;
	double tau;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	SingExpMod(double tbl);
	//destructor;
	virtual ~SingExpMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void)const {return "Single exponential model";}
	virtual char* RetParName(int ind)const;
	virtual char* RetParLb(int ind)const;
	virtual char* RetParUn(int ind)const;
};

//one binding state FRAP model
//3 parameters: r, c_eq, k_off
//intens=(1-r)(1-c_eq*exp(-k_off*(t-tbleach)))
class OneBndMod:public FRAPMod{
protected:
	double r;
	double c_eq;
	double k_off;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	OneBndMod(double tbl);
	//destructor;
	virtual ~OneBndMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void)const{return "One Binding State model";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
};

//one binding state FRAP model - Mikalai variant
//3 parameters: f_eq, k_on, k_off
//intens=f_eq*(1+k_on/k_off*(1-exp(-k_off*(t-tbleach))))
class OneBndModM:public FRAPMod{
protected:
	double F_eq;
	double k_on;
	double k_off;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	OneBndModM(double tbl);
	//destructor;
	virtual ~OneBndModM();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void)const{return "One Binding State model";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
};

//double exponential FRAP model
//5 parameters: a0, a1, a2, tau1, tau2
//intens=a0-a1*exp(-(t-tbleach)/tau1)-a2*exp(-(t-tbleach)/tau2)			 tau1=t_half1/ln2    tau2=t_half2/ln2
class DoubExpMod:public FRAPMod{
protected:
	double a0;
	double a1;
	double a2;
	double t_half1;
	double t_half2;
	double tau1;
	double tau2;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	DoubExpMod(double tbl);
	//destructor
	virtual ~DoubExpMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par) ;
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Double Exponential model";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
};

//two bining states FRAP model
//5 parameters: r, c_eq1, c_eq2, k_off1, k_off2
//intens=(1-r)(1-c_eq1*exp(-k_off1*(t-tbleach))-c_eq2*exp(-k_off2*(t-tbleach)))
class TwoBndMod:public FRAPMod{
protected:
	double r;
	double c_eq1;
	double c_eq2;
	double k_off1;
	double k_off2;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	TwoBndMod(double tbl);
	//destructor;
	virtual ~TwoBndMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Two Binding State model";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
};

//two binding states FRAP model - Mikalai variant
//5 parameters: f_eq, k_on1, k_on2, k_off1, k_off2
//intens=f_eq*(1+k_on1/koff1*(1-exp(-k_off1*(t-tbleach)))+k_on2/k_off2*(1-exp(-k_off2*(t-tbleach))))
class TwoBndModM:public FRAPMod{
protected:
	double F_eq;
	double k_on1;
	double k_on2;
	double k_off1;
	double k_off2;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	TwoBndModM(double tbl);
	//destructor;
	virtual ~TwoBndModM();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Two Binding State model";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
};

//Soumpasis diffusion FRAP model (diffusion circular spot)
//4 parameters:
							//a0(fraction of unbleached molecules),
							//a1(amplitude - in the case of not complete recovery),
							//D(diffusion coefficient),
							//w(radius of bleach spot)
//intens=a0+a1*exp(-2*tau/(t-tbleach))*(I0(-2*tau/(t-tbleach))+I1(-2*tau/(t-tbleach)))
//tau=w*w/4/D (parameters w and D are correlated)
//I0(),I1() - modified Bessel functions
class SoumpDiffMod:public FRAPMod{
protected:
	double a0;
	double a1;
	double D;
	double w;
	double tau;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	SoumpDiffMod(double tbl);
	//destructor;
	virtual ~SoumpDiffMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Diffusion Model for Circular Spot";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
};

//Ellenberg diffusion FRAP model (diffusion rectangular spot)
//4 parameters:
							//a0(fraction of unbleached molecules),
							//a1(amplitude - in the case of not complete recovery),
							//D(diffusion coefficient),
							//w(width of the bleach)
//intens=a0+a1*sqrt(1-w*w/(w*w+4*pi*D*(t-tbleach)))
//tau=w*w/4/D (parameters w and D are correlated)
//I0(),I1() - modified Bessel functions
class EllDiffMod:public FRAPMod{
protected:
	double a0;
	double a1;
	double D;
	double w;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	EllDiffMod(double tbl);
	//destructor;
	virtual ~EllDiffMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Diffusion Model for Rectangular Spot";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
};

//work model for testing purposes
//currently inverse Laplace transform
//function  1/(s^2+1)
class WrkMod:public FRAPLpMod{
protected:
	double a0;
	double a1;
	double D;
	double w;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	WrkMod(double tbl);
	//destructor;
	virtual ~WrkMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Work model";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
private:
	//function in Laplace space
	virtual cdcompl Lapl(cdcompl s, double* par);
};

//Binding and diffusion FRAP model
//5 parameters:
							//r - additional parameter for uncomplete recovery,
							//kons - pseudo binding rate constant,
							//koff - unbinding rate constant,
							//D - diffusion coefficient,
							//w - radius of the bleached spot
//for the eq. see Sprague(2004) BiophysJ
class BndDiffMod:public FRAPLpMod{
protected:
	double r;
	double kons;
	double koff;
	double D;
	double w;
	double f_eq;
	double c_eq;
	double q;
	//evaluation of model parameters
	virtual int par_val(double* pr);
public:
	//constructor
	BndDiffMod(double tbl);
	//destructor;
	virtual ~BndDiffMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Binding+Diffusion Model";}
	virtual char* RetParName(int ind) const;
	virtual char* RetParLb(int ind) const;
	virtual char* RetParUn(int ind) const;
private:
	//function in Laplace space
	virtual cdcompl Lapl(cdcompl s, double* par);
};

//Polymerization FRAP model
//6 parameters:
							//konb - association rate constant for barbed end,
							//konp - association rate constant for pointed end,
							//koffb - dissociation rate constant for barbed end,
							//koffp - dissociation rate constant for pointed end,
							//L - length of the filament (in actin subunits),
							//cf - concentration of filamentous actin
//intens=..... see description
class PolymerMod:public FRAPMod{
protected:
	double konb;
	double konp;
	double koffb;
	double koffp;
	double cf;
	double L;

	double v;
	double dib;
	double dip;
	double cm;

	double resb;
	double resp;
	double res;
	double tau;

	//evaluation of model parameters
	virtual int par_val(double* pr);

public:
	//constructor
	PolymerMod(double tbl);
	//destructor;
	virtual ~PolymerMod();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Polymerization Model";}
	virtual char* RetParName(int ind)const;
	virtual char* RetParLb(int ind)const;
	virtual char* RetParUn(int ind)const;
};

//Polymerization FRAP model in terms of polymerisation rates and diffusivity coefficients
//6 parameters:
							//dib - diffusivity coefficient for barbed end,
							//dip - diffusivity coefficient for pointed end,
							//v - polymerisation (treadmilling) rate,
							//cm - concentration of actin monomers,
							//L - length of the filament (in actin subunits),
							//cf - concentration of filamentous actin
//intens=..... see description
class PolymerMod2:public FRAPMod{
protected:
	double v;
	double dib;
	double dip;
	double cm;
	double cf;
	double L;

	double resb;
	double resp;
	double res;
	double tau;

	//evaluation of model parameters
	virtual int par_val(double* pr);

public:
	//constructor
	PolymerMod2(double tbl);
	//destructor;
	virtual ~PolymerMod2();
	//model evaluation for a single time point
	virtual double evaltpoint(double t, double* par);
	virtual int evalderr(double t, double* par,double* derr);
	//virtual int threepointpar(double* tm, double* intens, int point_num, int bl_ind, double* pr);
	virtual char* RetModName(void) const {return "Polymerization Model 2";}
	virtual char* RetParName(int ind)const;
	virtual char* RetParLb(int ind)const;
	virtual char* RetParUn(int ind)const;
};

#endif
