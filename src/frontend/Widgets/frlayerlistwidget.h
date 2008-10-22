#ifndef FR_LAYERLISTWIDGET
#define FR_LAYERLISTWIDGET

// Forward declarations 
class FrLayerSettings;
class QTableWidget;
class QHBoxLayout;
class QToolButton;
class QComboBox;
class QWidget;
class QSpinBox;
class QColor;
class QSlider;
class QGroupBox;

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
    void SetLayerParams(FrLayerSettings* layerSets);

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
	void OnVisibilityChanged(int state);
	void OnNameChanged(int row, int column);

	void onComboBoxChange(int index);
	void onColorBtnClicked();
	void SetOpacitySliderPosition(int value);
	void SetOpacitySpinBoxPosition(int value);
	void SetPxMinSliderPosition(int value);
	void SetPxMinSpinBoxPosition(int value);
	void SetPxMaxSliderPosition(int value);
	void SetPxMaxSpinBoxPosition(int value);
	void SetThresholdSliderPosition(int value);
	void SetThresholdSpinBoxPosition(int value);
	
private:
	QTableWidget*   m_layerTable;
	QHBoxLayout*    m_layout;
    QToolButton*    m_btnAdd;
    QToolButton*    m_btnDelete;
    QToolButton*    m_btnChange;

	// new
    QColor color;
	QWidget* colorWidget;
	QComboBox* comboBox;

	// spin boxes
	QSpinBox *OpacitySpinBox;
	QSpinBox *PxMinSpinBox;
	QSpinBox *PxMaxSpinBox;
	QSpinBox *ThresholdSpinBox;

	// sliders
    QSlider *OpacitySlider;
    QSlider *PxMinSlider;
    QSlider *PxMaxSlider;
    QSlider *ThresholdSlider;

	QGroupBox* groupBox;
	QGroupBox* groupBox2;

    bool m_signalsBlocked;
};

#endif