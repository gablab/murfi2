#include "FrVoxelInfoWidget.h"

#include "Qt/QApplication.h"
#include "Qt/QCheckBox.h"
#include "Qt/QVariant.h"
#include "Qt/QGroupBox.h"


FrVoxelInfoWidget::FrVoxelInfoWidget(QWidget *parent)
: QWidget(parent){
	mainVLayout = new QVBoxLayout(this);

	// general info
	QGroupBox* groupBox = new QGroupBox("General", this);
//	groupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	QLabel* label1 = new QLabel("Name: ", groupBox);
	label1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QLabel* label2 = new QLabel("Timepoint: ", groupBox);
	label2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QVBoxLayout* vl1 = new QVBoxLayout(this);
	vl1->addWidget(label1);
	vl1->addWidget(label2);

	// name and timepoint values
	labelName = new QLabel("layer 1", groupBox);
	labelName->setFrameShape(QFrame::Box);
	labelName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	labelTimepoint = new QLabel("123", groupBox);
	labelTimepoint->setFrameShape(QFrame::Box);
	labelTimepoint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QVBoxLayout* vl2 = new QVBoxLayout(this);
	vl2->addWidget(labelName);
	vl2->addWidget(labelTimepoint);

	QHBoxLayout* groupBoxLayout = new QHBoxLayout(groupBox);
	groupBoxLayout->addLayout(vl1);
	groupBoxLayout->addLayout(vl2);

	// voxel info
	QGroupBox* groupBox2 = new QGroupBox("Voxel Info", this);
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
	
	voxelInfo = new QTableWidget(groupBox2);
//	voxelInfo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	voxelInfo->setColumnCount(2);
	voxelInfo->setRowCount(0);					// number of layers

	voxelInfo->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
	voxelInfo->setColumnWidth(0, 50);
	voxelInfo->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
	voxelInfo->setColumnWidth(1, 50);
//	voxelInfo->setFixedWidth(120);

	QVBoxLayout* groupBox2Layout = new QVBoxLayout(groupBox2);
	groupBox2Layout->addWidget(gB);
	groupBox2Layout->addWidget(gB2);
	groupBox2Layout->addWidget(voxelInfo);

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

void FrVoxelInfoWidget::SetVoxelData(VoxelData vd){
	// General
	labelName->setText(vd.name);
	labelTimepoint->setText(QString::number(vd.timepoint));

	// Position
	labelx->setText(QString::number(vd.Position[0]));
	labely->setText(QString::number(vd.Position[1]));
	labelz->setText(QString::number(vd.Position[2]));
	
	// Index
	labeli->setText(QString::number(vd.Index[0]));
	labelj->setText(QString::number(vd.Index[1]));
	labelk->setText(QString::number(vd.Index[2]));
	
	// Voxels
	// clear table
	voxelInfo->clear();
	int rows = vd.voxels.size();
	voxelInfo->setRowCount(rows);

	for (int i = 0; i<rows; i++){
		voxelInfo->setItem(i, 0, new QTableWidgetItem(vd.voxels[i].name));
		voxelInfo->setItem(i, 1, new QTableWidgetItem(QString::number(vd.voxels[i].value)));
	}
}

VoxelData FrVoxelInfoWidget::GetVoxelData(){
	VoxelData vd;
	
	// General
	vd.name = labelName->text();
	QString s = labelTimepoint->text();
	vd.timepoint = s.toInt();
	
	// Position, Index
	s = labelx->text();
	vd.Position[0] = s.toInt(); 
	s = labely->text();
	vd.Position[1] = s.toInt(); 
	s = labelz->text();
	vd.Position[2] = s.toInt(); 
	s = labeli->text();
	vd.Index[0] = s.toInt(); 
	s = labelj->text();
	vd.Index[1] = s.toInt(); 
	s = labelk->text();
	vd.Index[2] = s.toInt(); 

	int rows = 	voxelInfo->rowCount();

	for (int i = 0; i<rows; i++){
		Voxel v;
		QVariant var = voxelInfo->item(i, 0)->data(0);
		v.name = var.toString();
		var = voxelInfo->item(i, 1)->data(0);
		v.value = var.toInt();
		vd.voxels.push_back(v);
	}

	return vd;
}

void FrVoxelInfoWidget::Clear(){
	// General
//	labelName->setText(vd.name);
//	labelTimepoint->setText(QString::number(vd.timepoint));

	// Position
	labelx->setText("");
	labely->setText("");
	labelz->setText("");
	
	// Index
	labeli->setText("");
	labelj->setText("");
	labelk->setText("");

	voxelInfo->clear();
}


