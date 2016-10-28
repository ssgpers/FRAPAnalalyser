//---------------------------------------------------------------------------


#pragma hdrstop

#include "Simplex_ft_FRAP.h"
#include "statappllib.h"
#include <math.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

Simpleks::Simpleks(FRAPMod* _md, int _st_pt, int _fin_pt, FRAPAnData* _dt1, FRAPAnData* _dt2){
	num_tr=0;
	Mod=_md;
	Dat1=_dt1; //*const_cast<FRAPAnData**>(&Dat)=_dt;
	Dat2=_dt2;
	*const_cast<int*>(&nparam)=Mod->RetPrNum();
	*const_cast<int*>(&st_pt)=_st_pt;
	*const_cast<int*>(&fin_pt)=_fin_pt;

	cpar1=NULL;
	cpar2=NULL;
}

Simpleks::~Simpleks(){}

void Simpleks::poisk_optim(VectPar& _param, int* _vparam,  int& _num_tr, double& f, double* _dpar){
	int i;
	int cnt=0;
	num_tr=0;
	//задание параметров метода и условия задачи
	init_simpl(_param, _vparam);             //инициализация симплекса

	//нахождение индексов особых точек

	do{
		find_ind();
		center_grav();
		otrazit();
		fr=fun(xr, _vparam);
		num_tr++;    //счетчик числа итераций
		if (fr<f_val[l]){
			stretch();
			fe=fun(xe, _vparam);
			//num_tr++;
			if (fe<f_val[l]){
				fix_value(versh[h],xe);
				f_val[h]=fe;
				continue;
			}
			else{
				fix_value(versh[h],xr);
				f_val[h]=fr;
				continue;
			}
		}
		else if((fr>f_val[l]) && (fr<=f_val[h1])){
			fix_value(versh[h],xr);
			f_val[h]=fr;
			continue;
		}
		else{              //(fr>f_val(l)) && (fr>f_val(h1))
			if (fr<f_val[h]){
				fix_value(versh[h],xr);
				f_val[h]=fr;
			}
			compres();
			fc=fun(xc,_vparam);
			//num_tr++;
			if (fc<f_val[h]){
				fix_value(versh[h],xc);
				f_val[h]=fc;
				continue;
			}
			else{
				reject();
				for (i=0;i<dim+1;i++)
					if (i!=l){
						f_val[i]=fun(versh[i],_vparam);
						//num_tr++;
					}
				continue;
			}
		}
	}while (!shodim());
	for (i=0; i<nparam;i++)
		if (_vparam[i]){
			_param[i]=versh[l][cnt];
			cnt++;
		}
	_num_tr=num_tr;
	f=f_val[l];
	Mod->evaltarray(Dat1->time,Dat1->point_num,_param,Dat1->Intens[Dat1->imodl]);
	if (Dat1->avnum==1){
		double* sig=new double[Dat1->point_num];
		sdevest(Dat1->Intens[Dat1->isign], Dat1->Intens[Dat1->imodl], Dat1->point_num, sig);
		clcovmtr(Dat1->time+st_pt, Dat1->Intens[Dat1->isign]+st_pt, sig+st_pt, fin_pt-st_pt,_param,_vparam,Mod,_dpar,&f);
		delete [] sig;
	}
	else
		clcovmtr(Dat1->time+st_pt, Dat1->Intens[Dat1->isign]+st_pt, Dat1->Intens[Dat1->isdev]+st_pt, fin_pt-st_pt,_param,_vparam,Mod,_dpar);
	steret();
}

