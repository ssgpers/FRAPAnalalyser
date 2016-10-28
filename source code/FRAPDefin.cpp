//---------------------------------------------------------------------------


#pragma hdrstop

#include "FRAPDefin.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void CFName::RenCopy(CFName _fnm){
	StrCopy(Name,"Copy of ");
	StrLCat(Name,_fnm.RetNm(),LenIntNm);
}
//---------------------------------------------------------------------------
void CFName::SetPNm(const char* _fpath){
	int pos;
	int i;
	int len;
	char s='a';
	pos=i=-1;
	while(s!='\0'){
		i++;
		s=_fpath[i];
		if (s=='\\')
			pos=i;
	}
	if ((i-pos)>LenIntNm)
		len=pos+LenIntNm;
	else
		len=i;
	pos++;
	for (i=pos;i<=len;i++)
		Name[i-pos]=_fpath[i];
	return;
}
