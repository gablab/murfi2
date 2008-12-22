// class for storing bookmark info
#ifndef FR_PLAYCTL_WGT
#define FR_PLAYCTL_WGT

//Forward declarations
class QToolBar;
class QActionBar;
class QLabel;

// includes 
#include "FrMacro.h"

#include "Qt/qstring.h"
#include "Qt/qwidget.h"

class FrPlayControlWidget : public QWidget {
	Q_OBJECT
public:
    typedef enum _States { LifeMode, Normal, Playing, Paused } States;

public:
	FrPlayControlWidget(QWidget *parent = 0);

    // Properties
    FrGetPropMacro(int, TimePointPerSecond);
    FrGetPropMacro(States, State);
    void SetState(States state);

Q_SIGNALS:    
    // add signals here
    void LifeModeChanged(bool isOn);
    void Play();
    void Pause();
    void Reset();
    void Speedup();
    void Slowdown();

    void GotoNextStep();

private Q_SLOTS:
    void OnLifeModeChanged();
    void OnPlayPressed();
    void OnPausePressed();
    void OnResetPressed();
    void OnSlowerPressed();
    void OnFasterPressed();
     
    void UpdateButtons();
        
private:
    // Toolbars
    QToolBar* m_tbAddition;
    QToolBar* m_tbPlayback;
    QLabel* m_lblInfo;

    // Play toolbar actions
    QAction* m_LifeMode;
    QAction* m_Play;
    QAction* m_Pause;
    QAction* m_Reset;
    QAction* m_Faster;
    QAction* m_Slower;

    // Additional toolbar Actions...

};

#endif
