#ifndef FR_MAIN_MENU
#define FR_MAIN_MENU

// Forward declaration
class QWidget;
class FrActionManager;

// includes
#include "Qt/QMenuBar.h"
#include "Qt/QMenu.h"


// This class represents main menu of app
class FrMainMenu : public QMenuBar {
	Q_OBJECT
public:
    // Constructor
	FrMainMenu(QWidget *parent, FrActionManager* mngr);

    // Methods
    void Retranslate();
	QMenu* GetMenuView(){return m_menuView;};

private:
    void Initialize();
    void SetupActions();

private:
    FrActionManager* m_actManager;
    QMenu* m_menuFile;
    QMenu* m_menuEdit;
    QMenu* m_menuView;
    QMenu* m_menuTools;
    QMenu* m_menuHelp;
    
};

#endif