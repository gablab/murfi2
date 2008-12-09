#ifndef FR_LAYERLISTWIDGET
#define FR_LAYERLISTWIDGET

// Forward declarations 
class FrLayerDocObj;
class FrLayerSettings;
class FrRoiDocObj;
class FrSpinSliderWidget; 
class FrColormapWidget;
class FrROIToolWidget;
class FrMainDocument;

class QTableWidget;
class QHBoxLayout;
class QToolButton;
class QTableWidgetItem;
class QModelIndex;

#include "FrMacro.h"

// includes
#include "Qt/qwidget.h"

// This class used to display layers
// available on image and also manage them
class FrLayerListWidget: public QWidget {
	Q_OBJECT
public:
	FrLayerListWidget(QWidget *parent = 0);

    // Common actions
    void AddLayer(FrLayerDocObj* layerDO);
    void RemoveLayers();
    void UpdateRoiList();

    void SetSelectedLayer(int layerID);
    void BlockSignals(bool value){ 
        m_signalsBlocked = value;
    }

    FrLayerDocObj* GetLayerDocObjByID(int id);
    //bool GetLayerParams(int id, FrLayerSettings& params);
    
    int GetOpacity();
    bool GetLayerVisibility(int id);

    FrColormapWidget* GetColormapWidget(){
        return m_colormapWidget;
    }

    FrROIToolWidget* GetRoiToolWidget(){
        return m_roiToolWidget;
    }

    FrSetPropMacro(FrMainDocument*, MainDoc);

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
    //Emited when current ROI tool is changed
    void RoiToolChanged();
    
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
    void OnRoiToolChanged();

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
    FrROIToolWidget* m_roiToolWidget;
    FrMainDocument* m_mainDoc;

    bool m_signalsBlocked;
};

#endif
