#include "FrBookmarkWidget.h"

FrBookmarkWidget::FrBookmarkWidget(QWidget *parent):QDialog(parent){
	setMinimumHeight(30);
//	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//	QWidget *horizontalLayout;
//  horizontalLayout = new QWidget(this);
//    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
//    horizontalLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);;

	layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);

	addBookmarkButton = new QPushButton("Add bookmark", this);
	addBookmarkButton->setMaximumWidth(80);
	addBookmarkButton->setFixedHeight(30);


/*	bookmarks[0] = new FrBookmark("Test", this);
	bookmarks[0]->setFixedWidth(80);
	bookmarks[0]->setFixedHeight(30);

	layout->addWidget(bookmarks[0]);*/
	defaultTab = new FrBookmark("Default", this);
	defaultTab->setMaximumWidth(80);
	defaultTab->setFixedHeight(30);

	connect(this->defaultTab, SIGNAL(bmClicked(FrBookmark &)), this, SLOT(bookmarkClicked(FrBookmark &)));

	layout->addWidget(addBookmarkButton);
	layout->setAlignment(addBookmarkButton, Qt::AlignLeft);
	layout->addWidget(defaultTab);
//	layout->setAlignment(defaultTab, Qt::AlignRight);

	connect(this->addBookmarkButton, SIGNAL(clicked()), this, SLOT(addBookmark()));

	bookmarks.clear();
	nob = 0;
}

void FrBookmarkWidget::addBookmark()
{
	//QMessageBox::information(this, "Info", "Add bookmark button clicked");

	FrBookmark* bookmark = new FrBookmark("Test", this);
	bookmark->setMaximumWidth(80);
	bookmark->setFixedHeight(30);
	
	layout->addWidget(bookmark);
	bookmarks.push_back(bookmark);

	connect(this->bookmarks[nob], SIGNAL(bmClicked(FrBookmark &)), this, SLOT(bookmarkClicked(FrBookmark &)));
	nob++;
}

void FrBookmarkWidget::bookmarkClicked(FrBookmark &bookmark)
{
	QMessageBox::information(this, "Info", "bookmark clicked");
}
