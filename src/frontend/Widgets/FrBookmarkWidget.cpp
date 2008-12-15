#include "FrBookmarkWidget.h"
#include "FrBookmark.h"
#include "FrTabSettingsDocObj.h"

#include "Qt/qtoolbutton.h"
#include "Qt/qtabwidget.h"
#include "Qt/qdialog.h"
#include "Qt/qboxlayout.h"
#include "Qt/qmessagebox.h"
#include "Qt/qaction.h"

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
    FrBookmark* bookmark = new FrBookmark(ts->GetID(), this);
    bookmark->SetIsDefault(ts->GetIsDefault());
        
    m_tabWidget->addTab(bookmark, ts->GetName());
    // TODO: find the way how to setup tooltips
    //int idx = m_tabWidget->addTab(bookmark, ts->GetName());
    //m_tabWidget->setToolTip(idx, ts->GetDescription());

    if(ts->GetIsCurrent()){
        m_tabWidget->setCurrentWidget(bookmark);
    }

    // HACK: Catch this signal and init our app
    if(m_tabWidget->count()==1) {
		emit CurrentChanged(bookmark->GetID());
    }
    return true;
}

bool FrBookmarkWidget::RemoveBookmark(FrTabSettingsDocObj* ts){

    // delete tab from TabWidget
    unsigned int id = ts->GetID();
    for(int i=0; i < m_tabWidget->count(); ++i){
        FrBookmark* bmw = dynamic_cast<FrBookmark*>(m_tabWidget->widget(i));
        if(bmw != 0L && id == bmw->GetID()){
            // TODO: find somethinig for tooltip
            //m_tabWidget->removeToolTip(i);
            m_tabWidget->removeTab(i);
        }
    }    
    return true;    
}

void FrBookmarkWidget::OnCloseButtonClicked(){
    // Close currently selected tab 
    FrBookmark* currentTab = dynamic_cast<FrBookmark*>(m_tabWidget->currentWidget());
    if(currentTab){
        // inform others
        emit DeleteTab( currentTab->GetID() );
    }
}

void FrBookmarkWidget::OnCurrentChanged(QWidget* page){
    // pass signal to document to keep track about current page
    FrBookmark* currentTab = dynamic_cast<FrBookmark*>(page);
    if(currentTab){
        // inform others
        emit CurrentChanged( currentTab->GetID() );
    }
}

int FrBookmarkWidget::GetBookmarkCount(){
    return m_tabWidget->count();
}
