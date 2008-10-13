#include "add_functions.h"

double rint( double x)
{
    if( x > 0 ) 
	{
        __int64 xint = (__int64) (x+0.5);
        if( xint % 2 ) 
		{
            // then we might have an even number...
            double diff = x - (double)xint;
            if( diff == -0.5 )
                return double(xint-1);
        }
        return double(xint);
    } 
	else 
	{
        __int64 xint = (__int64) (x-0.5);
        if( xint % 2 ) 
		{
            // then we might have an even number...
            double diff = x - (double)xint;
            if( diff == 0.5 )
                return double(xint+1);
        }
        return double(xint);
    }
} 

