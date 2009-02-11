#ifndef FR_VOXEL_INFO_CMD
#define FR_VOXEL_INFO_CMD

// Forward declarations
class FrLayerSettings;
class FrMainDocument;
class vtkPointPicker;
class vtkImageData;

// Some headers
#include "FrBaseCmd.h"
#include "FrMacro.h"
#include "FrVoxelInfoWidget.h"
#include <vector>

// This command updates voxel information.
class FrVoxelInfoCmd : public FrBaseCmd {
public:
  enum Action { Undefined, Update, Reset, Add, Set };

public:
    // Constructor/destructor
    FrVoxelInfoCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    void SetMouseXY(int x, int y);
    void SetVoxel(int v[3]);
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(vtkPointPicker*, PointPicker);

private:
    bool UpdateVoxelInfoMouse(bool addPoint);
    bool UpdateVoxelInfoDirect(bool addPoint);
    bool UpdateVoxelInfo(VoxelData &vd, vtkImageData* pImageData, bool addPoint);
    bool ResetVoxelInfo();
    bool AddPoint(int* Index, vtkImageData* pImageData = NULL);

    void GetVoxelInfo();
    double* GetMappedPoint();
    int GetVisibleLayer(FrMainDocument* doc);

private:
    int m_mouseX;
    int m_mouseY;
    bool m_isMouseXY;

    int v[3];
    bool m_isVoxel;
};

#endif // FR_VOXEL_INFO_CMD
