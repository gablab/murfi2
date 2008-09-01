#include "FrBookmarkWidget.h"

FrBookmarkWidget::FrBookmarkWidget(QWidget *parent):QTabWidget(parent){
	setTabPosition(QTabWidget::East);
	setTabShape(QTabWidget::Rounded);
	setMaximumWidth(50);
	
//	setMinimumWidth(100);
//	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//	QWidget *horizontalLayout;
//  horizontalLayout = new QWidget(this);
//    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
//    horizontalLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);;

//	layout = new QVBoxLayout(this);
//	layout->setContentsMargins(0, 0, 0, 0);
	//layout->setDirection(QBoxLayout::TopToBottom);

/*	addBookmarkButton = new QPushButton("Add bookmark", this);
	addBookmarkButton->setMaximumHeight(30);
	addBookmarkButton->setFixedWidth(80);

	//QPushButton* addBookmarkButton2 = new QPushButton("Add bookmark", this);
	//addBookmarkButton2->setMaximumHeight(30);
	//addBookmarkButton2->setFixedWidth(80);

	defaultTab = new FrBookmark("Default", this);
	defaultTab->setMaximumHeight(30);
	defaultTab->setFixedWidth(80);

	connect(this->defaultTab, SIGNAL(bmClicked(FrBookmark &)), this, SLOT(bookmarkClicked(FrBookmark &)));

	layout->addWidget(addBookmarkButton);
//	layout->addWidget(addBookmarkButton2);
	//layout->setAlignment(addBookmarkButton, Qt::AlignTop);
	layout->addWidget(defaultTab);
	//layout->setAlignment(defaultTab, Qt::AlignTop);
    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	layout->addItem(spacerItem);

	connect(this->addBookmarkButton, SIGNAL(clicked()), this, SLOT(addBookmark()));
*/

	//--------------------------------- test area -----------------------------------
/*	QTabWidget* tabWidget = new QTabWidget(parent);
    tabWidget->setTabPosition(QTabWidget::East);
    tabWidget->setTabShape(QTabWidget::Rounded);
    QWidget* tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tabWidget->addTab(tab, "Tab 1");
    QWidget* tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    tabWidget->addTab(tab_2, "Tab 2");*/
//	layout->addWidget(tabWidget);
	//--------------------------------- test area -----------------------------------
	bookmarks.clear();

    defaultTab = new FrBookmark(this);
	addTab(defaultTab, "Default");
	bookmarks.push_back(defaultTab);
	nob = 1;
}

// this method should be modified - we can send any data via this method 
// to create FrBookmark object with properties that we need
// (or send the whole FrBookmark object
void FrBookmarkWidget::addBookmark(){
	//QMessageBox::information(this, "Info", "Add bookmark button clicked");

	FrBookmark* bookmark = new FrBookmark(this);

//	layout->addWidget(bookmark);
//	layout->removeItem(spacerItem);
//	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
//	layout->addItem(spacerItem);

	//layout->setAlignment(bookmark, Qt::AlignTop);
	bookmarks.push_back(bookmark);

//	connect(this->bookmarks[nob], SIGNAL(bmClicked(FrBookmark &)), this, SLOT(bookmarkClicked(FrBookmark &)));

	addTab(bookmark, "Test"+QString::number(nob));
	nob++;
}

void FrBookmarkWidget::bookmarkClicked(FrBookmark &bookmark){
	QMessageBox::information(this, "Info", "bookmark clicked");
}
