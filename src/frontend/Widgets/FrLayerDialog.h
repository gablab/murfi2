#ifndef FR_LAYER_DLG
#define FR_LAYER_DLG

class QLabel;
class QLineEdit;
class QPushButton;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
class QComboBox;
class QCheckBox;
class QSlider;

#include "FrTabSettingsDocObj.h"

#include "Qt/QString.h"
#include "QtGui/QDialog.h"


class FrLayerDialog: public QDialog
{
	Q_OBJECT
public:
    FrLayerDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);
	void GetLayerParams(FrLayerSettings& layerSets);
    void SetLayerParams(FrLayerSettings& layerSets);

    bool SimpleExec();
 
public slots:
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
    void OnOKClicked();

private:
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_btnLayout;

    QLabel* m_lblName;
    QLineEdit* m_txtName;
    QLabel* m_lblDescription;
    QLineEdit* m_txtDescription;
	QLineEdit *lineEdit;	// name

    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;

	// new
    QColor color;
	QWidget* colorWidget;
	QComboBox* comboBox;

	// spin boxes
	QSpinBox *OpacitySpinBox;
	QSpinBox *PxMinSpinBox;
	QSpinBox *PxMaxSpinBox;
	QSpinBox *ThresholdSpinBox;

	QCheckBox *visibleCheckBox;

	// sliders
    QSlider *OpacitySlider;
    QSlider *PxMinSlider;
    QSlider *PxMaxSlider;
    QSlider *ThresholdSlider;

	QGroupBox* groupBox;
	QGroupBox* groupBox2;

    enum FrColormapSettings::Type cmType;
};

#endif