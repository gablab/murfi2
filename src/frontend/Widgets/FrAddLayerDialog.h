// class to get bookmark info

#ifndef FR_ADD_LAYER_DLG
#define FR_ADD_LAYER_DLG

class QLabel;
class QLineEdit;
class QPushButton;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;


#include "Qt/QString.h"
#include "QtGui/QDialog.h"

class FrAddLayerDialog: public QDialog
{
	Q_OBJECT
public:
    FrAddLayerDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);

    QString GetName();
    void SetName(QString& value);

    QString GetDescription();
    void SetDescription(QString& value);

    bool SimpleExec();
 
public slots:
	void onComboBoxChange(int index);
	void onColorBtnClicked();

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

	QGroupBox* groupBox;
	QGroupBox* groupBox2;
    
};

#endif