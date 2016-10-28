//---------------------------------------------------------------------------

#ifndef FRAPDefinH
#define FRAPDefinH

#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

#define ChState(_s)  	if (_s<state) state=_s;

 //type definitions
 typedef double* doubleptr;
 typedef char* charptr;

 const int LenIntNm=70;							//Maximum length of the intensity or intensity array name

 const char exprnm[]="Experiment ";

 // variable for the errors indication
 //int state(1);    //now defined in commmands.h
 //from -1	 to -100 - CFRAPIntens and its derivatives
 //from -101 to -200 - CFRAPData	 and its derivatives
 //from -201 to -300 - CFRAPModel	 and its derivatives
 //from -301 to -400 - CFRAPExper
 //from -401 to -500 - CFRAPProj
 //from -501 to -600 - running environment
 //from -601 to -700 - fitting and analysis methods

 ///!!! Add List of Messages which will be used by interface.



 //Indexes and corresponding names in intensity datasets
 const	int naver=3;                //Number of intensity columns in averaged dataset;
 const	int nfit=2;                 //number of fitting indexes
 const	int nexp=3;                 //Number of experimental indexes

 //pre-defined column names
 const char signnm[]="Signal";
 const char sdevnm[]="Deviation";
 const char serrnm[]="Error";
 const char simlnm[]="Model";
 const char resdnm[]="Residuals";
 const char mesrnm[]="ROI ";
 const char normnm[]="Normalised";

 enum tcol{
	iundef=-1,												//undefined column
	ifrap,                            //frap signal (measured)
	iref,                             //reference signal (measured)
	ibase,                            //background signal (measured)
	inorm,                            //normalised data  or average signal (calculated)
	isdev,                            //standard deviations (calculated)
	iserr,                            //standard errors (calculated)
	isiml,                            //simulated or fitted curve
	iresd                            //residuals(between model and normalised or average signal)
};

 //Indexes for the tyeps of FRAP data
 const int ndat=2;                //number of data types
 enum tdat{
	undefd=-1,											//non-defined data
	expd,      							        //experimental data
	avrd               							//averaged data
 };

 //indexes for plottymes
 const int nplots=7;
 enum tplot{
	pall,
	pfrap,
	pref,
	pback,
	pnorm,
	psiml,
	pnormsiml,
	presid
};														 	 //modify menu accordingly


 //indexes of normalisation algorithms
 const int nnorm=5;									//number of normalisation algorithms
 enum tnorm{doub,sing,fulc,nonorm, beadn}; //normalisation methods

class CFName{
 public:
	CFName(){StrCopy(Name,"");}
	CFName(char* _nm){SetNm(_nm);}
	~CFName(){;}

	void SetNm(const char* _nm){StrLCopy(Name,_nm,LenIntNm);}//set (overwrite) the name
  void SetPNm (const char* _fpath);								 //get name from the file path and set it.
	char* RetNm()/*const*/{return Name;}						 //return name (without copying)
	void RenCopy(CFName _fnm);										 //name of the copy
	void CopyNm(char* _tnm){StrCopy(_tnm,Name);}		//copy the name. Memory has to be allocated before.
 protected:
	char Name[LenIntNm];
};

#endif
