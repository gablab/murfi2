#ifndef FR_ACTION_MANAGER
#define FR_ACTION_MANAGER

// Froward declarations
class QAction;

// includes
#include "FrMacro.h"
#include "Qt/QObject.h"

// This class incapsulates all actions
class FrActionManager : public QObject {
    Q_OBJECT
public:
    FrActionManager(QObject* parent);
    
    void Retranslate();

    // Properties
    // File
    FrGetPropMacro(QAction*, OpenImageAction);
    FrGetPropMacro(QAction*, SaveTabsAction);
    FrGetPropMacro(QAction*, LoadTabsAction);
    FrGetPropMacro(QAction*, ExitAction);    
    // Edit
    FrGetPropMacro(QAction*, SaveToTabAction);
    FrGetPropMacro(QAction*, ChangeThresholdAction);
    FrGetPropMacro(QAction*, ChangeColormapAction);
    //View
    FrGetPropMacro(QAction*, ViewSliceAction);
    FrGetPropMacro(QAction*, ViewMosaicAction);
    FrGetPropMacro(QAction*, ViewOrthoAction);
    // Tools 
    FrGetPropMacro(QAction*, ManipulatorToolAction);
    FrGetPropMacro(QAction*, VoxelToolAction);
    FrGetPropMacro(QAction*, RoiToolAction);
    // Help
    FrGetPropMacro(QAction*, ShowHelpAction);
    FrGetPropMacro(QAction*, ShowAboutAction);

    // Test
    FrGetPropMacro(QAction*, TestAction);

private:
    void Initialize(QObject* parent);
};

#endif