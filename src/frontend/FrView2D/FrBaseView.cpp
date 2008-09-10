#include "FrBaseView.h"
#include "FrMainWindow.h"

FrBaseView::FrBaseView(FrMainWindow* mainWindow){
    m_MainWindow = mainWindow;    
    m_QtView = mainWindow->GetQVTKWidget();
}

FrBaseView::~FrBaseView(){
}
