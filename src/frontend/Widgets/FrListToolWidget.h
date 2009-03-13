#ifndef FR_LISTTOOL_WIDGET
#define FR_LISTTOOL_WIDGET

#include "FrBaseToolWidget.h"

#include "Qt/qwidget.h"
#include "Qt/qstring.h"

class QLabel;
class QListWidget;
class QListWidgetItem;

class FrListToolWidget: public FrBaseToolWidget{
    Q_OBJECT

public:
    FrListToolWidget(QWidget* parent=0);

    void SetName(QString name);
    void ClearAll();
    void AddListItem(unsigned long ID, QString& name);

    unsigned long GetCurrentItemID();
    
signals:
    virtual void ParamsChanged();

private Q_SLOTS:
    void ROIListItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
    QLabel* label;
    QListWidget* roiList;

};

#endif
