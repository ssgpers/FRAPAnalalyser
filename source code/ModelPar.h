//---------------------------------------------------------------------------

#ifndef ModelParH
#define ModelParH

#include "FRAPDefin.h"
//---------------------------------------------------------------------------
extern int state;

class CModPar{
 private:
	const int num_par;																	//number of parameters
	double* par;                      									//values of parameters
	double* dev;                      									//values of deviations
	bool dv;                           									//are deviations defined

	//characteristic of fit
	int niter;                                          //number of iterations for fitting
	double crit;                                        //goodness criterion

 public:
	CModPar(int _npar=0);																//default constructor
	CModPar(const CModPar& Source);											//copy constructor
	CModPar& CModPar::operator=(const CModPar& Source);	//operator = (assignment)

	virtual ~CModPar();																	//destructor

	void SetPar(int _npar,double* _pr,double* _dev=NULL,
							int _iter=0, double _crit=0);						//set parameters and deviations. If necessary, reallocate memory for data structures. At the same time set fitting iterations and criterion. If not defined, the last two parameters are 0 by default.
  void SetFitAttr(int _iter, double _crit);           //set fit attributes: number of fitting iterations and optimum criterion
	int RetNPar()const {return num_par;}  							//return number of parameters
	double* RetPPar()const{return par;}									//return pointer to the parameters values
	double* RetPDev()const{return dev;}									//return pointer to the deviations values
	void CpPar(double* _pr);														//copy parameters values to the pre-defined array
	void CpDev(double* _dev);														//copy deviations values to the pre-defined array
	int RetIter()const {return niter;}	  							//return number of iterations
	double RetCrit()const {return crit;}		  					//return goodness criterion
};


#endif
