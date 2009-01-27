#include "FrAction.h"

FrAction::FrAction(const QString &text, QObject* parent)
: QAction(text, parent) {
    this->setCheckable(true);

    connect(this, SIGNAL(toggled(bool)), this, SLOT(OnPress(bool)));

}

void FrAction::OnPress(bool checked){
    emit actionChecked(m_ID, checked);
}
