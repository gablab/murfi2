#include "FrToolBar.h"
#include "FrActionManager.h"

//Qt
#include "Qt/QWidget.h"
#include "Qt/QApplication.h"

FrToolBar::FrToolBar(QWidget* parent, FrActionManager* mngr)
: QToolBar(parent), m_actManager(mngr){
    this->SetupActions();
}

void FrToolBar::Retranslate(){
    this->setWindowTitle(
        QApplication::translate("MainWindow", "MRI", 0, 
        QApplication::UnicodeUTF8));
}

void FrToolBar::SetupActions(){
    // File
    this->addAction(m_actManager->GetOpenImageAction());
    this->addSeparator();
    
    // View
    this->addAction(m_actManager->GetViewSliceAction());
    this->addAction(m_actManager->GetViewMosaicAction());
    this->addAction(m_actManager->GetViewOrthoAction());
    this->addSeparator();
    
    //Tool
    this->addAction(m_actManager->GetManipulatorToolAction());
    this->addAction(m_actManager->GetVoxelToolAction());
    this->addAction(m_actManager->GetRoiToolAction());
    this->addSeparator();

    // Help 
    this->addAction(m_actManager->GetShowAboutAction());
    this->addSeparator();

    // Test
    this->addAction(m_actManager->GetTestAction());
}