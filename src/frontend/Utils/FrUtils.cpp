#include "FrUtils.h"

float Frange(float a, float minv, float maxv){
	if (a<minv) 
		a=minv;
	if (a>maxv) 
		a=maxv;
	return a;
}

int  Irange(int a, int minv, int maxv){
	if (a<minv) 
		a=minv;
	if (a>maxv)
		a=maxv;
	return a;
}