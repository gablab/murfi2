#include "FrLayerListWidget.h"
//#include "FrAddLayerDialog.h"

#include "Qt/QApplication.h"
#include "Qt/QCheckBox.h"
#include "Qt/QVariant.h"

FrLayerListWidget::FrLayerListWidget(QWidget *parent)
: QWidget(parent){
	m_layerTable = new QTableWidget(this);
	// add column headers
	m_layerTable->setColumnCount(6);
	m_layerTable->setRowCount(0);
	m_layerTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	m_layerTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	m_layerTable->setColumnWidth(0, 50);
	m_layerTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Visible"));
	m_layerTable->setColumnWidth(1, 50);
	m_layerTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Name"));
	m_layerTable->setColumnWidth(2, 100);
	m_layerTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Colormap"));
	m_layerTable->setColumnWidth(3, 70);
	m_layerTable->setHorizontalHeaderItem(4, new QTableWidgetItem("Opacity"));
	m_layerTable->setColumnWidth(4, 70);
	m_layerTable->setHorizontalHeaderItem(5, new QTableWidgetItem("Threshold"));
	m_layerTable->setColumnWidth(5, 70);

	//m_layerTable->setItem(1, 1, new QTableWidgetItem("Test"));

	m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(1);
    m_layout->addWidget(m_layerTable);
    this->setLayout(m_layout);

	connect(m_layerTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onLayerEdit(int, int)));

	//// test
	//LayerSettings ls;
	//ls.color = QColor(0, 255, 255, 255);
	//ls.ColormapType = 1;
	//ls.id = 1;
	//ls.name = "layer 1";
	//ls.Opacity = 100;
	//ls.Threshold = 128;
	//ls.Visible = true;

	//AddLayer(ls);
	//ls.id = 2;
	//AddLayer(ls);
}

//void FrLayerListWidget::AddLayer(LayerSettings ls){
//	int rowCount = m_layerTable->rowCount();
//	m_layerTable->setRowCount(rowCount+1);
//
//	// add new row
//	WriteLayerSettings(rowCount, ls);
//
//	// connect cell widgets signals here?
//}

//void FrLayerListWidget::WriteLayerSettings(int rownumber, LayerSettings ls){
//	QString s;
//	m_layerTable->setItem(rownumber, 0, new QTableWidgetItem(s.setNum(ls.id)));
//
//	s = "false";
//	if (ls.Visible)
//		s = "true";
//
//	m_layerTable->setItem(rownumber, 1, new QTableWidgetItem(s));
//	m_layerTable->setItem(rownumber, 2, new QTableWidgetItem(QString(ls.name)));
//	
//	s = "none";
//	if (ls.ColormapType == 1)
//		s = "multi";
//	else if (ls.ColormapType == 2)
//		s = "single";
//	m_layerTable->setItem(rownumber, 3, new QTableWidgetItem(s));
//	m_layerTable->setItem(rownumber, 4, new QTableWidgetItem(s.setNum(ls.Opacity)));
//	m_layerTable->setItem(rownumber, 5, new QTableWidgetItem(s.setNum(ls.Threshold)));
//}

void FrLayerListWidget::RemoveLayer(){
	// TODO: implement..
}

void FrLayerListWidget::onLayerEdit(int row, int column){
	//// call (add/edit) dialog with selected layers params
	//FrAddLayerDialog dlg(this, true);
	//LayerSettings layerParams;
	//// get params from selected row / tabsettings?
	//QVariant var = m_layerTable->item(row, 0)->data(0);
	//layerParams.id = var.toInt();
	//var = m_layerTable->item(row, 2)->data(0);
	////QString s = QString(var.toString());
	////wchar_t *tmp;
	////s.toWCharArray(tmp);
	//
	////layerParams.name = 
	//dlg.SetLayerParams(layerParams);

	//if(!dlg.SimpleExec())
	//	return; 
	//
 //   //FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
	//layerParams = dlg.GetLayerParams();
	//// write new params to tabsettings obj
	//// renew views
}