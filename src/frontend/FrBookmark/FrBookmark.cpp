#include "FrBookmark.h"

FrBookmark::FrBookmark(const QString &text ,QWidget *parent):QPushButton(text, parent){

	//QLabel *label = new QLabel(this);
	//label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	//label->setText(text);
	//label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
//	label->setFixedWidth(80);
//	label->setMaximumHeight(30);
	//setFlat(true);
}

void FrBookmark::mousePressEvent(QMouseEvent *event){
	//QMessageBox::information(this, "inside bookmark class", "bookmark clicked");
	
	emit bmClicked(*this);
}