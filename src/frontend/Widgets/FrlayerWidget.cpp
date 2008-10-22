#include "FrLayerWidget.h"
#include "FrTabSettingsDocObj.h"

#include "Qt/QTableWidget.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QCheckBox.h"
#include "Qt/QToolButton.h"
#include "Qt/QVariant.h"
#include "Qt/QLabel.h"
#include "Qt/QPicture.h"
#include <QtGui/QSpacerItem>
#include "Qt/QPainter.h"
#include "Qt/QPaintDevice.h"


FrLayerWidget::FrLayerWidget(int cmtype, QString name, bool visible, QWidget *parent)
: QWidget(parent) {
	picLabel = new QLabel("", this);
	switch(cmtype){
		case 0:
			picLabel->setPixmap(QPixmap("icons\\background.png"));
			break;
		case 1:
			picLabel->setPixmap(QPixmap("icons\\multi.png"));
			break;
		case 2:
			picLabel->setPixmap(QPixmap("icons\\single.png"));
			break;
	}

	layerNameLabel = new QLabel(name, this);

	QSpacerItem *spacerItem;
	spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	visibilityCheckBox = new QCheckBox(this);
	visibilityCheckBox->setChecked(visible);
	
	m_layout = new QHBoxLayout(this);
	m_layout->addWidget(picLabel);
	m_layout->addWidget(layerNameLabel);
	m_layout->addItem(spacerItem);
	m_layout->addWidget(visibilityCheckBox);

	this->setLayout(m_layout);

	// some signals
	connect(this->visibilityCheckBox, SIGNAL(stateChanged(int)), parent, SLOT(OnVisibilityChanged(int)));
}

void FrLayerWidget::SetName(QString name){
	layerNameLabel->text() = name;
}

void FrLayerWidget::SetVisibility(bool visibile){
	if (visibile)
		visibilityCheckBox->setChecked(true);
	else
		visibilityCheckBox->setChecked(true);
}

void FrLayerWidget::SetColormapType(int cmtype){
	switch(cmtype){
		case 0:
			picLabel->setPixmap(QPixmap("icons\\background.png"));
			break;
		case 1:
			picLabel->setPixmap(QPixmap("icons\\multi.png"));
			break;
		case 2:
			picLabel->setPixmap(QPixmap("icons\\single.png"));
			break;
	}
}