//---------------------------------------------------------------------------

#ifndef FRAPProjH
#define FRAPProjH

#include "FRAPExper.h"
//---------------------------------------------------------------------------
extern int state;

class CFRAPProj{
 public:
	//constructors
	CFRAPProj();
	//destructor
	~CFRAPProj();

	//data manipulation methods
	CFRAPExper* AddExper();	 			 		  									//add new experiment
	CFRAPExper* AddToList(CFRAPExper* _AExp);		  				//add previously created experiment to list
	void DelExper(int _iexp);	 					 	  							//delete experiment
	//input methods
	//AddExpFls(char* _nms, int _expnum);										//add experimental files to the experiment
	//output methods
	int RetExpNum()const{return expnum;}									//turn currrent number of experiments
	CFRAPExper* RetE(int _iexp){return Exp[_iexp];} 			//return pointer to the experiment (without copying
	void CopyExpNms(char** _Nms)const;		 								//copy names of the experiments. Memory has to be reserved in advance
	void CopyDatNms(char** _Nms,int* _tp, int _iexp)const;//copy names of the datasets of one experiment and their types. Memory has to be reserved in advance
	char*	RetExpNm(int _iexp)const;												//return pointer to the name of one experiment
	char*	RetDatNm(int _iexp, int _idat)const;						//return pointer to the name of one dataset
	void SvPar(char* _filenm,
						 bool _biter=false, bool _bcrit=false)const; //save parameters for all experiments. Number of fitting iteration and goodness of fit can be optionally saved
	bool SvDataS(char* _filenm, int _iexp, int _idat)const;//save dataset as a text file

	//calculations running methods
	void RunNormE(tnorm _meth, int _iexp);	           	  //run normalisation for the experiment
	void RunNormD(tnorm _meth, int _iexp, int _idat);	    //run normalisation for the dataset
	int RunAverE(int _iexp);								              //calculates averages for the particular experiment and adds the dataset to the experiment. All experimental data are considered. Returns position of the created dataset in experiment
	//void RunSim(int _meth, int _expnum, int _datanum);  //run simulation for the dataset
	//void RunFit(int _meth, int _expnum, int _datanum);  //run fitting for the dataset

 private:
	void AddMem();
 protected:
	CFRAPExper** Exp;
	int expnum;
	int mexpnum;
	int expcnt;
 private:
};


#endif
