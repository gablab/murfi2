#include "FrBookmark.h"

#include "Qt/QMessageBox.h"

// Defines
#define DEF_TAB_NAME        "NoName"
#define DEF_TAB_DESCRIPTION "No description"

// NOTE: assume that int will be enought
int FrBookmark::s_CurrentID = 0;

FrBookmark::FrBookmark(QWidget *parent)
: QWidget(parent){

    //setup id
    m_ID = s_CurrentID;
    s_CurrentID++;

    //// set up default name and description
    //m_name = DEF_TAB_NAME;
    //m_name += QString::number(m_ID);
    //m_description = DEF_TAB_DESCRIPTION;
}

void FrBookmark::mousePressEvent(QMouseEvent *event){
	emit Clicked(*this);
}