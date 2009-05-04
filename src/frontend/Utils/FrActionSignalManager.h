#ifndef FR_ACTION_SIGNAL_MANAGER
#define FR_ACTION_SIGNAL_MANAGER

// Forward declaration
class FrMainWindow;

// includes
#include "FrMacro.h"
#include "Qt/qobject.h"


// This class incapsulates all actions
class FrActionSignalManager : public QObject {
    Q_OBJECT
public:
    FrActionSignalManager(FrMainWindow* mainWnd);
    virtual ~FrActionSignalManager();
    
public slots:
    // File 
    void OnOpenImageAction();
    void OnOpenConfigAction();
    void OnOpenDataStoreAction();
    void OnSaveTabsAction();
    void OnLoadTabsAction();
    void OnSaveGraphTabsAction();
    void OnLoadGraphTabsAction();
    void OnExitAction();
    //Edit 
    void OnSaveToTabAction();
    void OnSaveGraphToTabAction();
	void OnNewLayerAction();
	void OnDeleteLayerAction();
    void OnChangeLayerAction();
    void OnNewROIAction();
    //View
    void OnViewSliceAction();
    void OnViewMosaicAction();
    void OnViewOrthoAction();
    void OnResetImageAction();
    // Tools 
    void OnManipulatorToolAction();
    void OnVoxelToolAction();
    void OnRoiToolAction();
    void OnVoxelSelectionToolAction();
    // Help
    void OnShowHelpAction();
    void OnShowAboutAction();
    //Test
    void OnTestAction();
    // Other stuff
    void OnLayerParamsChanged();
    void OnLayerColormapChanged();
    void OnLayerPositionChanged(int inc);
    void OnRoiToolChanged();
    void OnImageParamsChanged();
    // Graph pane
    void OnLiveModeChanged(bool currentValue);
    void OnTimePointChanged(int currentTimePoint);
    void OnPreviousTimePointPressed();
    void OnNextTimePointPressed();
    void OnGraphChanged(unsigned long id, bool add);

public:
    void Initialize();
    void Deinitialize();

private:
    FrMainWindow* m_mainWindow;
    bool m_isInit;
};

#endif
