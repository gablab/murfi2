#ifndef FR_LAYERLISTWIDGET
#define FR_LAYERLISTWIDGET

// Forward declarations 
class FrLayerSettings;
class FrSpinSliderWidget; 
class FrColormapWidget;
class QTableWidget;
class QHBoxLayout;
class QToolButton;
class QTableWidgetItem;
class QModelIndex;

// includes
#include "Qt/qwidget.h"

// This class used to display layers
// available on image and also manage them
class FrLayerListWidget: public QWidget {
	Q_OBJECT
public:
	FrLayerListWidget(QWidget *parent = 0);

    // Common actions
    void AddLayer(FrLayerSettings* layerSets);
    void RemoveLayers();
    void SetSelectedLayer(int layerID);
    void BlockSignals(bool value){ 
        m_signalsBlocked = value;
    }

    bool GetLayerParams(int id, FrLayerSettings& params);

signals:
    void NewLayer();
    void DeleteLayer();
    void ChangeLayer();
    void LayerSelected(int id);

    // Emmited when Name, Visibility 
    // or Opacity are changed
    void ChangeLayerParams();
    // Emmited when colormap params are changed
    void ChangeLayerColormap();

private Q_SLOTS:
    // Add slots here
    void OnCellClicked(int row, int col);
    // Manage whole layer
    void OnAddClicked();
    void OnDeleteClicked();
    void OnChangeClicked();
    // Manage some params
    void OnVisibilityChanged(int id);
    void OnOpacityChanged(int value);
    void OnColormapParamsChanged();

    // Utility methods
    void UpdateCurrentLayerParams();

private:
    QTableWidget*   m_layerTable;
    QHBoxLayout*    m_layout;
    QToolButton*    m_btnAdd;
    QToolButton*    m_btnDelete;
    QToolButton*    m_btnChange;

    FrColormapWidget*   m_colormapWidget;
    FrSpinSliderWidget* m_opacityWidget;

    bool m_signalsBlocked;
};

#endif
