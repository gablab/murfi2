#ifndef FR_POINTSDOC_OBJ
#define FR_POINTSDOC_OBJ

// Forward declarations
class FrPoint;
class QString;
class vtkImageData;
class RtMRIImage;

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrGraphSettings.h"

#include <vector>

class FrPointsDocObj : public FrDocumentObj {
public:
   typedef std::vector<FrPoint*> PointCollection; 
public:
    FrPointsDocObj();
    //FrPointsDocObj(FrPointsDocObj* src);
    virtual ~FrPointsDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();
    
    void AddPoint(FrPoint* point);
    void RemovePoint(FrPoint* point);
    void GetDimsFromImg(RtMRIImage* img);
    void CopySettings(FrPointsDocObj* src);
    void ClearAll();

    // Properties
//    FrGetPropMacro(vtkImageData*, PointsData);
    FrGetPropMacro(PointCollection, Points);

    vtkImageData* GetPointsXY(int z);
    vtkImageData* GetPointsXZ(int y);
    vtkImageData* GetPointsYZ(int x);
    vtkImageData* GetMosaicData(int dimx, int dimy);

    // method to obtain one point (since we have only 1 point at the same time)
    void GetPoint(int* point);

private:
    void InitTransparentData();

private:
    //PointCollection m_Points;
    double m_spacing[3];
    int m_dimensions[3];

};

#endif

