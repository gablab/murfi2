#ifndef FR_ROIINFOWIDGET
#define FR_ROIINFOWIDGET

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

typedef struct _roi {
	QString name;
	int value;
} ROI;

typedef struct _roiData {
    QString name;
	int nov;			// number of voxels
	int Position[3];
	int Index[3];
	vector<ROI> rois;	
} ROIData;


class FrROIInfoWidget: public QWidget
{
	Q_OBJECT
public:
	FrROIInfoWidget(QWidget *parent = 0);

	void SetROIData(ROIData vd);
	ROIData GetROIData();

public slots:
    // Add slots here

signals:
	
private:
	QTableWidget*	roiInfo;
	QVBoxLayout*    mainVLayout;
	
	// labels
	QLabel*			labelName;
	QLabel*			labelVoxelNumber;
	QLabel*			labelx;
	QLabel*			labely;
	QLabel*			labelz;
	QLabel*			labeli;
	QLabel*			labelj;
	QLabel*			labelk;

};

#endif