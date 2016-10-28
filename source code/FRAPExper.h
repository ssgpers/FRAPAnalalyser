//---------------------------------------------------------------------------

#ifndef FRAPExperH
#define FRAPExperH

#include "FRAPData.h"

#include "FRAPModel.h"
//#include "ErrAnalysis.h"
#include "Fit.h"


#include <fstream.h>
//---------------------------------------------------------------------------
extern int state;

struct SFitPar{
	SFitPar():fmeth(0),fmod(0),fit_all(true),st_pt(0),fin_pt(0),iter_max(200){;}
	tfitmeth fmeth;                                               								//fitting method
	tmodl fmod;																																		//fitting model
	bool fit_all;
	int st_pt;
	int fin_pt;
	int iter_max;                                                 								//maximum number of iterations
};


class CFRAPExper:public CFName, CFit{
 public:
	//constructors
	CFRAPExper(char* _nm=NULL);
	//destructor
	~CFRAPExper();

	//input/output commands
	int RetDtNum()const{return datnum;}									 													//return number of the datasets
	bool SvDataS(char* _filenm,int _idat)const;					 													//save dataset as a text file

	//datasets manipulation methods
	int AddNewD();																			 													//Add New Dataset. Returns index of the created dataset in Array.
	int AddNewDFl(char* _filenm);                        												 	//Add New Dataset by copying data from file. Returns index of the created dataset in Array.
	int AddD(CFRAPData* _dt);                            													//Add Dataset to Experiment (without copying the data). Returns index of the created dataset in Array.
	void DelD(int _pos);															 	 													//Deletes (erases memory) the dataset (without copying)
	CFRAPData* RetD(int _pos){return FDataS[_pos];}			 													//Return pointer to the dataset (without copying)

	void SetInd(int _fr, int _ref, int _base);					 													//set experimental indexes for all experimental datasets
	void SetTBl(double _t);															 													//set bleaching time
	void SetMBl(int _pt);																 													//set first postbleach time point

	//data processing methods
	void RunNorm(tnorm _meth);													 													//normalisation of all datasets in experiment
	int AddAver(void);																	 													//creates new averaged dataset and fills it with data. Returns index of the created average dataset in Array. All experimental datasets of the current experiment are considerted.

	//model associated methods
	FRAPMod* RetModl()const{return Md;}									 													//return pointer to the current model
	tmodl RetModTp();																		 													//return type of the current model
	FRAPMod* SetModl(tmodl _ind);												 													//set model for the current experiment and given index, returns pointer to the model
	void RunSim(double* _par, int _idat=-1);						 													//run simulation
	void SvParam(char* _filenm, int _idat=-1);					 													//save parameters
	void ParToStream(ofstream& _ostr, int _idat=-1,
									 bool _condnm=false,bool _biter=false, bool _bcrit=false);		//save parameters to the output stream

	//fitting associated methods
	virtual double FitFunc(double _x, double* _par);					    								//function for fit (single point evaluation) - overload
	virtual void FitFunc(double* _par, int _point_num, double* _x, double* _res);	//function for fit (array evaluation) - overload
	virtual int InRange(double* _par) const;                                      //function for checking if parameters in the appropriate range.
	virtual double FindPenalty(double* _par,double* _bod_par)const;               //function to find penalty and closest border if parameters are not in range
	virtual void EvDerr(double _x, double* _par, double* _derr);	                //function for evaluation derivatives
	void FitExper(double* _par,int* _vpar,tfitmeth _fmet, bool _fitall,
								int	_stp, int _finp, int _miternum);														//run fit for all datasets in experiment
	void FitDataS(double* _par,int* _vpar,tfitmeth _fmet, bool _fitall,
								int	_stp, int _finp, int _miternum, int _idat);									//run fit for one dataset


 private:
	void AddMem();
 protected:
	int datnum;      																		 //current number of datasets
	int mdatnum;                                         //current maximum number of datasets
	CFRAPData** FDataS;                                  //array of datasets

	//model and fitting associated class members
	FRAPMod* Md;                                         //current model used for this experiment

};
#endif
