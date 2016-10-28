//---------------------------------------------------------------------------

#ifndef Simplex_ft_FRAPH
#define Simplex_ft_FRAPH

#include "FRAPModel.h"
#include "FRAPData.h"
//---------------------------------------------------------------------------
//индексы вершин симплекса 0..dim
//индексы аргументов функции 0..dim-1
typedef double* VectPar;
typedef VectPar* SimpTop;

const double a=1.0;                	                         //коэффициент отражения
const double g=2.0;              	                           //коэффициент растяжения
const double b=0.5;            	                             //коэффициент сжатия
const double b1=0.5;         	                               //коэффициент режекции
const double e=0.1;        	                                 //шаг для начального задания симплекса
const double ep=1e-5;    	                                   //параметр выхода

class Simpleks
{
public:
	////////////////функции - члены/////////////////////////////////////////////
	Simpleks(FRAPMod* _md, int _st_pt, int _fin_pt, FRAPAnData* _dt1, FRAPAnData* _dt2=NULL);//конструктор -
	virtual ~Simpleks();
	void poisk_optim(VectPar& _param, int* _vparam, int& _num_tr, double& f, double* _dpar);//поиск оптимального решения
	void poisk_optim_comb(VectPar& _param1,VectPar& _param2,int* _vparam,int* _lparam,int& _num_tr,
												double& f,double* _dpar1,double* _dpar2);
protected:
	////////////////функции - члены/////////////////////////////////////////////

	double fun(double* _param, int* _vparam);                  //целевая функция
	double fun_comb(double* _param, int* _vparam, int* _lparam);
	void init_simpl(VectPar& _param, int* _vparam);		         //инициализация симплекса
	void init_simpl_comb(VectPar& _param1,VectPar& _param2,int* _vparam,int* _lparam);
	void zanulit(VectPar x);                                   //зануление вектора
	void summa(VectPar Sl1, VectPar Sl2, VectPar res);         //сложение двух векторов
	void proizv(VectPar m1, double lamda, VectPar res);        //умножение вектора на число
	void fix_value(VectPar mesto,VectPar s);                   //поменять местами значения указателей
	void find_ind();                                           //нахождение индексов особых точек
	void center_grav();                                        //центр тяжести точек за исключением наибольшей
	void otrazit();                                            //отражение относительно центра тяжести точек
	void stretch();                                            //растяжение
	void compres();                                            //сжатие
	void reject();                                             //режекция

	bool shodim();                                             //возвращает true, если достигнута требуемая сходимость

	void steret();                                             //уничтожение динамических переменных
	////////////////переменные - члены/////////////////////////////////////////

	FRAPMod* Mod;   			                                     //
	FRAPAnData* Dat1;      			                               //
	FRAPAnData* Dat2;                                          //

	const int dim;                                             //количество аргументов функции
	const int nparam;																					 //число параметров модели (nparam>=dim)
	SimpTop versh;                                             //массив указателей на вершины симплекса
	VectPar perv;                                              //указатель на начальные параметры
	VectPar cpar1;																						 //указатель на текущие параметры
  VectPar cpar2;
	VectPar x0;                                                //кооординаты центра масс
	VectPar xr;                                                //координаты отражённой точки
	VectPar xe;                                                //кооординаты растянутой точки
	VectPar xc;                                                //координаты сжатой точки

	double* f_val;                                             //значения целевой функции в вершинах симплекса
	double fr;                                                 //значение функции в точке xr
	double fe;                                                 //значение функции в точке xe
	double fc;                                                 //значение функции в точке xc

	int l;                                                     //индекс вершины с наименьшим значением
	int h;                                                     //индекс вершины с наибольшим значением
	int h1;                                                    //индекс вершины со вторым по величине значением

	int num_tr;
	const int st_pt;
	const int fin_pt;
};

#endif
