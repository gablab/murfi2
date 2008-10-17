#ifndef FR_VOXELINFOWIDGET
#define FR_VOXELINFOWIDGET

//class QVBoxLayout;
//class QTableWidget;

//class FrTabSettingsDocObj;
#include "FrTabSettingsDocObj.h"


#include "Qt/QWidget.h"
#include "Qt/QTableWidget.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QLabel.h"
#include <vector>
using namespace std;

typedef struct _voxel {
	QString name;
	int value;
} Voxel;

typedef struct _voxelData {
    QString name;
	int timepoint;	// what type it should be?
	int Position[3];
	int Index[3];
	vector<Voxel> voxels;	
} VoxelData;


class FrVoxelInfoWidget: public QWidget
{
	Q_OBJECT
public:
	FrVoxelInfoWidget(QWidget *parent = 0);

	void SetVoxelData(VoxelData vd);
	VoxelData GetVoxelData();

public slots:
    // Add slots here

signals:
	
private:
	QTableWidget*	voxelInfo;
	QVBoxLayout*    mainVLayout;
	
	// labels
	QLabel*			labelName;
	QLabel*			labelTimepoint;
	QLabel*			labelx;
	QLabel*			labely;
	QLabel*			labelz;
	QLabel*			labeli;
	QLabel*			labelj;
	QLabel*			labelk;

};

#endif