void Simpleks::poisk_optim_comb(VectPar& _param1,VectPar& _param2,int* _vparam,int* _lparam,int& _num_tr,
																double& f,double* _dpar1,double* _dpar2){
	int i;
	int cnt=0;
	num_tr=0;
	//задание параметров метода и условия задачи
	init_simpl_comb(_param1,_param2, _vparam,_lparam);             //инициализация симплекса
	do{
		find_ind();
		center_grav();
		otrazit();
		fr=fun_comb(xr, _vparam, _lparam);
		num_tr++;    //счетчик числа итераций
		if (fr<f_val[l]){
			stretch();
			fe=fun_comb(xe,_vparam,_lparam);
			//num_tr++;
			if (fe<f_val[l]){
				fix_value(versh[h],xe);
				f_val[h]=fe;
				continue;
			}
			else{
				fix_value(versh[h],xr);
				f_val[h]=fr;
				continue;
			}
		}
		else if((fr>f_val[l]) && (fr<=f_val[h1])){
			fix_value(versh[h],xr);
			f_val[h]=fr;
			continue;
		}
		else{              //(fr>f_val(l)) && (fr>f_val(h1))
			if (fr<f_val[h]){
				fix_value(versh[h],xr);
				f_val[h]=fr;
			}
			compres();
			fc=fun_comb(xc,_vparam, _lparam);
			//num_tr++;
			if (fc<f_val[h]){
				fix_value(versh[h],xc);
				f_val[h]=fc;
				continue;
			}
			else{
				reject();
				for (i=0;i<dim+1;i++)
					if (i!=l){
						f_val[i]=fun_comb(versh[i],_vparam,_lparam);
						//num_tr++;
					}
				continue;
			}
		}
	}while (!shodim());
	for (i=0; i<nparam;i++)
		if (_vparam[i]){
			_param1[i]=versh[l][cnt];
			cnt++;
			if (_lparam[i])
				_param2[i]=_param1[i];
			else{
				_param2[i]=versh[l][cnt];
				cnt++;
			}
		}
	_num_tr=num_tr;
	f=f_val[l];
	Mod->evaltarray(Dat1->time,Dat1->point_num,_param1,Dat1->Intens[Dat1->imodl]);
	Mod->evaltarray(Dat2->time,Dat2->point_num,_param2,Dat2->Intens[Dat2->imodl]);
	/*if (Dat1->avnum==1){
		double* sig=new double[Dat1->point_num];
		sdevest(Dat1->Intens[Dat1->isign], Dat1->Intens[Dat1->imodl], Dat1->point_num, sig);
		clcovmtr(Dat1->time+st_pt, Dat1->Intens[Dat1->isign]+st_pt, sig+st_pt, fin_pt-st_pt,_param,_vparam,Mod,_dpar,&f);
		delete [] sig;
	}
	else
		clcovmtr(Dat1->time+st_pt, Dat1->Intens[Dat1->isign]+st_pt, Dat1->Intens[Dat1->isdev]+st_pt, fin_pt-st_pt,_param,_vparam,Mod,_dpar);
	*/
	steret();
}

double Simpleks::fun(double* _param, int* _vparam){
	int cnt=0;
	for (int i= 0;i<nparam;i++)
		if (_vparam[i]){
			cpar1[i]=_param[cnt];
			cnt++;
		}

	if (!Mod->InRange(cpar1))
		return 1e30;//2*f_val[h];

	//
	if (cpar1[1]<0)
		return 0.0/0.0;
	//
	int pnum=Dat1->point_num;
	double r;
	double* anal=new double[pnum];
	Mod->evaltarray(Dat1->time+st_pt,pnum,cpar1,anal);
	if (Dat1->avnum==1)
		r=Hi_Sq(Dat1->Intens[Dat1->isign]+st_pt,anal,fin_pt-st_pt);
	else
		r=Hi_Sq(Dat1->Intens[Dat1->isign]+st_pt,anal,fin_pt-st_pt,Dat1->Intens[Dat1->isdev]+st_pt);
	delete [] anal;
	return r;
}

