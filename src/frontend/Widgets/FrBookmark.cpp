#include "FrBookmark.h"

#include "Qt/QMessageBox.h"

// Defines
#define DEF_TAB_NAME        "NoName"
#define DEF_TAB_DESCRIPTION "No description"

// NOTE: assume that int will be enought
int FrBookmark::currentID = 0;

FrBookmark::FrBookmark(QWidget *parent)
: QWidget(parent){

    //setup id
    m_ID = currentID;
    currentID++;

    // set up default name and description
    m_name = DEF_TAB_NAME;
    m_name += QString::number(m_ID);

    m_description = DEF_TAB_DESCRIPTION;
}

void FrBookmark::mousePressEvent(QMouseEvent *event){		
	emit Clicked(*this);
}

int FrBookmark::GetID(){ 
    return m_ID;
}
        
QString& FrBookmark::GetName(){
    return m_name;
}

void FrBookmark::SetName(QString& value){    
    if(!value.isEmpty()){
        m_name = value;
    }
    else{
        m_name = DEF_TAB_NAME;
        m_name += QString::number(m_ID);
    }
    emit NameChanged(value);
}

QString& FrBookmark::GetDescription(){
    return m_description;
}

void FrBookmark::SetDescription(QString& value){
    m_description = ( value.isEmpty() ) ? 
        DEF_TAB_DESCRIPTION : value;
}