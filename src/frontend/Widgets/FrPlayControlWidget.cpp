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
    : QWidget(parent), 
      m_TimePointPerSecond(DEFAULT_TP_PER_SEC),
      m_State(FrPlayControlWidget::LifeMode){ 

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

    m_lblInfo = new QLabel(QString("%1 TP / sec").
        arg(m_TimePointPerSecond), 
        m_tbPlayback);

    m_lblInfo->setFrameShape(QFrame::Box);
    m_tbPlayback->addWidget(m_lblInfo);
    
    this->setFixedHeight(this->sizeHint().height());

    // HACK: this sets up buttons
    this->UpdateButtons();

    // connect signals
    CONNECT_ACTION_TRIGGERED(m_LifeMode, OnLifeModeChanged());
    CONNECT_ACTION_TRIGGERED(m_Play, OnPlayPressed());
    CONNECT_ACTION_TRIGGERED(m_Pause, OnPausePressed());
    CONNECT_ACTION_TRIGGERED(m_Reset, OnResetPressed());
    CONNECT_ACTION_TRIGGERED(m_Slower, OnSlowerPressed());
    CONNECT_ACTION_TRIGGERED(m_Faster, OnFasterPressed());
}

void FrPlayControlWidget::SetState(States state){

    if(m_State != state){

        m_State = state;
        this->UpdateButtons();
    }
}

void FrPlayControlWidget::OnLifeModeChanged(){
    
    States state = (m_State == LifeMode) ? Normal : LifeMode;
    this->SetState(state);

    emit LifeModeChanged(state == LifeMode);
}
    
void FrPlayControlWidget::OnPlayPressed(){

    this->SetState(Playing);
    emit Play();
}

void FrPlayControlWidget::OnPausePressed(){

    this->SetState(Paused);
    emit Pause();
}

void FrPlayControlWidget::OnResetPressed(){

    this->SetState(Normal);
    emit Reset();
}

void FrPlayControlWidget::OnSlowerPressed(){

    if(m_TimePointPerSecond > MIN_TP_PER_SEC){
        
        m_TimePointPerSecond--;
        m_lblInfo->setText(QString("%1 TP / sec").arg(m_TimePointPerSecond));
        m_Slower->setEnabled(m_TimePointPerSecond > MIN_TP_PER_SEC);
        m_Faster->setEnabled(m_TimePointPerSecond < MAX_TP_PER_SEC);

        emit Slowdown();
    }
}

void FrPlayControlWidget::OnFasterPressed(){

    if(m_TimePointPerSecond < MAX_TP_PER_SEC){
        
        m_TimePointPerSecond++;
        m_lblInfo->setText(QString("%1 TP / sec").arg(m_TimePointPerSecond));
        m_Faster->setEnabled(m_TimePointPerSecond < MAX_TP_PER_SEC);
        m_Slower->setEnabled(m_TimePointPerSecond > MIN_TP_PER_SEC);

        emit Speedup();
    }
}

void FrPlayControlWidget::UpdateButtons(){
    
    switch(m_State) {
        case LifeMode:
            {
                // For life mode everithing is off
                m_Play->setEnabled(false);
                m_Play->setChecked(false);
                m_Pause->setEnabled(false);
                m_Pause->setChecked(false);
                m_Reset->setEnabled(false);
                m_Faster->setEnabled(false);
                m_Slower->setEnabled(false);
                m_lblInfo->setEnabled(false);
            }
            break;
        case Normal:
            {   
                m_Play->setEnabled(true);
                m_Play->setChecked(false);
                m_Pause->setEnabled(false);
                m_Pause->setChecked(false);
                m_Reset->setEnabled(true);
                m_Faster->setEnabled(m_TimePointPerSecond < MAX_TP_PER_SEC);
                m_Slower->setEnabled(m_TimePointPerSecond > MIN_TP_PER_SEC);
                m_lblInfo->setEnabled(true);
            }
            break;
        case Playing:
            {
                m_Play->setEnabled(false);
                m_Play->setChecked(true);
                m_Pause->setEnabled(true);
                m_Pause->setChecked(false);
                m_Reset->setEnabled(false);
                m_Faster->setEnabled(false);
                m_Slower->setEnabled(false);
                m_lblInfo->setEnabled(true);
            }
            break;
        case Paused:
            {
                m_Play->setEnabled(true);
                m_Play->setChecked(false);
                m_Pause->setEnabled(false);
                m_Pause->setChecked(true);
                m_Reset->setEnabled(true);
                m_Faster->setEnabled(m_TimePointPerSecond < MAX_TP_PER_SEC);
                m_Slower->setEnabled(m_TimePointPerSecond > MIN_TP_PER_SEC);
                m_lblInfo->setEnabled(false);
            }
            break;
    }
}