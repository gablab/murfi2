// class for displaying bookmarks

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include "Qt/qboxlayout.h"
#include "Qt/QMessageBox.h"


class FrBookmarkWidget: public QDialog
{
	Q_OBJECT
public:
	FrBookmarkWidget(QWidget *parent = 0);

	public slots:
		void addBookmark();

protected:


private:
	QPushButton* addBookmarkButton;
	
};