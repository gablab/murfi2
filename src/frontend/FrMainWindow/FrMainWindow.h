#ifndef FR_MAIN_WINDOW
#define FR_MAIN_WINDOW

// Forward declaration
class QWidget;
class QStatusBar;

class FrBaseView;
class FrSliceView;
class FrMosaicView;
class FrOrthoView;
class FrMainDocument;
class FrMainController;
class FrToolBar;
class FrMainMenu;
class FrBookmarkWidget;
class QVTKWidget;
class FrActionManager;
class FrMainWindowSignalManager;

// Includes
#include "FrMacro.h"
#include "Qt/QMainWindow.h"

// This is main window of application
class FrMainWindow: public QMainWindow {
	Q_OBJECT
public:
	FrMainWindow();
    ~FrMainWindow();

    FrPropMacro(FrMainDocument*,MainDocument);
    FrPropMacro(FrMainController*,MainController);
    

    void Initialize();

    // Properties
    FrPropMacro(FrBaseView*,      CurrentView);
    FrGetPropMacro(FrSliceView*,  SliceView);
    FrGetPropMacro(FrMosaicView*, MosaicView);
    FrGetPropMacro(FrOrthoView*,  OrthoView);

    // GUI elements
    FrGetPropMacro(QVTKWidget*, QVTKWidget);
    FrGetPropMacro(FrBookmarkWidget*, BookmarkWidget);

public slots:
    void OnBookmarkChanged(int id);
    void OnBookmarkDelete(int id);

private:
    void SetupUi(QMainWindow* mainWindow);
    void RetranslateUi(QMainWindow* mainWindow);
    void InitializeWidgets();

private:
    FrActionManager* m_actManager;
    QWidget*    m_centralWidget;
    FrMainMenu* m_mainMenu;
    FrToolBar*  m_toolBar;
    QStatusBar* m_statusBar;

    friend class FrActionSignalManager;
    FrActionSignalManager* m_signalManager;
};

#endif
