#ifndef FR_ROI_TOOL
#define FR_ROI_TOOL

#include "FrTool.h"

class FrMaskRectangleTool;
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
    FrMaskRectangleTool* m_maskRectTool;
    vtkPointPicker* m_PointPicker;

};

#endif
