#ifndef FR_UTILS
#define FR_UTILS

#include "FrSettings.h"
#include "FrTabSettingsDocObj.h"
#include "FrSelection.h"

#include "vtkImageData.h"

#include <vector>

#define MY_PI 3.1415926854

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
int Min(int a, int b);
int Max(int a, int b);

// This functions helps to get layer settings
void GetLayerSettings(FrSliceViewSettings*  sets, LayerCollection& layers);
void GetLayerSettings(FrMosaicViewSettings* sets, LayerCollection& layers);
void GetLayerSettings(FrOrthoViewSettings*  sets, LayerCollection& layers, int viewID);

// Utils
double GetLength(int x1, int y1, int x2, int y2);
void GetRealImagePosition(FrTabSettingsDocObj* ts, vtkImageData* data, int point[3], int imgNumber);
bool IsPointInsideOfPolygon(std::vector<Pos> Points, Pos point);
bool IsPointInsideOfSphere(Pos center, int radius, Pos point);

#endif
