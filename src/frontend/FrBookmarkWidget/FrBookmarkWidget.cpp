#include "FrBookmarkWidget.h"

FrBookmarkWidget::FrBookmarkWidget(QWidget *parent):QDialog(parent){
	setMinimumWidth(100);
//	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//	QWidget *horizontalLayout;
//  horizontalLayout = new QWidget(this);
//    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
//    horizontalLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);;

	layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
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
	bookmarks.clear();
	nob = 0;
}

void FrBookmarkWidget::addBookmark(){
	//QMessageBox::information(this, "Info", "Add bookmark button clicked");

	FrBookmark* bookmark = new FrBookmark("Test", this);
	bookmark->setMaximumHeight(30);
	bookmark->setFixedWidth(80);

	layout->addWidget(bookmark);
	layout->removeItem(spacerItem);
//	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	layout->addItem(spacerItem);

	//layout->setAlignment(bookmark, Qt::AlignTop);
	bookmarks.push_back(bookmark);

	connect(this->bookmarks[nob], SIGNAL(bmClicked(FrBookmark &)), this, SLOT(bookmarkClicked(FrBookmark &)));
	nob++;
}

void FrBookmarkWidget::bookmarkClicked(FrBookmark &bookmark){
	QMessageBox::information(this, "Info", "bookmark clicked");
}
