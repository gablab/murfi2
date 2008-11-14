#ifndef FR_BROWSEFILE_WIDGET
#define FR_BROWSEFILE_WIDGET

class QHBoxLayout;
class QPushButton;
class QLineEdit;

#include "Qt/qwidget.h"
#include "Qt/qstring.h"

class FrBrowseFileWidget : public QWidget {
    Q_OBJECT
public:
    FrBrowseFileWidget(QWidget* parent=0L);

    QString GetFileName();
    void SetFileName(const QString& fileName);
    
private Q_SLOTS:
    void OnBrowseClick();

private:
    QHBoxLayout* m_layout;
    QPushButton* m_btnBrowse;
    QLineEdit*   m_txtFile;
};

#endif
