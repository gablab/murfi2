// class for graphs panel

#ifndef FR_GRAPHPANE_WIDGET
#define FR_GRAPHPANE_WIDGET

// Forward declarations
class QHBoxLayout;
class FrQwtPlotWidget;
class FrGraphListWidget;
class FrPlayControlWidget;
class FrMainDocument;

class FrPlayThread;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"

#include "Qt/qwidget.h"

class FrGraphPaneWidget : public QWidget {
	Q_OBJECT

public:
	FrGraphPaneWidget(QWidget *parent = 0, FrMainDocument* doc = 0);

    // Properties
    FrGetPropMacro(FrMainDocument*, Document);
    void SetDocument(FrMainDocument* doc);

    FrGetPropMacro(FrQwtPlotWidget*, QwtPlotWidget);
    FrGetPropMacro(FrGraphListWidget*, GraphListWidget);
    FrGetPropMacro(FrPlayControlWidget*, PlayControlWidget);
    
    // Methods
    void Update();
    void UpdateTimePoint();
    void UpdateLiveMode();

    // Here all signals
Q_SIGNALS:
    void TimePointChanged(int timePoint);
    void LiveModeChanged(bool value);

protected:
    virtual void timerEvent(QTimerEvent *event);

private Q_SLOTS:
    void OnGraphMarkerPositionChanged(int position);

    // Player processing
    void OnLiveModeChanged(bool);
    void OnPlayClicked();
    void OnPauseClicked();
    void OnResetClicked();
    void OnPlayFinished();

private:
    // Video playback
    int m_TimerID;
    int m_PlayingTimePoint;
    bool m_IsPaused;
};

#endif
