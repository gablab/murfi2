#include "FrGraphListWidget.h"
#include "FrGraphWidget.h"

// Qt stuff
#include "Qt/qlist.h"
#include "Qt/qtoolbox.h"
#include "Qt/qlayout.h"
#include "Qt/qpalette.h"
#include "Qt/qpushbutton.h"


FrGraphListWidget::FrGraphListWidget(QWidget* parent) 
: QScrollArea(parent){
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setWidgetResizable(true);

    // create central widget
    m_centralWidget = new QWidget();
    m_centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidget(m_centralWidget);
    
    // create layout
    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->setSpacing(0);
    m_layout->setAlignment(Qt::AlignTop);

#if QT_VERSION >= 0x040300
    m_layout->setContentsMargins(0, 0, 0, 0);
#else
    m_layout->setMargin(0);
#endif

    // NOTE: for testing 
    QColor colors[] = { QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255) };
    for(int i=0; i < 3; ++i){
        this->AddGraphWidget(i, QString("Graph %1").arg(i), colors[i]);
    }
}

void FrGraphListWidget::AddGraphWidget(FrGraphWidget* widget){
    widget->setParent(m_centralWidget);
    m_layout->addWidget(widget);
    connect(widget, SIGNAL(VisibilityChanged(int)), 
            this, SLOT(OnGraphVisibilityChanged(int)));
}

void FrGraphListWidget::AddGraphWidget(int id, QString& name, QColor& color, bool isVisible){
    FrGraphWidget* gw = new FrGraphWidget(id);
    gw->SetName(name);
    gw->SetColor(color);
    gw->SetVisibility(isVisible);
    this->AddGraphWidget(gw);
}

bool FrGraphListWidget::RemoveGraphWidget(int id, bool doDelete){
    bool result = false;
    FrGraphWidget* wgt = this->GetGraphWidget(id);
    if(wgt){
        result = this->RemoveGraphWidget(wgt, doDelete);
    }
    return result;
}

bool FrGraphListWidget::RemoveGraphWidget(FrGraphWidget* widget, bool doDelete){
    // result of operation
    bool result = false;

    if(widget->parent() == m_centralWidget){
        m_layout->removeWidget(widget);
        m_centralWidget->setParent(0);
        disconnect(widget, SIGNAL(VisibilityChanged(int)), 
                   this, SLOT(OnGraphVisibilityChanged(int)));

        if(doDelete){
            delete widget;
        }
        result = true;
    }

    return result;
}

FrGraphWidget* FrGraphListWidget::GetGraphWidget(int id){
    // Find widget by id
    FrGraphWidget* result = 0L;
    QObjectList::const_iterator it, itEnd = m_layout->children().end();
    for(it = m_layout->children().begin(); it != itEnd; ++it){
        FrGraphWidget* wgt = qobject_cast<FrGraphWidget*>((*it));

        if(wgt && wgt->GetID() == id){
            result = wgt;
            break;
        }
    }

    return result;
}

void FrGraphListWidget::OnGraphVisibilityChanged(int id){
    emit GraphVisibilityChanged(id);
}