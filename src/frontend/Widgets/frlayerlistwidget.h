// class for displaying bookmarks

#ifndef FR_LAYERLISTWIDGET
#define FR_LAYERLISTWIDGET

//class QVBoxLayout;
//class QTableWidget;

//class FrTabSettingsDocObj;
#include "FrTabSettingsDocObj.h"


#include "Qt/QWidget.h"
#include "Qt/QTableWidget.h"
#include "Qt/QBoxLayout.h"
#include <vector>
using namespace std;


class FrLayerListWidget: public QWidget
{
	Q_OBJECT
public:
	FrLayerListWidget(QWidget *parent = 0);

//    void AddLayer(LayerSettings ls);
    void RemoveLayer();
//	void WriteLayerSettings(int rownumber, LayerSettings ls);

public slots:
    // Add slots here
	void onLayerEdit(int row, int column);

signals:
	
private:
	QTableWidget*		m_layerTable;
	QVBoxLayout*        m_layout;
	
};

#endif