// class for graphs panel

#ifndef FR_GRAPH_WIDGET
#define FR_GRAPH_WIDGET

// Forward declarations
class QCheckBox;
class QLabel;
class QPaintEvent;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"

#include "Qt/qstring.h"
#include "Qt/qcolor.h"
#include "Qt/qwidget.h"

class FrGraphWidget : public QWidget {
	Q_OBJECT
public:
	FrGraphWidget(unsigned long id, QWidget *parent = 0);

    // Properties
    FrGetPropMacro(unsigned long, ID);
    FrGetPropMacro(QColor, Color);
    void SetColor(QColor& value);

    QString GetName();
    void SetName(QString& name);

    bool GetVisibility();
    void SetVisibility(bool value);
    
    virtual void paintEvent(QPaintEvent* e);

    // Here all signals
Q_SIGNALS:
    void VisibilityChanged(unsigned long id);    

private Q_SLOTS:
    void OnVisibilityChanged();

private:
    QCheckBox*  m_chkVisibility;
    QLabel*     m_lblName;
};

#endif
