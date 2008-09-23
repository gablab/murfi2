#ifndef FR_MAIN_WINDOW
#define FR_MAIN_WINDOW

// Forward declaration
class QTabWidget;

class FrBaseView;
class FrSliceView;
class FrMosaicView;
class FrOrthoView;
class FrMainDocument;
class FrMainController;
class FrLowPanel;
class FrToolsPanel;
class FrBookmarkWidget;
class QVTKWidget;

#include "ui_FrMainWindow.h"
#include "FrMacro.h"

class FrMainWindow: public QMainWindow, public Ui::MainWindow {
	Q_OBJECT
public:
	FrMainWindow();
    ~FrMainWindow();

    FrPropMacro(FrMainDocument*,MainDocument);
    FrPropMacro(FrMainController*,MainController);
    
    QVTKWidget* GetQVTKWidget(){ return m_qtView; }
	void UpdateTBCValues(double contrast, double brightness);

    void Initialize();

    // Properties
    FrPropMacro(FrBaseView*,      CurrentView);
    FrGetPropMacro(FrSliceView*,  SliceView);
    FrGetPropMacro(FrMosaicView*, MosaicView);
    FrGetPropMacro(FrOrthoView*,  OrthoView);

    // GUI elements
    FrGetPropMacro(FrBookmarkWidget*, BookmarkWidget);

public slots:
	void brightnessValueChanged();
	void contrastValueChanged();
	void thresholdValueChanged();

	void tool1Triggered();
	void tool2Triggered();
	void tool3Triggered();
	void mode1Clicked();
	void mode2Clicked();
	void mode3Clicked();
	void tabChanged(int index);
	void saveToTab();
    void openImage();

signals:

private:
    void InitializeWidgets();
    void InitializeSignals();

public:
    QVTKWidget* m_qtView;
    QTabWidget* m_tabWidget;
	QWidget*    m_slice2DWidget;
	QWidget*    m_graphWidget;

	FrToolsPanel*   m_toolsPanel;
	FrLowPanel*     m_lowPanel;
};

#endif
