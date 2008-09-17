// class to get bookmark info

#ifndef FR_TABINFO_DLG
#define FR_TABINFO_DLG

class QLabel;
class QLineEdit;
class QPushButton;
class QVboxLayout;
class QHBoxLayout;


#include "Qt/QString.h"
#include "QtGui/QDialog.h"

class FrTabInfoDialog: public QDialog
{
	Q_OBJECT
public:
    FrTabInfoDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);
    void SetInfo(QString& value);

    QString GetName();
    void SetName(QString& value);

    QString GetDescription();
    void SetDescription(QString& value);

    bool SimpleExec();
    
private:
    QVboxLayout* m_vLayout;
    QHBoxLayout* m_hLayout;

    QLabel* m_lblInfo;
    QLineEdit* m_txtName;
    QLineEdit* m_txtDescription;

    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
    
};

#endif