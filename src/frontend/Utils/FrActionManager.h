#ifndef FR_ACTION_MANAGER
#define FR_ACTION_MANAGER

// Froward declarations
class QAction;

// includes
#include "FrMacro.h"
#include "Qt/qobject.h"

// This class incapsulates all actions
class FrActionManager : public QObject {
    Q_OBJECT
public:
    FrActionManager(QObject* parent);
    ~FrActionManager();
    
    void Retranslate();

    // Properties
    // File
    FrGetPropMacro(QAction*, OpenImageAction);
    FrGetPropMacro(QAction*, OpenDataStoreAction);
    FrGetPropMacro(QAction*, SaveTabsAction);
    FrGetPropMacro(QAction*, LoadTabsAction);
    FrGetPropMacro(QAction*, ExitAction);    
    // Edit
    FrGetPropMacro(QAction*, SaveToTabAction);
    FrGetPropMacro(QAction*, NewLayerAction);
    FrGetPropMacro(QAction*, DeleteLayerAction);
    FrGetPropMacro(QAction*, ChangeLayerAction);
    FrGetPropMacro(QAction*, NewROIAction);
    //View
    FrGetPropMacro(QAction*, ViewSliceAction);
    FrGetPropMacro(QAction*, ViewMosaicAction);
    FrGetPropMacro(QAction*, ViewOrthoAction);
    FrGetPropMacro(QAction*, ResetImageAction);
    // Tools 
    FrGetPropMacro(QAction*, ManipulatorToolAction);
    FrGetPropMacro(QAction*, VoxelToolAction);
    FrGetPropMacro(QAction*, RoiToolAction);
    FrGetPropMacro(QAction*, VoxelSelectionToolAction);
    // Help
    FrGetPropMacro(QAction*, ShowHelpAction);
    FrGetPropMacro(QAction*, ShowAboutAction);

    // Test
    FrGetPropMacro(QAction*, TestAction);

private:
    void Initialize(QObject* parent);
};

#endif
