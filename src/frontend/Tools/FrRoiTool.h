#ifndef FR_ROI_TOOL
#define FR_ROI_TOOL

#include "FrTool.h"

class FrRectangleTool;
class FrFreeShapeTool;
class FrLayerSettings;
class vtkPointPicker;

#include <vector>

// ROI tool
class FrRoiTool : public FrTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

public:
	// Default constructor
	FrRoiTool();
	// Destructor
	virtual ~FrRoiTool();

private:
    bool GetMappedCoords(FrInteractorStyle* is, FrMouseParams& params);
    int GetVisibleLayer(std::vector<FrLayerSettings*> layers);

private:
    // tools
    FrRectangleTool* m_maskRectTool;
    FrFreeShapeTool* m_maskFSTool;
    
    vtkPointPicker* m_PointPicker;
    int imgNumber;

};

#endif
