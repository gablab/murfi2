#ifndef FR_ROI_TOOL
#define FR_ROI_TOOL

#include "FrBaseRoiTool.h"

class FrRoiInfoTool;
class FrPenTool;
class FrRectangleTool;
class FrFreeShapeTool;
class FrSphereTool;
class FrMaskMaskTool;
class FrInvertTool;
class FrDilatoneErosionTool;
class FrLayerSettings;
class vtkPointPicker;
class FrMainController;

#include <vector>

// ROI tool
class FrRoiTool : public FrBaseRoiTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

    void StartCurrentTool();

public:
	// Default constructor
	FrRoiTool();
	// Destructor
	virtual ~FrRoiTool();

private:
    FrMainController* GetMainController();
    bool GetMappedCoords(FrInteractorStyle* is, FrMouseParams& params);
    int GetVisibleLayer(std::vector<FrLayerSettings*> layers);
    void EnableRoiToolWidget();

private:
    // tools
    FrBaseRoiTool* m_curTool;
    FrRoiInfoTool*   m_riTool;
    FrPenTool*       m_penTool;
    FrRectangleTool* m_rectTool;
    FrFreeShapeTool* m_fsTool;
    FrSphereTool*    m_sphereTool;
    FrMaskMaskTool*  m_mmTool;
    FrInvertTool*    m_invTool;
    FrDilatoneErosionTool* m_deTool;
    
    vtkPointPicker* m_PointPicker;
    int m_ImgNumber;

};

#endif
