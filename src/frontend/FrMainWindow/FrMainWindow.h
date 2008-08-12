#ifndef FR_MAIN_WINDOW
#define FR_MAIN_WINDOW


#include "ui_FrMainWindow.h"
#include "../QVTKWidget/QVTKWidget.h"
#include "../FrToolsPanel/FrToolsPanel.h"
#include "../FrToolsPanel/FrLowPanel.h"
#include "../FrBookmarkWidget/FrBookmarkWidget.h"
#include "Qt/qboxlayout.h"
#include "Qt/qscrollarea.h"
#include "Qt/qtabwidget.h"

// VTK stuff
#include "vtkImageViewer2.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkPNGReader.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"



class FrMainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
public:
	FrMainWindow(QWidget *parent = 0);

public slots:
	void brightnessValueChanged(int value);
	void contrastValueChanged(int value);
	void thresholdValueChanged(int value);

	void tool1Triggered();
	void tool2Triggered();
	void tool3Triggered();
	void mode1Clicked();
	void mode2Clicked();
	void mode3Clicked();


signals:


private:
	QScrollArea	*scrollAreaOfToolsPanel;

protected:


public:
	QVTKWidget* myQVTKWidget;
	FrToolsPanel* myToolsPanel;
	FrLowPanel* myLowPanel;
	FrBookmarkWidget* myBookmarkWidget;



};

#endif
