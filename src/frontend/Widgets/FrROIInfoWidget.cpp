#include "FrROIInfoWidget.h"

#include "Qt/QApplication.h"
#include "Qt/QCheckBox.h"
#include "Qt/QVariant.h"
#include "Qt/QGroupBox.h"


FrROIInfoWidget::FrROIInfoWidget(QWidget *parent)
: QWidget(parent){
	mainVLayout = new QVBoxLayout(this);

	// general info
	QGroupBox* groupBox = new QGroupBox("General", this);
//	groupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	QLabel* label1 = new QLabel("Name: ", groupBox);
	label1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QLabel* label2 = new QLabel("Number of voxels: ", groupBox);
	label2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QVBoxLayout* vl1 = new QVBoxLayout(this);
	vl1->addWidget(label1);
	vl1->addWidget(label2);

	// name and timepoint values
	labelName = new QLabel("layer 1", groupBox);
	labelName->setFrameShape(QFrame::Box);
	labelName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	labelVoxelNumber = new QLabel("123", groupBox);
	labelVoxelNumber->setFrameShape(QFrame::Box);
	labelVoxelNumber->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QVBoxLayout* vl2 = new QVBoxLayout(this);
	vl2->addWidget(labelName);
	vl2->addWidget(labelVoxelNumber);

	QHBoxLayout* groupBoxLayout = new QHBoxLayout(groupBox);
	groupBoxLayout->addLayout(vl1);
	groupBoxLayout->addLayout(vl2);

	// voxel info
	QGroupBox* groupBox2 = new QGroupBox("ROI Info", this);
//	groupBox2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QGroupBox* gB = new QGroupBox("Position (x, y, z)", groupBox2);
	QHBoxLayout* gBLayout = new QHBoxLayout(gB);

	labelx = new QLabel("100", gB);
	labely = new QLabel("102", gB);
	labelz = new QLabel("203", gB);
	
	gBLayout->addWidget(labelx);
	gBLayout->addWidget(labely);
	gBLayout->addWidget(labelz);

	QGroupBox* gB2 = new QGroupBox("Index (i, j, k)", groupBox2);
	QHBoxLayout* gB2Layout = new QHBoxLayout(gB2);

	labeli = new QLabel("234", gB2);
	labelj = new QLabel("111", gB2);
	labelk = new QLabel("231", gB2);

	gB2Layout->addWidget(labeli);
	gB2Layout->addWidget(labelj);
	gB2Layout->addWidget(labelk);
	
	roiInfo = new QTableWidget(groupBox2);
//	roiInfo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	roiInfo->setColumnCount(2);
	roiInfo->setRowCount(0);					// number of layers

	roiInfo->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
	roiInfo->setColumnWidth(0, 50);
	roiInfo->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
	roiInfo->setColumnWidth(1, 50);
//	roiInfo->setFixedWidth(120);

	QVBoxLayout* groupBox2Layout = new QVBoxLayout(groupBox2);
	groupBox2Layout->addWidget(gB);
	groupBox2Layout->addWidget(gB2);
	groupBox2Layout->addWidget(roiInfo);

	mainVLayout->addWidget(groupBox);
	mainVLayout->addWidget(groupBox2);
	this->setLayout(mainVLayout);

	//// test
	//VoxelData vd;
	//vd.Index[0] = 100;
	//vd.Index[1] = 120;
	//vd.Index[2] = 123;
	//vd.Position[0] = 33;
	//vd.Position[1] = 232;
	//vd.Position[2] = 324;
	//vd.name = "hgf";
	//vd.timepoint = 55;
	//Voxel v;
	//v.name = "test";
	//v.value = 55;
	//vd.voxels.push_back(v);
	//SetVoxelData(vd);
}

void FrROIInfoWidget::SetROIData(ROIData rd){
	// General
	labelName->setText(rd.name);
	labelVoxelNumber->setText(QString::number(rd.nov));

	// Position
	labelx->setText(QString::number(rd.Position[0]));
	labely->setText(QString::number(rd.Position[1]));
	labelz->setText(QString::number(rd.Position[2]));
	
	// Index
	labeli->setText(QString::number(rd.Index[0]));
	labelj->setText(QString::number(rd.Index[1]));
	labelk->setText(QString::number(rd.Index[2]));
	
	// Voxels
	// clear table
	roiInfo->clear();
	int rows = rd.rois.size();
	roiInfo->setRowCount(rows);

	for (int i = 0; i<rows; i++){
		roiInfo->setItem(i, 0, new QTableWidgetItem(rd.rois[i].name));
		roiInfo->setItem(i, 1, new QTableWidgetItem(QString::number(rd.rois[i].value)));
	}
}

ROIData FrROIInfoWidget::GetROIData(){
	ROIData rd;
	
	// General
	rd.name = labelName->text();
	QString s = labelVoxelNumber->text();
	rd.nov = s.toInt();
	
	// Position, Index
	s = labelx->text();
	rd.Position[0] = s.toInt(); 
	s = labely->text();
	rd.Position[1] = s.toInt(); 
	s = labelz->text();
	rd.Position[2] = s.toInt(); 
	s = labeli->text();
	rd.Index[0] = s.toInt(); 
	s = labelj->text();
	rd.Index[1] = s.toInt(); 
	s = labelk->text();
	rd.Index[2] = s.toInt(); 

	int rows = 	roiInfo->rowCount();

	for (int i = 0; i<rows; i++){
		ROI r;
		QVariant var = roiInfo->item(i, 0)->data(0);
		r.name = var.toString();
		var = roiInfo->item(i, 1)->data(0);
		r.value = var.toInt();
		rd.rois.push_back(r);
	}

	return rd;
}



