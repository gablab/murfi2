#include "FrBaseView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"

#include "vtkRenderWindow.h"

FrBaseView::FrBaseView(FrMainWindow* mainWindow){
    m_MainWindow = mainWindow;    
    m_QtView = mainWindow->GetQVTKWidget();
    m_RenderWindow = m_QtView->GetRenderWindow();
}

FrBaseView::~FrBaseView(){
}
