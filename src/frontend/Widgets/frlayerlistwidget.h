#ifndef FR_LAYERLISTWIDGET
#define FR_LAYERLISTWIDGET

// Forward declarations 
class FrLayerSettings;
class QTableWidget;
class QVBoxLayout;

// includes
#include "Qt/QWidget.h"


// This class used to display layers
// available on image and also manage them
class FrLayerListWidget: public QWidget
{
	Q_OBJECT
public:
	FrLayerListWidget(QWidget *parent = 0);

    void RemoveLayers();
    void AddLayer(FrLayerSettings* layerSets);
    
    void SetSelectedLayer(int layerID);

signals:
    void LayerEdit(int id);
    void LayerSelected(int id); 
    

private Q_SLOTS:
    // Add slots here
	void OnLayerEdit(int row, int column);
    void OnLayerSelect(int row, int column);

    int GetLayerID(int row, int column);
	
private:
	QTableWidget*		m_layerTable;
	QVBoxLayout*        m_layout;
};

#endif