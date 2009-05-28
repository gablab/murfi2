#ifndef FR_GRAPH_WIDGET
#define FR_GRAPH_WIDGET

// Forward declarations
class QHBoxLayout;
class QToolButton;

class FrQwtPlotWidget;
class FrPlayControlWidget;
class FrMainDocument;
class FrGraphContextMenu;
class FrGraphDocObj;
//class FrGraphBookmarkWidget;

//class FrPlayThread;
class RtMRIImage;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"
#include "FrGraphSettings.h"

#include "RtDataImage.h"

#include "Qt/qwidget.h"
#include "Qt/qframe.h"
#include "Qt/qmutex.h"


class FrGraphPaneWidget : public QFrame {
    Q_OBJECT

public:
    FrGraphPaneWidget(unsigned long id, QWidget *parent = 0, FrMainDocument* doc = 0);
    ~FrGraphPaneWidget();

    FrGetPropMacro(FrMainDocument*, Document);
    void SetDocument(FrMainDocument* doc);

    FrGetPropMacro(FrQwtPlotWidget*, QwtPlotWidget);
    FrGetPropMacro(FrGraphContextMenu*, GraphContextMenu);

    FrPropMacro(unsigned long, ID);
    FrPropMacro(unsigned long, Position);
    FrGetPropMacro(bool, Selected);
    void SetSelected(bool selected);

    // Methods
    void Update();
    void SetGraphData(FrGraphDocObj* graphDO);
    virtual void mousePressEvent(QMouseEvent* event);

    double GetIntensityValue(RtDataImage<short>* img, FrGraphSettings* gs);
    //double GetActivationValue(FrGraphSettings* gs);
    
    // Here all signals
Q_SIGNALS:
    void GraphChanged(unsigned long id, bool add);
    void UpdateSignal();
    void Clicked(unsigned long id);
    void DeleteWidget(unsigned long id);
    void GraphChanged(unsigned long gID, unsigned long tsID, bool add); // send id of widget and chosen TS
    void TimePointChanged(int timePoint);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private Q_SLOTS:
    void OnGraphMarkerPositionChanged(int position);
    void OnUpdate();
    void OnDeleteBtnClicked();

    // context menu 
    void itemChecked(unsigned long id, bool checked);
    //void ShowRoiMean(bool checked);
    //void ShowRoiSTD(bool checked);

private:
    //QMutex* mutex;  
    QToolButton* deleteBtn;

};

#endif