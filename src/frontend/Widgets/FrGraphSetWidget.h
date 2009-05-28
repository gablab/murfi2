// class for graphs panel

#ifndef FR_GRAPHPANE_WIDGET
#define FR_GRAPHPANE_WIDGET

// Forward declarations
class FrQwtPlotWidget;
class FrGraphListWidget;
class FrPlayControlWidget;
class FrMainDocument;
class FrGraphContextMenu;
class FrGraphDocObj;
class FrGraphBookmarkWidget;
class FrGraphPaneWidget;

class FrPlayThread;

class QVBoxLayout;
class QHBoxLayout;
class QToolButton;
class QScrollArea;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"
#include "FrGraphSettings.h"

#include "Qt/qwidget.h"
#include "Qt/qmutex.h"

class FrGraphSetWidget : public QWidget {
	Q_OBJECT

public:
	FrGraphSetWidget(QWidget *parent = 0, FrMainDocument* doc = 0);
    ~FrGraphSetWidget();

    // Properties
    FrGetPropMacro(FrMainDocument*, Document);
    void SetDocument(FrMainDocument* doc);
    FrGetPropMacro(FrPlayControlWidget*, PlayControlWidget);

    // Methods
    void Update();
    void AddGraphWidget(unsigned long id);
    void RemoveGraphWidget(unsigned long id);
    void ChangeGraphPosition(int inc);
    FrGraphPaneWidget* GetGraphById(unsigned long id);
    FrGraphPaneWidget* GetGraphByPos(unsigned long pos);

    // play control methods
    //void UpdateTimePoint();
    void UpdateLiveMode();

    // signals
Q_SIGNALS:
    void UpdateSignal();
    void NewGraphWidget();
    void DeleteGraphWidget(unsigned long gID);
    void GraphChanged(unsigned long gID, unsigned long tsID, bool add);

    // play control signals
    void TimePointChanged(int timePoint);
    void LiveModeChanged(bool value);
    void PreviousTimePoint();
    void NextTimePoint();

    // slots
private Q_SLOTS:
    void OnUpdate();
    void OnAddGraphWidgetBtnClicked();
    void OnWidgetSelect(unsigned long id);
    void OnWidgetDelete(unsigned long id);
    void OnGraphChanged(unsigned long gID, unsigned long tsID, bool add);
    void OnTimePointChanged(int timePoint);

    void OnUpArrowClicked();
    void OnDownArrowClicked();

    // Player processing
    void OnLiveModeChanged(bool);
    void OnPlayClicked();
    void OnPauseClicked();
    void OnResetClicked();
    void OnPlayFinished();
    void OnPreviousClicked();
    void OnNextClicked();

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    int GetMaxTimePoint();

private:
    typedef std::vector<FrGraphPaneWidget*> GraphsCollection;
    GraphsCollection m_Graphs;

    QToolButton*    m_btnUpArrow;
    QToolButton*    m_btnDownArrow;

    QVBoxLayout*    graphsLayout;
    QScrollArea*    scrollArea;

private:
    // Video playback
    unsigned long m_TimerID;
    int m_PlayingTimePoint;
    bool m_IsPaused;

};

#endif
