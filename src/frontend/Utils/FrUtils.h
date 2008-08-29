#ifndef FR_UTILS
#define FR_UTILS


// This function is used to ensure that
// value is in specified range [min..max].
template<class T>
T ClampValue(T value, T min, T max){
    if(value < min) value = min;
    else if(value > max) value = max;
    return value;
}

#endif