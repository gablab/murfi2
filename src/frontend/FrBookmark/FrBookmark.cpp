#include "FrBookmark.h"

FrBookmark::FrBookmark(const QString &text ,QWidget *parent):QWidget(parent){

	QLabel *label = new QLabel(this);
	label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	label->setText(text);
	label->setAlignment(Qt::AlignBottom | Qt::AlignRight); 
}

void FrBookmark::mousePressEvent(QMouseEvent *event)
{
	//QMessageBox::information(this, "inside bookmark class", "bookmark clicked");
	
	emit bmClicked(*this);
}