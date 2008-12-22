#ifndef FR_VOXELSELECTION_TOOL
#define FR_VOXELSELECTION_TOOL

// Forward declaration
class vtkRenderer;
class vtkPointPicker;

// Some includes
#include "FrTool.h"

// Voxel tool
class FrVoxelSelectionTool : public FrTool {
public:
	virtual void Start();
	virtual void Stop();
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params);
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params);

	virtual bool OnKeyPress(FrInteractorStyle* is, FrKeyParams& params);

public:
	/// Default constructor
	FrVoxelSelectionTool();
	/// Destructor
	virtual ~FrVoxelSelectionTool();

private:

private:
    vtkPointPicker* m_pointPicker;

};

#endif
