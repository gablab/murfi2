#ifndef FR_LAYERLISTWIDGET
#define FR_LAYERLISTWIDGET

// Forward declarations 
class FrLayerSettings;
class QTableWidget;
class QVBoxLayout;
class QToolButton;

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
    void BlockSignals(bool value){ 
        m_signalsBlocked = value;
    }

signals:
    void NewLayer();
    void DeleteLayer();
    void ChangeLayer();
    void LayerSelected(int id);

private Q_SLOTS:
    // Add slots here
    void OnSelectionChanged();
    void OnAddClicked();
    void OnDeleteClicked();
    void OnChangeClicked();    
	
private:
	QTableWidget*   m_layerTable;
	QVBoxLayout*    m_layout;
    QToolButton*    m_btnAdd;
    QToolButton*    m_btnDelete;
    QToolButton*    m_btnChange;
    bool m_signalsBlocked;
};

#endif