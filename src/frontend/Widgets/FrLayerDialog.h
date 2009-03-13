#ifndef FR_LAYER_DLG
#define FR_LAYER_DLG

class FrImageLayerSettings;
class FrSpinSliderWidget;
class FrColormapWidget;

class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;


#include "Qt/qstring.h"
#include "QtGui/qdialog.h"


class FrLayerDialog: public QDialog {
	Q_OBJECT
public:
    FrLayerDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);
	void GetLayerParams(FrImageLayerSettings& layerSets);
    void SetLayerParams(FrImageLayerSettings& layerSets);

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
