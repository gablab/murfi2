// class to get bookmark info

#ifndef FR_TABINFO_DLG
#define FR_TABINFO_DLG

class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;


#include "Qt/QString.h"
#include "QtGui/QDialog.h"

class FrTabInfoDialog: public QDialog
{
	Q_OBJECT
public:
    FrTabInfoDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);

    QString GetName();
    void SetName(QString& value);

    QString GetDescription();
    void SetDescription(QString& value);

    bool SimpleExec();
    
private:
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_btnLayout;

    QLabel* m_lblName;
    QLineEdit* m_txtName;
    QLabel* m_lblDescription;
    QLineEdit* m_txtDescription;

    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
    
};

#endif