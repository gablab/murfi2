#ifndef FR_LAYERLISTWIDGET
#define FR_LAYERLISTWIDGET

// Forward declarations 
class FrLayerDocObj;
class FrLayerSettings;
class FrRoiDocObj;
class FrSpinSliderWidget; 
class FrSliderWidget; 
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
    FrLayerListWidget(QWidget *parent = 0, FrMainDocument* doc = 0);

    // Common actions
    void Update();
    void AddLayer(FrLayerDocObj* layerDO, int pos);
    void RemoveLayers();
    void UpdateRoiList();

    void SetSelectedLayer(unsigned long layerID);
    void BlockSignals(bool value){ 
        m_signalsBlocked = value;
    }

    //FrLayerDocObj* GetLayerDocObjByID(int id);
    //bool GetLayerParams(int id, FrLayerSettings& params);
    
    int GetOpacity();
    bool GetLayerVisibility(unsigned long id);

    FrColormapWidget* GetColormapWidget(){
        return m_colormapWidget;
    }

    FrROIToolWidget* GetRoiToolWidget(){
        return m_roiToolWidget;
    }

    FrSetPropMacro(FrMainDocument*, Document);

    void OnUpdate();

signals:
    void NewLayer();
    void DeleteLayer();
    void ChangeLayer();
    void LayerSelected(unsigned long id);
    void ChangeLayerPosition(int inc);

    // Emmited when Name, Visibility 
    // or Opacity are changed
    void ChangeLayerParams();
    // Emmited when colormap params are changed
    void ChangeLayerColormap();
    //Emited when current ROI tool is changed
    void RoiToolChanged();
    void ClearCurrentRoi();
    
private Q_SLOTS:
    // Add slots here
    void OnCellClicked(int row, int col);
    // Manage whole layer
    void OnAddClicked();
    void OnDeleteClicked();
    void OnChangeClicked();
    void OnUpArrowClicked();
    void OnDownArrowClicked();

    // Manage some params
    void OnVisibilityChanged(unsigned long id);
    void OnOpacityChanged(int value);
    void OnColormapParamsChanged();
    void OnRoiToolChanged();
    void OnClearCurrentRoiAction();

    // Utility methods
    void UpdateCurrentLayerParams();

private:
    QTableWidget*   m_layerTable;
    QHBoxLayout*    m_layout;
    QToolButton*    m_btnAdd;
    QToolButton*    m_btnDelete;
    QToolButton*    m_btnChange;
    QToolButton*    m_btnUpArrow;
    QToolButton*    m_btnDownArrow;

    FrColormapWidget*   m_colormapWidget;
    FrSliderWidget* m_opacityWidget;
    FrROIToolWidget* m_roiToolWidget;

    bool m_signalsBlocked;

};

#endif
