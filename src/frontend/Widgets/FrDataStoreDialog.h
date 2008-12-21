#ifndef FR_DATASTORE_DLG
#define FR_DATASTORE_DLG

// Forward declarations 
class QTreeWidget;
class QHBoxLayout;
class FrBrowseFileWidget;
class FrMainDocument;

// includes
#include "Qt/qstring.h"
#include "QtGui/qdialog.h"


// This class aloow user to choose 
// how to create ROI
class FrDataStoreDialog: public QDialog {
	Q_OBJECT
public:
    FrDataStoreDialog(QWidget* parent = 0L, bool isModal = true);

    // Accessors / modifiers
    void SetCaption(QString& caption);
    // Exec method
    bool SimpleExec();
    
    // Initializer
    void Initialize(FrMainDocument* doc);

private:
    QHBoxLayout* CreateButtonLayout();
    QTreeWidget* CreateTreeViewWidget();

private Q_SLOTS:
    
private:
    QTreeWidget* m_tvDataStore;
    QTreeWidget* m_tvApplication;
    
};

#endif
