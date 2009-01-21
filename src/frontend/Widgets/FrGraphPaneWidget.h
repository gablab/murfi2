// class for graphs panel

#ifndef FR_GRAPHPANE_WIDGET
#define FR_GRAPHPANE_WIDGET

// Forward declarations
class QHBoxLayout;
class FrQwtPlotWidget;
class FrGraphListWidget;
class FrPlayControlWidget;
class FrMainDocument;
class FrGraphContextMenu;
class FrGraphDocObj;
class FrGraphBookmarkWidget;

class FrPlayThread;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"

#include "Qt/qwidget.h"
#include "Qt/qmutex.h"

class FrGraphPaneWidget : public QWidget {
	Q_OBJECT

public:
	FrGraphPaneWidget(QWidget *parent = 0, FrMainDocument* doc = 0);

    // Properties
    FrGetPropMacro(FrMainDocument*, Document);
    void SetDocument(FrMainDocument* doc);

    FrGetPropMacro(FrQwtPlotWidget*, QwtPlotWidget);
    //FrGetPropMacro(FrGraphListWidget*, GraphListWidget);
    FrGetPropMacro(FrPlayControlWidget*, PlayControlWidget);
    FrGetPropMacro(FrGraphContextMenu*, GraphContextMenu);
    FrGetPropMacro(FrGraphBookmarkWidget*, GraphBookmarkWidget);
    
    // Methods
    void Update();
    void UpdateTimePoint();
    void UpdateLiveMode();
    
    void SetIntencityData(FrGraphDocObj* graphDO);

    // Here all signals
Q_SIGNALS:
    void TimePointChanged(int timePoint);
    void LiveModeChanged(bool value);
    void PreviousTimePoint();
    void NextTimePoint();
    void GraphChanged(int id, bool add);
    void UpdateSignal();

protected:
    virtual void timerEvent(QTimerEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

private Q_SLOTS:
    void OnGraphMarkerPositionChanged(int position);

    // Player processing
    void OnLiveModeChanged(bool);
    void OnPlayClicked();
    void OnPauseClicked();
    void OnResetClicked();
    void OnPlayFinished();
    void OnPreviousClicked();
    void OnNextClicked();

    void OnUpdate();

    // context menu 
    void itemChecked(int id, bool checked);
    void ShowRoiMean(bool checked);
    void ShowRoiSTD(bool checked);

private:
    // Video playback
    int m_TimerID;
    int m_PlayingTimePoint;
    bool m_IsPaused;
    QMutex mutex;
};

#endif
