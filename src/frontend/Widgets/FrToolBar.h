#ifndef FR_TOOL_BAR
#define FR_TOOL_BAR

//Forward declaration
class QWidget;
class FrActionManager;

// inclused
#include "Qt/qtoolbar.h"

// This class represents Toolbar of app
class FrToolBar : public QToolBar {
	Q_OBJECT
public:
    // Constructor
	FrToolBar(QWidget *parent, FrActionManager* mngr);

    // Methods
    void Retranslate();

private:
    void SetupActions();

private:
    FrActionManager* m_actManager;
};

#endif
