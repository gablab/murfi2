// class for displaying bookmarks

#ifndef FR_GRAPH_BOOKMARKWIDGET
#define FR_GRAPH_BOOKMARKWIDGET

class QAction;
class QTabWidget;
class QToolButton;
class QHBoxLayout;

class FrBookmark;
class FrGraphTabDocObj;

#include "Qt/qwidget.h"
#include <vector>
using namespace std;


class FrGraphBookmarkWidget: public QWidget {
	Q_OBJECT
public:
	FrGraphBookmarkWidget(QWidget *parent = 0);

    bool AddBookmark(FrGraphTabDocObj* gts);
    bool RemoveBookmark(FrGraphTabDocObj* gts);

    int GetBookmarkCount();

public slots:
    // Add slots here
    void OnCloseButtonClicked();
    void OnCurrentChanged(QWidget* page);

signals:
    void DeleteTab(unsigned long tabID);
    void CurrentChanged(unsigned long tabID);

private:
    QTabWidget*         m_tabWidget;
    QToolButton*        m_btnCloseTab;
	QHBoxLayout*        m_layout;
	
	FrBookmark*         m_defaultTab;
    vector<FrBookmark*> m_bookmarks;
};

#endif
