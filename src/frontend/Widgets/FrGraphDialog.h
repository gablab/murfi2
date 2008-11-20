// class to get bookmark info
#ifndef FR_GRAPH_DLG
#define FR_GRAPH_DLG

class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;


#include "Qt/qstring.h"
#include "QtGui/qdialog.h"

class FrGraphDialog: public QDialog {
	Q_OBJECT
public:
    FrGraphDialog(QWidget* parent = 0L, bool isModal = true);

    void SetCaption(QString& caption);
   
    bool SimpleExec();
    
private:
    QVBoxLayout* m_vLayout;

    // Buttons
    QHBoxLayout* m_btnLayout;    
    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
};

#endif
