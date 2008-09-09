#ifndef FR_UTILS
#define FR_UTILS

#define M_PI 3.1415926854

// This function is used to ensure that
// value is in specified range [min..max].
template<class T>
T ClampValue(T value, T min, T max){
    if(value < min) value = min;
    else if(value > max) value = max;
    return value;
}

float Frange(float a, float minv, float maxv);
int  Irange(int a, int minv, int maxv);


#endif