double Simpleks::fun_comb(double* _param, int* _vparam, int* _lparam){
	int pnum;
	double* anal;
	double r1=0, r2=0;

	int cnt=0;

	for (int i= 0;i<nparam;i++)
		if (_vparam[i]){
			cpar1[i]=_param[cnt];
			cnt++;
			if (_lparam[i])
				cpar2[i]=cpar1[i];
			else{
				cpar2[i]=_param[cnt];
				cnt++;
			}

		}

	if ((!Mod->InRange(cpar1))||(!Mod->InRange(cpar2)))
		return 1e30;//2*f_val[h];

	pnum=Dat1->point_num;
	anal=new double[pnum];
	Mod->evaltarray(Dat1->time+st_pt,pnum,cpar1,anal);
	if (Dat1->avnum==1)
		r1=Hi_Sq(Dat1->Intens[Dat1->isign]+st_pt,anal,fin_pt-st_pt);
	else
		r1=Hi_Sq(Dat1->Intens[Dat1->isign]+st_pt,anal,fin_pt-st_pt,Dat1->Intens[Dat1->isdev]+st_pt);
	delete [] anal;

	pnum=Dat2->point_num;
	anal=new double[pnum];
	Mod->evaltarray(Dat2->time+st_pt,pnum,cpar2,anal);
	if (Dat2->avnum==1)
		r2=Hi_Sq(Dat2->Intens[Dat2->isign]+st_pt,anal,fin_pt-st_pt);
	else
		r2=Hi_Sq(Dat2->Intens[Dat2->isign]+st_pt,anal,fin_pt-st_pt,Dat2->Intens[Dat2->isdev]+st_pt);
	delete [] anal;
 
	return 0.5*sqrt(r1*r1+r2*r2);
}

void Simpleks::init_simpl(VectPar& _param, int* _vparam){
	int i;
	int j;
	int cnt;
	perv=_param;                             //передаём указатель на вектор начального приближения
	*const_cast<int*>(&dim)=0;
	//вычисление числа варьируемых параметров
	for (i=0; i<nparam; i++)
		if (_vparam[i])
			*const_cast<int*>(&dim)+=1;

	//выделение памяти под мссивы
	versh=new VectPar[dim+1];
	for (int i=0;i<dim+1;i++)
		versh[i]=new double[dim];
	f_val=new double[dim+1];
	x0=new double[dim];
	xr=new double[dim];
	xe=new double[dim];
	xc=new double[dim];
	cpar1=new double[nparam];
	for (i=0;i<nparam;i++)
		cpar1[i]=perv[i];

	for (i=0; i<dim+1; i++){
		cnt=0;
		for (j=0;j<nparam;j++)
			if (_vparam[j]){
				versh[i][cnt]=perv[j];
				cnt++;
			}
	}

	for (j=0;j<dim;j++){
		if (abs(versh[j][j]))
			versh[j][j]=versh[j][j]*(1.0+e);
		else
			versh[j][j]=e;
		x0[j]=0;
		xr[j]=0;
		xe[j]=0;
		xc[j]=0;
	}

	//нахождение значений целевой функции в вершинах симплекса
	for (i=0;i<dim+1;i++){
		f_val[i]=fun(versh[i],_vparam);
		//num_tr++;
	}
}

void Simpleks::init_simpl_comb(VectPar& _param1, VectPar& _param2, int* _vparam, int* _lparam){
	int i;
	int j;
	int cnt;
	//perv=_param;                             //передаём указатель на вектор начального приближения
	*const_cast<int*>(&dim)=0;
	//вычисление числа варьируемых параметров
	for (i=0; i<nparam; i++)
		if (_vparam[i]){
			*const_cast<int*>(&dim)+=1;
			if (!(_lparam[i]))
				*const_cast<int*>(&dim)+=1;
		}
	//выделение памяти под массивы
	versh=new VectPar[dim+1];
	for (int i=0;i<dim+1;i++)
		versh[i]=new double[dim];
	f_val=new double[dim+1];
	x0=new double[dim];
	xr=new double[dim];
	xe=new double[dim];
	xc=new double[dim];
	cpar1=new double[nparam];
  cpar2=new double[nparam];
	for (i=0;i<nparam;i++){
		cpar1[i]=_param1[i];
		cpar2[i]=_param2[i];
	}
	for (i=0; i<dim+1; i++){
		cnt=0;
		for (j=0;j<nparam;j++)
			if (_vparam[j]){
				versh[i][cnt]=_param1[j];
				cnt++;
				if (!(_lparam[j])){
					versh[i][cnt]=_param2[j];
					cnt++;
				}
			}
	}

	for (j=0;j<dim;j++){
		if (abs(versh[j][j]))
			versh[j][j]=versh[j][j]*(1.0+e);
		else
			versh[j][j]=e;
		x0[j]=0;
		xr[j]=0;
		xe[j]=0;
		xc[j]=0;
	}

	//нахождение значений целевой функции в вершинах симплекса
	for (i=0;i<dim+1;i++){
		f_val[i]=fun_comb(versh[i],_vparam, _lparam);
		//num_tr++;
	}
}

