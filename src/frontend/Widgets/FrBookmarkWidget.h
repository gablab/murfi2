// class for displaying bookmarks

#ifndef FR_BOOKMARKWIDGET
#define FR_BOOKMARKWIDGET

class QAction;
class QTabWidget;
class QToolButton;
class QVBoxLayout;

class FrBookmark;

#include "Qt/QWidget.h"
#include <vector>
using namespace std;


class FrBookmarkWidget: public QWidget
{
	Q_OBJECT
public:
	FrBookmarkWidget(QWidget *parent = 0);

public slots:
    // Add slots here
    void AddBookmark(QString& tabName, QString& tabDescription);
	void BookmarkClicked(FrBookmark &bookmark);
    void OnCloseButtonClicked();

signals:
    void DeleteTab(int tabID);

private:
    QTabWidget*         m_tabWidget;
    QToolButton*        m_btnCloseTab;
	QVBoxLayout*        m_layout;
	
	FrBookmark*         m_defaultTab;
    vector<FrBookmark*> m_bookmarks;
};

#endif