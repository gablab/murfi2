#include "FrMainMenu.h"
#include "FrActionManager.h"

// Qt
#include "Qt/QList.h"
#include "Qt/QString.h"
#include "Qt/QWidget.h"
#include "Qt/QAction.h"
#include "Qt/QApplication.h"



//defines
#define DO_TRANSLATE(context,name) \
    (QApplication::translate(context, name, 0, QApplication::UnicodeUTF8))

#define TR_CONTEXT "MainWindow"

FrMainMenu::FrMainMenu(QWidget *parent, FrActionManager* mngr)
: QMenuBar(parent), m_actManager(mngr){
    this->Initialize();
    this->SetupActions();
}

void FrMainMenu::Initialize(){
    // Create all elements of menu
    m_menuFile = new QMenu(this);
    m_menuFile->setObjectName(QString::fromUtf8("m_menuFile"));

    m_menuEdit = new QMenu(this);
    m_menuEdit->setObjectName(QString::fromUtf8("m_menuEdit"));

    m_menuView = new QMenu(this);
    m_menuView->setObjectName(QString::fromUtf8("m_menuView"));

    m_menuTools = new QMenu(this);
    m_menuTools->setObjectName(QString::fromUtf8("m_menuTools"));

    m_menuHelp = new QMenu(this);
    m_menuHelp->setObjectName(QString::fromUtf8("m_menuHelp"));

    this->addAction(m_menuFile->menuAction());
    this->addAction(m_menuEdit->menuAction());
    this->addAction(m_menuView->menuAction());
    this->addAction(m_menuTools->menuAction());
    this->addAction(m_menuHelp->menuAction());
}

void FrMainMenu::Retranslate(){
    // Set up correct names
    m_menuFile->setTitle(DO_TRANSLATE(TR_CONTEXT,  "File"));
    m_menuEdit->setTitle(DO_TRANSLATE(TR_CONTEXT,  "Edit"));
    m_menuView->setTitle(DO_TRANSLATE(TR_CONTEXT,  "View"));
    m_menuTools->setTitle(DO_TRANSLATE(TR_CONTEXT, "Tools"));
    m_menuHelp->setTitle(DO_TRANSLATE(TR_CONTEXT,  "Help"));
        
}

void FrMainMenu::SetupActions(){
    // File
    m_menuFile->addAction(m_actManager->GetOpenImageAction());
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actManager->GetSaveTabsAction());
    m_menuFile->addAction(m_actManager->GetLoadTabsAction());
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actManager->GetExitAction());
    
    // Edit
    m_menuEdit->addAction(m_actManager->GetSaveToTabAction());
	m_menuEdit->addAction(m_actManager->GetAddLayerAction());
	m_menuEdit->addSeparator();
    m_menuEdit->addAction(m_actManager->GetChangeThresholdAction());
    m_menuEdit->addAction(m_actManager->GetChangeColormapAction());

    // View
    m_menuView->addAction(m_actManager->GetViewSliceAction());
    m_menuView->addAction(m_actManager->GetViewMosaicAction());
    m_menuView->addAction(m_actManager->GetViewOrthoAction());

    // Tool
    m_menuTools->addAction(m_actManager->GetManipulatorToolAction());
    m_menuTools->addAction(m_actManager->GetVoxelToolAction());
    m_menuTools->addAction(m_actManager->GetRoiToolAction());
    
    // Help
    m_menuHelp->addAction(m_actManager->GetShowHelpAction());
    m_menuHelp->addSeparator();
    m_menuHelp->addAction(m_actManager->GetShowAboutAction());
}