void Simpleks::zanulit(VectPar x){
	int i;
	for (i=0;i<dim;i++)
		x[i]=0.0;
}

void Simpleks::summa(VectPar Sl1, VectPar Sl2, VectPar res){
	int i;
	for (i=0;i<dim;i++)
		res[i]=Sl1[i]+Sl2[i];
}

void Simpleks::proizv(VectPar m1, double lamda, VectPar res){
	int i;
	for (i=0;i<dim;i++)
		res[i]=lamda*m1[i];
}

void Simpleks::fix_value(VectPar mesto,VectPar s){
	int i;
	for (i=0;i<dim;i++)
		mesto[i]=s[i];
}

void Simpleks::find_ind()
{
	int i;

	l=0;
	h=0;

	for (i=1;i<dim+1;i++)
		if (f_val[i]<f_val[l])
			l=i;
	else if (f_val[i]>f_val[h])
			h=i;
	h1=l;
	for (i=0;i<dim+1;i++)
		if ((f_val[i]<f_val[h])&& (f_val[i]>f_val[h1]))
			h1=i;
}

void Simpleks::center_grav(){
	int i;
	zanulit(x0);
	for (i=0;i<dim+1;i++)
		if (i!=h)
			summa(x0,versh[i],x0);
	proizv(x0,(1/double(dim)),x0);

}

void Simpleks::otrazit(){                         //xr=x0+a*(x0-versh[h])

	zanulit(xr);
	proizv(versh[h],(-1),xr);                       //-versh[h]
	summa(x0,xr,xr);                                //x0-versh[h]
	proizv(xr,a,xr);                                //a*(x0-versh[h])
	summa(x0,xr,xr);                                //x0+a*(x0-versh[h])
}

void Simpleks::stretch(){                          //xe=x0+g*(xr-x0)
	zanulit(xe);
	proizv(x0,(-1),xe);                             //-x0
	summa(xe,xr,xe);                                //xr-x0
	proizv(xe,g,xe);                                //g*(xr-x0)
	summa(x0,xe,xe);                                //x0+g*(xr-x0)
}

void Simpleks::compres(){                          //xc=x0+b*(versh[h]-x0)
	zanulit(xc);
	proizv(x0,(-1),xc);                             //-x0
	summa(versh[h],xc,xc);                          //versh[h]-x0
	proizv(xc,b,xc);                                //b*(versh[h]-x0)
	summa(x0,xc,xc);                                //x0+g*(xr-x0)
}

void Simpleks::reject(){
	int i;
	VectPar t;

	t=new double[dim];
	for (i=0;i<dim+1;i++)
		if (i!=l){                                      //versh[i](íîâ)=versh[l]+b1*(versh[i]-versh[l])
			proizv(versh[l],(-1),t);                    //t=-versh[l]
			summa(versh[i],t,versh[i]);                 //versh[i]-versh[l]
			proizv(versh[i],b1,versh[i]);               //b1*(versh[i]-versh[l])
			summa(versh[l],versh[i],versh[i]);          //versh[l]+b1*(versh[i]-versh[l])
		}
	delete[] t;
}

bool Simpleks::shodim(){
	int i;
	int j;
	double s2;
	VectPar center;

	center=new double[dim];
	zanulit(center);
	for (i=0;i<dim+1;i++)
		summa(center,versh[i],center);
	proizv(center,(1/double(dim+1)),center);
	s2=0;
	for (i=0;i<dim+1;i++)
		for (j=0;j<dim;j++)
			s2=s2+(center[j]-versh[i][j])*(center[j]-versh[i][j]);
	s2=sqrt(s2);

return(s2<ep);
}

void Simpleks::steret(){
	int i;
	for (i=0;i<dim+1;i++)
		delete[] versh[i];
	delete[] versh;

	delete[] x0;
	delete[] xr;
	delete[] xe;
	delete[] xc;
	delete[] f_val;
	delete[] cpar1;
	if (cpar2!=NULL) delete [] cpar2;
}
