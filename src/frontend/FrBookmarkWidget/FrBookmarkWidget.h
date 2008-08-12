// class for displaying bookmarks

#ifndef FR_BOOKMARKWIDGET
#define FR_BOOKMARKWIDGET

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include "Qt/qboxlayout.h"
#include "Qt/QMessageBox.h"

#include "FrBookmark.h"

#include <vector>
using namespace std;


class FrBookmarkWidget: public QDialog
{
	Q_OBJECT
public:
	FrBookmarkWidget(QWidget *parent = 0);

	public slots:
		void addBookmark();
		void bookmarkClicked(FrBookmark &bookmark);

protected:


private:
	QPushButton* addBookmarkButton;
	QVBoxLayout *layout;
	QSpacerItem *spacerItem;
	vector<FrBookmark*> bookmarks;
	FrBookmark* defaultTab;
	int nob;	// nummber of bookmarks

};

#endif