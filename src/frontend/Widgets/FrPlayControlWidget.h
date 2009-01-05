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
    typedef enum _States { LiveMode, Normal, Playing, Paused } States;

public:
	FrPlayControlWidget(QWidget *parent = 0);

    void SetAdditionalInfo(QString& text);

    // Properties
    FrGetPropMacro(int, TimePointPerSecond);
    FrGetPropMacro(States, State);
    bool SetState(States state);

Q_SIGNALS:    
    // add signals here
    void LiveModeChanged(bool isOn);
    void Play();
    void Pause();
    void Reset();
    void Previous();
    void Next();
    void Speedup();
    void Slowdown();

    void GotoNextStep();

private Q_SLOTS:
    void OnLiveModeChanged();
    void OnPlayPressed();
    void OnPausePressed();
    void OnResetPressed();
    void OnPreviousPressed();
    void OnNextPressed();
    void OnSlowerPressed();
    void OnFasterPressed();
     
    void UpdateButtons();
        
private:
    // Toolbars
    QToolBar* m_tbAddition;
    QToolBar* m_tbPlayback;
    QLabel* m_lblInfo;
    QLabel* m_lblAdditional;

    // Play toolbar actions
    QAction* m_LiveMode;
    QAction* m_Play;
    QAction* m_Pause;
    QAction* m_Reset;
    QAction* m_Previous;
    QAction* m_Next;
    QAction* m_Faster;
    QAction* m_Slower;

    // Additional toolbar Actions...

};

#endif
