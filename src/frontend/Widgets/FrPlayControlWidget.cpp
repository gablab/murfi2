#include "FrPlayControlWidget.h"
#include "Qt/qtoolbar.h"

#include "Qt/qstring.h"
#include "Qt/qlayout.h"
#include "Qt/qaction.h"
#include "Qt/qlabel.h"

// defines
#define DEFAULT_TP_PER_SEC 1
#define MAX_TP_PER_SEC 25
#define MIN_TP_PER_SEC 1

#define CONNECT_ACTION_TRIGGERED(action, slot)\
    connect(action,SIGNAL(triggered()),this,SLOT(slot))


FrPlayControlWidget::FrPlayControlWidget(QWidget *parent)
: QWidget(parent), m_tpPerSecond(DEFAULT_TP_PER_SEC){ 

    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Create toolbars
    m_tbAddition = new QToolBar(this);
    m_tbAddition->setAcceptDrops(false);
    m_tbAddition->setMovable(false);
    m_tbAddition->setFixedHeight(30);

    m_tbPlayback = new QToolBar(this);
    m_tbPlayback->setAcceptDrops(false);
    m_tbPlayback->setMovable(false);
    m_tbPlayback->setFixedHeight(30);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_tbAddition);
    layout->addWidget(m_tbPlayback);

    // Init actions for playback toolbar 
    m_LifeMode = m_tbPlayback->addAction("Life Mode");
    m_LifeMode->setCheckable(true);
    m_tbPlayback->addSeparator();

    m_Play = m_tbPlayback->addAction("Play");
    m_Play->setCheckable(true);
    m_Pause = m_tbPlayback->addAction("Pause");
    m_Pause->setCheckable(true);
    m_Reset = m_tbPlayback->addAction("Reset");
    m_tbPlayback->addSeparator();

    m_Slower = m_tbPlayback->addAction("Slower");
    m_Faster = m_tbPlayback->addAction("Faster");

    m_lblInfo = new QLabel(QString("%1 TP / sec").arg(m_tpPerSecond), m_tbPlayback);
    m_lblInfo->setFrameShape(QFrame::Box);
    m_tbPlayback->addWidget(m_lblInfo);
    
    this->setFixedHeight(this->sizeHint().height());

    // Emulate click 
    m_LifeMode->setChecked(true);
    this->OnLifeModeChanged();

    // connect signals
    CONNECT_ACTION_TRIGGERED(m_LifeMode, OnLifeModeChanged());
    CONNECT_ACTION_TRIGGERED(m_Play, OnPlayPressed());
    CONNECT_ACTION_TRIGGERED(m_Pause, OnPausePressed());
    CONNECT_ACTION_TRIGGERED(m_Reset, OnResetPressed());
    CONNECT_ACTION_TRIGGERED(m_Slower, OnSlowerPressed());
    CONNECT_ACTION_TRIGGERED(m_Faster, OnFasterPressed());
}

void FrPlayControlWidget::OnLifeModeChanged(){
    bool notPressed = !m_LifeMode->isChecked();
    m_Play->setEnabled(notPressed);
    m_Pause->setEnabled(notPressed);
    m_Reset->setEnabled(notPressed);
    m_Faster->setEnabled(notPressed && (m_tpPerSecond < MAX_TP_PER_SEC));
    m_Slower->setEnabled(notPressed && (m_tpPerSecond > MIN_TP_PER_SEC));
    m_lblInfo->setEnabled(notPressed);

    if(notPressed){
        m_Play->setChecked(false);
        m_Pause->setChecked(false);
    }
    emit LifeModeChanged(!notPressed);
}
    
void FrPlayControlWidget::OnPlayPressed(){
    if(m_Pause->isChecked()){
        m_Pause->setChecked(false);

        emit Play();
    }
    else{
        m_Play->setChecked(true);
    }
}

void FrPlayControlWidget::OnPausePressed(){
    if(m_Play->isChecked()){
        m_Play->setChecked(false);
        emit Pause();
    }
    else{
        m_Pause->setChecked(true);
    }
}

void FrPlayControlWidget::OnResetPressed(){
    m_Play->setChecked(false);
    m_Pause->setChecked(false);
    emit Reset();
}

void FrPlayControlWidget::OnSlowerPressed(){

    if(m_tpPerSecond > MIN_TP_PER_SEC){
        m_tpPerSecond--;
        emit Slowdown();
    }
    m_Slower->setEnabled(m_tpPerSecond > MIN_TP_PER_SEC);
    m_Faster->setEnabled(m_tpPerSecond < MAX_TP_PER_SEC);
}

void FrPlayControlWidget::OnFasterPressed(){

    if(m_tpPerSecond < MAX_TP_PER_SEC){
        m_tpPerSecond++;
        emit Speedup();
    }
    m_Faster->setEnabled(m_tpPerSecond < MAX_TP_PER_SEC);
    m_Slower->setEnabled(m_tpPerSecond > MIN_TP_PER_SEC);
}
