#include "FrBookmark.h"

#include "Qt/qmessagebox.h"

// Defines
#define DEF_TAB_NAME        "NoName"
#define DEF_TAB_DESCRIPTION "No description"

FrBookmark::FrBookmark(unsigned long id, QWidget *parent)
: QWidget(parent){

    //setup id
    m_ID = id;

    //// set up default name and description
    //m_name = DEF_TAB_NAME;
    //m_name += QString::number(m_ID);
    //m_description = DEF_TAB_DESCRIPTION;
}

void FrBookmark::mousePressEvent(QMouseEvent *event){
	emit Clicked(*this);
}
