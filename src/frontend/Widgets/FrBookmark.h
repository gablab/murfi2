// class for storing bookmark info

#ifndef FR_BOOKMARK
#define FR_BOOKMARK

#include "FrMacro.h"

#include "Qt/qstring.h"
#include "Qt/qwidget.h"

class FrBookmark: public QWidget
{
	Q_OBJECT
public:
	FrBookmark(QWidget *parent = 0);
    FrGetPropMacro(int, ID);

    FrPropMacro(bool,IsDefault);
	
public slots:
    // Add slots here 

signals:    
    // add signals here
	void Clicked(FrBookmark &bookmark);

protected:
	virtual void mousePressEvent(QMouseEvent *event);

private:
    static int s_CurrentID;
};

#endif
