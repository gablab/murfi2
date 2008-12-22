#ifndef FR_VOXEL_SELECTION_CMD
#define FR_VOXEL_SELECTION_CMD

// Forward declarations
class FrLayerSettings;
class FrMainDocument;
class vtkPointPicker;

// Some headers
#include "FrBaseCmd.h"
#include "FrMacro.h"

#include <vector>

// This command updates voxel information.
class FrVoxelSelectionCmd : public FrBaseCmd {
public:
    enum Action { Undefined, Add, Remove };

public:
    // Constructor/destructor
    FrVoxelSelectionCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    void SetMouseXY(int x, int y);
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(vtkPointPicker*, PointPicker);

private:
    bool AddPoint();
    bool RemovePoint();

    double* GetMappedPoint();
    int GetVisibleLayer(FrMainDocument* doc);

private:
    int m_mouseX;
    int m_mouseY;
    bool m_isMouseXY;

};

#endif // FR_VOXEL_SELECTION_CMD
