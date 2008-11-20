// class for storing bookmark info
#ifndef FR_PLAYCTL_WGT
#define FR_PLAYCTL_WGT


#include "FrMacro.h"

#include "Qt/qstring.h"
#include "Qt/qwidget.h"

class FrPlayControlWidget : public QWidget {
	Q_OBJECT
public:
	FrPlayControlWidget(QWidget *parent = 0);
	
public Q_SLOTS:
    // Add slots here 

Q_SIGNALS:    
    // add signals here

private:
    /*QAction* m_Play;
    QAction* m_Pause;
    QAction* m_Reset;
    QAction* m_Faster;
    QAction* m_Slower;
    QToolBar* m_Pane; */

};

#endif
