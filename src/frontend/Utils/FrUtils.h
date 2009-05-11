#ifndef FR_UTILS
#define FR_UTILS

#include "FrSettings.h"
#include "FrViewDocObj.h"
#include "FrSelection.h"
#include "FrMainDocument.h"
#include "FrLayerDocObj.h"

#include "vtkImageData.h"

// Backend includes
#include "RtMRIImage.h"
#include "RtDataID.h"

#include <vector>

#include <Qt/qfile.h>
#include <Qt/qstring.h>

#include "add_functions.h"

#define MY_PI 3.1415926854

// This function is used to ensure that
// value is in specified range [min..max].
template<class T>
T ClampValue(T value, T min, T max){
    if(value < min) value = min;
    else if(value > max) value = max;
    return value;
}

//// This functions helps to get layer settings
//void GetLayerSettings(FrSliceViewSettings*  sets, LayerCollection& layers);
//void GetLayerSettings(FrMosaicViewSettings* sets, LayerCollection& layers);
//void GetLayerSettings(FrOrthoViewSettings*  sets, LayerCollection& layers, int viewID);

// Utils
double GetLength(int x1, int y1, int x2, int y2);
void GetRealImagePosition(FrViewDocObj* viewDO, vtkImageData* data, int point[3], unsigned long imgNumber);
void GetRealImagePosition(FrViewDocObj* viewDO, vtkImageData* data, double point[3], unsigned long imgNumber);
bool IsPointInsideOfPolygon(std::vector<Pos> Points, Pos point);
bool IsPointInsideOfSphere(Pos center, int radius, Pos point);
bool IsImageAddedAsLayer(FrMainDocument* doc, RtDataID id);

#endif
