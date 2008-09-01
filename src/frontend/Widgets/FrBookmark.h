// class for storing bookmark info

#ifndef FR_BOOKMARK
#define FR_BOOKMARK

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include "Qt/QMessageBox.h"


class FrBookmark: public QWidget
{
	Q_OBJECT
public:
	FrBookmark(QWidget *parent = 0);
	
public slots:

signals:
	void bmClicked(FrBookmark &bookmark);

protected:
	void mousePressEvent(QMouseEvent *event);

private:


};

#endif