#ifndef FR_ACTION_SIGNAL_MANAGER
#define FR_ACTION_SIGNAL_MANAGER

// Forward declaration
class FrMainWindow;

// includes
#include "FrMacro.h"
#include "Qt/QObject.h"


// This class incapsulates all actions
class FrActionSignalManager : public QObject {
    Q_OBJECT
public:
    FrActionSignalManager(FrMainWindow* mainWnd);
    
public slots:
    // File 
    void OnOpenImageAction();
    void OnSaveTabsAction();
    void OnLoadTabsAction();
    void OnExitAction();
    //Edit 
    void OnSaveToTabAction();
	void OnNewLayerAction();
	void OnDeleteLayerAction();
    void OnChangeLayerAction();
    //View
    void OnViewSliceAction();
    void OnViewMosaicAction();
    void OnViewOrthoAction();
    void OnResetImageAction();
    // Tools 
    void OnManipulatorToolAction();
    void OnVoxelToolAction();
    void OnRoiToolAction();
    // Help
    void OnShowHelpAction();
    void OnShowAboutAction();
    //Test
    void OnTestAction();

private:
    void Initialize();

private:
    FrMainWindow* m_mainWindow;
};

#endif