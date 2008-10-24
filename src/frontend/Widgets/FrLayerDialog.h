#ifndef FR_LAYER_DLG
#define FR_LAYER_DLG

class FrLayerSettings;
class FrSpinSliderWidget;
class FrColormapWidget;

class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;


#include "Qt/QString.h"
#include "QtGui/QDialog.h"


class FrLayerDialog: public QDialog {
	Q_OBJECT
public:
    FrLayerDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);
	void GetLayerParams(FrLayerSettings& layerSets);
    void SetLayerParams(FrLayerSettings& layerSets);

    bool SimpleExec();
 
public slots:
	void OnBtnOKClicked();

private:
    QHBoxLayout* CreateTitleLayout();
    QHBoxLayout* CreateButtonLayout();
    QHBoxLayout* CreateGeneralLayout();

private:
    QLineEdit* m_txtName;
    FrSpinSliderWidget* m_opacityWidget; 
    QCheckBox *m_chkVisibility;
    FrColormapWidget* m_colormapWidget;

    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
};

#endif