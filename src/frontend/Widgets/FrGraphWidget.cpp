#include "FrGraphWidget.h"

// Qt stuff
#include "Qt/qcheckbox.h"
#include "Qt/qlabel.h"
#include "Qt/qlayout.h"
#include "Qt/qpainter.h"
#include "Qt/qstyle.h"


FrGraphWidget::FrGraphWidget(int id, QWidget* parent) 
: QWidget(parent){
    // init values
    m_ID = id;
    m_Color = QColor(Qt::black);

    m_chkVisibility = new QCheckBox(this);
    m_chkVisibility->setTristate(false);

    m_lblName = new QLabel(QString("Graph"), this);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_chkVisibility);
    layout->addWidget(m_lblName);

    // Connect signals
    connect(m_chkVisibility, SIGNAL(stateChanged(int)),
            this, SLOT(OnVisibilityChanged()));

    this->setFixedHeight(this->sizeHint().height());
    this->setBackgroundRole(QPalette::Light);
}

void FrGraphWidget::SetColor(QColor& color){
    if(color.isValid() && color != m_Color){
        m_Color = color;
        this->repaint();
    }
}

QString FrGraphWidget::GetName(){ 
    return m_lblName->text(); 
}

void FrGraphWidget::SetName(QString& name){
    m_lblName->setText(name); 
}

bool FrGraphWidget::GetVisibility(){  
    return (m_chkVisibility->checkState() == 
            Qt::Checked);
}

void FrGraphWidget::SetVisibility(bool value){ 
    Qt::CheckState state = value ? Qt::Checked : Qt::Unchecked;
    m_chkVisibility->setCheckState(state); 
}

void FrGraphWidget::OnVisibilityChanged(){
    emit VisibilityChanged(m_ID);
}

void FrGraphWidget::paintEvent(QPaintEvent* e){
    // Draw separated line
    QPainter painter( this );
    painter.setPen(QColor(Qt::black));

    int line_y = this->height()-1;
    painter.drawLine(0, line_y, this->width(), line_y);
    
    	
	// Draw colored border around checkbox
    QPen pen(Qt::SolidLine);
    pen.setColor(m_Color);
    pen.setWidth(3);
    
    painter.setPen(pen);
    painter.drawRect(m_chkVisibility->geometry());
    painter.fillRect(m_chkVisibility->geometry(), QBrush(m_Color));
}