//---------------------------------------------------------------------------

#ifndef FRAPDataH
#define FRAPDataH

#include "FRAPDefin.h"
#include "ModelPar.h"
//---------------------------------------------------------------------------
extern int state;
//class for FRAP Data.
//Utilises storage of the data and data output.
//InIntens for either Experimental or Average intensity datasets. FitIntens is for fitting intensity dataset
//type of data object is identified by cdat class member
class CFRAPData:public CFName{
 public:
	//constructors
	CFRAPData(int _inum=0, int _pnum=0, int _npar=0);    //creates structure. allocates memory, if both arguments >0.but does not fill in the data
	CFRAPData(char* _filenm, int _adint=0, int _npar=0); //create from file reserve columns for additional intensities
	CFRAPData(const CFRAPData& Source);		 					 		 //copy constructor

	~CFRAPData(void);

	//memory manipulation
	void ChangeDim(int _inum, int _pnum);								 //resize

	//input methods
	void SetTBl(double _t);															 //set bleaching time
	void SetMBl(int _pt);																 //set first postbleach time point
	virtual void SetInd(int _roi, int _ind)=0;					 //set indexes in the data
	void SetPar(CModPar _mp){MPar=_mp;}									 //set model mapameters
	void SetCPar(int _n, double* _par, double* _var=NULL,
							 int _iter=0, double _crit=0);					 //set parameters by copying from the arrays. Fitting criterion and number of iterations are set at the same time
	void SetFitAttr(int _iter, double _crit);						 //set fit attributes: number of fitting iterations and optimum criterion

	//output methods
	void CopyTmArr(double* _tm) const;				 		 	 		 //copy time column (memory has to be allocated before)
	doubleptr RetTm()const{return time;}								 //return pointer to the time array
	int RetIntNum()const{return intens_num;}						 //return number of intensity columns in the dataset
	int RetPtNum()const{return point_num;}							 //return number of points in the dataset
	void CopyIntArr(double* _dest, int _iind) const;		 //copy intensity column (memory has to be allocated before)
	double* RetIntArr(int _iind) const;									 //return pointer to the intensity array.
	double RetTBl()const{return tbl;}                    //return bleach time
	int RetMBl()const{return mbl;}		                   //return first postbleach time point
	virtual int RetInd(tcol _roi)const=0;								 //return data index
	char* RetIntNm(int _iind) const;						 				 //return intensity name
	tdat RetType()const{return cdat;}                 	 //return dataset type


	bool SvData(char* _filenm);                          // save dataset to the text file
	//data processing methods
	void EvalResid();															       //evaluate residuals between normalised/averaged and simulated data
	CModPar RetPar()const{return MPar;}									 //return model mapameters
	double*	RetPPar()const{return MPar.RetPPar();}			 //return pointer to the parameters array
	int RetIter()const{return MPar.RetIter();}           //return number of iterations
	double RetCrit()const{return MPar.RetCrit();}				 //return goodness criterion

	//data type specific methods - producing errors if asked
	//for experimental datasets
	virtual	void RunNorm(tnorm _meth){ChState(-151);}    //run normalisation

 protected:
	void SetNames(char* _st=NULL);           				 		 //Set intensity names. Input - single string with tab-separated names.
	void AllocMem(int _inum, int _pnum);               	 //Initialise structures in the object.
 protected:
	tdat cdat; 																			 		 //data type

	int point_num;																	 		 //number of data points

	doubleptr time;
	double tbl; 																		 		 //bleaching time
	int mbl;																						 //first postbleach time point

	int intens_num;    			                             //total number of intensity arrays
	doubleptr* Intens;                                   //matrix for intensity values
	char** IntNames;                                     //names of the intensity columns

	int csiml;                                           //index for simulated data
	int cresd;                                           //index for residuals
	int cnorm;																					 //normalised or averaged data

	//model and fit associated parameters
	CModPar MPar;																			 	 //fitted model parameters
};

//---------------------------------------------------------------------------
//class for the FRAP Data that contains experimental information.
//Utilises data input and normalisation/calibration algorithms.
class CExpData:public CFRAPData{
 public:
	//constructors
	CExpData(void);                                      //empty structure
	CExpData(char* _filenm);											 		   //create from file
	CExpData(const CExpData& Source);										 //copy constructor
	~CExpData(void);

	//input methods
	virtual void SetInd(int _roi, int _ind);						 //set experimental indexes

	//output methods
	virtual int RetInd(tcol _roi)const;

	//data processing methods
	virtual void RunNorm(tnorm _meth);									 //normalisation of this dataset


 private:
	//"point by point" normalisation methods.
	void DoubleNorm(void);
	void SingleNorm(void);
	void NoNorm(void);
	void FullScaleCalibr(void);
	void PostToUn(void);
  void BeadNorm(void);

	double PreIntens(int _croi);										 		 //average prebleach intensity of selected roi with subtracted background
 protected:
	int eint_num;

	int cfrap;
	int cref;
	int cbase;
};
//---------------------------------------------------------------------------
//class for the FRAP Data that contains Averaged FRAP recoveries
class CAvData:public CFRAPData{
 public:
	//constructors
	CAvData(doubleptr* _EInt,int _inum, int _pnum, double* _tm, double _tbl, int _npar);	//create average from other datasets
	CAvData(const CAvData& Source);											//copy constructor
	~CAvData(void);

	//data manipulation methods
	virtual void SetInd(int _roi, int _ind){ChState(-155);}//Set indexes of the data!!! currently unused here
	//output methods
	virtual int RetInd(tcol _roi)const;


 private:
	void EvalObj(doubleptr* _EInt,int _inum, int _pnum); //evaluation of the object - calculation of average signal, standard deviations and standard errors
 protected:
	int csdev;                                          //index for standard deviations
	int cserr;                                          //index for standard errors
	int avnum;																					//number of averages
};
#endif
