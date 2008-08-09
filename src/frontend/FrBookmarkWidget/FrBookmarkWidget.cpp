#include "FrBookmarkWidget.h"

FrBookmarkWidget::FrBookmarkWidget(QWidget *parent):QDialog(parent){
	setMinimumHeight(30);
//	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	addBookmarkButton = new QPushButton("Add bookmark", this);
	addBookmarkButton->setFixedWidth(80);
	addBookmarkButton->setFixedHeight(30);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(addBookmarkButton);

	connect(this->addBookmarkButton, SIGNAL(clicked()), this, SLOT(addBookmark()));
	
}

void FrBookmarkWidget::addBookmark()
{
	QMessageBox::information(this, "Info", "Add bookmark button clicked");
}