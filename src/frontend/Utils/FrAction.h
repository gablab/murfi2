#ifndef FR_ACTION
#define FR_ACTION

#include "FrMacro.h"
#include "Qt/qaction.h"

class FrAction : public QAction {
    Q_OBJECT
public:
    FrAction(const QString &text, QObject* parent);

    // properties
    FrSetPropMacro(int, ID);

private Q_SLOTS:
    void OnPress(bool checked);

signals:
    void actionChecked(int id, bool checked);

};

#endif
