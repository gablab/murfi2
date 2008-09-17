#include "FrBookmarkWidget.h"
#include "FrBookmark.h"

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

	m_bookmarks.clear();

    m_defaultTab = new FrBookmark(this);    
    m_bookmarks.push_back(m_defaultTab);
	m_tabWidget->addTab(m_defaultTab, "Default");
}

void FrBookmarkWidget::AddBookmark(QString& tabName, QString& tabDescription){

    FrBookmark* bookmark = new FrBookmark(this);
    bookmark->SetName(tabName);
    bookmark->SetDescription(tabDescription);
	m_bookmarks.push_back(bookmark);
        
    int idx = m_tabWidget->addTab(bookmark, bookmark->GetName());
    //m_tabWidget->setToolTip(idx, bookmark->GetDescription());
}

void FrBookmarkWidget::BookmarkClicked(FrBookmark &bookmark){
	QMessageBox::information(this, "Info", "bookmark clicked");
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