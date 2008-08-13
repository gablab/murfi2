#include "FrBookmark.h"

FrBookmark::FrBookmark(QWidget *parent):QWidget(parent){

	//QLabel *label = new QLabel(this);
	//label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	//label->setText(text);
	//label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
//	label->setFixedWidth(80);
//	label->setMaximumHeight(30);
	//setFlat(true);
	//setGeometry(0, 0, 0, 0);
}

void FrBookmark::mousePressEvent(QMouseEvent *event){
	//QMessageBox::information(this, "inside bookmark class", "bookmark clicked");
	
	emit bmClicked(*this);
}