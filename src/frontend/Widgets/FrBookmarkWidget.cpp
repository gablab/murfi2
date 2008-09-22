#include "FrBookmarkWidget.h"
#include "FrBookmark.h"
#include "FrTabSettingsDocObj.h"
#include "FrCommandController.h"

#include "Qt/QToolButton.h"
#include "Qt/QTabWidget.h"
#include "QtGui/QDialog"
#include "Qt/QBoxLayout.h"
#include "Qt/QMessageBox.h"
#include "Qt/QAction.h"

// STL stuff
#include <algorithm>


FrBookmarkWidget::FrBookmarkWidget(QWidget *parent)
: QWidget(parent){

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabPosition(QTabWidget::East);
	m_tabWidget->setTabShape(QTabWidget::Rounded);
	m_tabWidget->setMaximumWidth(50);
    connect( m_tabWidget, SIGNAL(currentChanged(QWidget*)), 
             this, SLOT(OnCurrentChanged(QWidget*)) );
	
    m_btnCloseTab = new QToolButton(this);
    m_btnCloseTab->setText(trUtf8("x"));
    m_btnCloseTab->setToolTip(trUtf8("Close current tab"));
    connect( m_btnCloseTab, SIGNAL(clicked()), 
             this, SLOT(OnCloseButtonClicked()) ); 

    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(1);
    m_layout->addWidget(m_tabWidget);
    m_layout->addWidget(m_btnCloseTab);
    this->setLayout(m_layout);
}

bool FrBookmarkWidget::AddBookmark(FrTabSettingsDocObj* ts){
    FrBookmark* bookmark = new FrBookmark(this);
    bookmark->SetIsDefault(ts->GetIsDefault());
    ts->SetID(bookmark->GetID());
    
    m_tabWidget->addTab(bookmark, ts->GetName());
    // TODO: find the way how to setup tooltips
    //int idx = m_tabWidget->addTab(bookmark, ts->GetName());
    //m_tabWidget->setToolTip(idx, ts->GetDescription());

    if(ts->GetIsCurrent()){
        m_tabWidget->setCurrentWidget(bookmark);
    }
    return true;
}

bool FrBookmarkWidget::RemoveBookmark(FrTabSettingsDocObj* ts){

    return true;    
}

void FrBookmarkWidget::OnCloseButtonClicked(){
    // Close currently selected tab 
    FrBookmark* currentTab = dynamic_cast<FrBookmark*>(m_tabWidget->currentWidget());
    if(currentTab){
        if(currentTab != m_defaultTab){
                       
            // delete from internal list
            std::vector<FrBookmark*>::iterator elem;
            elem = std::find(m_bookmarks.begin(), m_bookmarks.end(), currentTab);
            m_bookmarks.erase(elem);
            
            // delete tab from TabWidget
            int idx = m_tabWidget->currentIndex();
            //m_tabWidget->removeToolTip(idx);
            m_tabWidget->removeTab(idx);

            // inform others
            emit DeleteTab(currentTab->GetID());
        }
        else{
            QMessageBox::information(this, "Information", "Can't close default tab...");
        }
    }
}

void FrBookmarkWidget::OnCurrentChanged(QWidget* page){
    // TODO: pass signal to document to keep track about current page
    FrBookmark* currentTab = dynamic_cast<FrBookmark*>(page);
    if(currentTab){
        FrUpdateTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
        cmd->SetAction(FrUpdateTabsCmd::UpdateCurrent);
        cmd->SetTabSettingsDocObj(0L);
        cmd->SetTabID( currentTab->GetID() );
        cmd->Execute();
        delete cmd;
    }
}