#ifndef FR_TREEWIDGET_ITEM
#define FR_TREEWIDGET_ITEM


#include "FrMacro.h"
#include "Qt/qtreewidget.h"
#include "RtDataID.h"


class FrTreeWidgetItem : public QTreeWidgetItem {
public:
    FrTreeWidgetItem(const QStringList & strings, int type = Type);

    FrPropMacro(RtDataID, ID);

};

#endif