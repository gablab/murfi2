/******************************************************************************
 * RtQtWindow.h class for showing real-time functional imaging results
 *
 * Oliver Hinds <ohinds@mit.edu> 2010-10-13
 * 
 * based on an the 'texture' opengl example qt program, 
 * which is Copyright (C) 1992-2008 Trolltech ASA.  
 *
 ****************************************************************************/

#ifndef RTQTWINDOW_H
#define RTQTWINDOW_H

#include<qwidget.h>
#include"RtDataListener.h"

class RtQtWindow 
	: public QWidget,
	  public RtDataListener
{
    Q_OBJECT

public:

    RtQtWindow(QWidget* parent = 0, const char* name = 0);
	
	// open the window and return when app exits
	static int run(int argc, char **argv);
	
	// notification that new rt data is available
	void notify(const RtDataID &id);

public slots:

    void onLoadRunConfig();
    void onLoadExperimentConfig();

};


#endif
