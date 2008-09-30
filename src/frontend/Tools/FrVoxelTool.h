#ifndef FR_VOXEL_TOOL
#define FR_VOXEL_TOOL

// Forward declaration
class vtkRenderer;
class vtkPointPicker;

// Some includes
#include "FrTool.h"

// Voxel tool
class FrVoxelTool : public FrTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

public:
	/// Default constructor
	FrVoxelTool();
	/// Destructor
	virtual ~FrVoxelTool();

private:
    int GetPickedPointIndex(int x, int y, vtkRenderer* renderer);

private:
    vtkPointPicker* m_pointPicker;
};

#endif
