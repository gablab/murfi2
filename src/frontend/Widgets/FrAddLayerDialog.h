// class to get bookmark info

#ifndef FR_ADD_LAYER_DLG
#define FR_ADD_LAYER_DLG

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


class FrAddLayerDialog: public QDialog
{
	Q_OBJECT
public:
    FrAddLayerDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);

//    QString GetName();
//    void SetName(QString& value);

//    QString GetDescription();
//    void SetDescription(QString& value);
	LayerSettings GetLayerParams();

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

private:
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_btnLayout;

    QLabel* m_lblName;
    QLineEdit* m_txtName;
    QLabel* m_lblDescription;
    QLineEdit* m_txtDescription;

    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;

	// new
	QWidget* colorWidget;
	QColor color;
	QComboBox* comboBox;

	// spin boxes
	QSpinBox *OpacitySpinBox;
	QSpinBox *PxMinSpinBox;
	QSpinBox *PxMaxSpinBox;
	QSpinBox *ThresholdSpinBox;

	QCheckBox *checkBox;

	// sliders
    QSlider *OpacitySlider;
    QSlider *PxMinSlider;
    QSlider *PxMaxSlider;
    QSlider *ThresholdSlider;

	int cmType;

	QGroupBox* groupBox;
	QGroupBox* groupBox2;
    
};

#endif