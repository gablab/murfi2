#ifndef FR_CREATEROI_DLG
#define FR_CREATEROI_DLG

// Forward declarations 
class QLineEdit;
class QHBoxLayout;
class QGroupBox;
class QComboBox;
class FrBrowseFileWidget;
class FrEmptyToolWidget;
class FrSpinToolWidget;
class QStackedLayout;

// includes
#include "Qt/qstring.h"
#include "QtGui/qdialog.h"


// This class aloow user to choose 
// how to create ROI
class FrCreateRoiDialog: public QDialog {
	Q_OBJECT
public:
    enum Action { None=0, CreateEmpty, ThresholdToMask, LoadFromFile };
public:
    FrCreateRoiDialog(QWidget* parent = 0L, bool isModal = true);

    // Accessors / modifiers
    void SetCaption(QString& caption);
    Action GetAction();
    double GetThreshold();
    QString GetFileName();

    // Exec method
    bool SimpleExec();
    
private:
    QHBoxLayout* CreateButtonLayout();	
    QHBoxLayout* CreateSelectionLayout();
    void InitializeParams(QGroupBox* gb);

private Q_SLOTS:
    void OnActionIndexChanged(int idx);

private:
    QComboBox* m_cmbAction;    
    FrBrowseFileWidget* m_fileWidget;
    FrEmptyToolWidget* m_emptyWidget;
    FrSpinToolWidget* m_threshWidget;
    QStackedLayout* m_stacked;
};

#endif
