#include "FrMainWindow.h"

FrMainWindow::FrMainWindow(QWidget *parent): QMainWindow(parent)
{
	setupUi(this);
	
	// actions of File-menu
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

	// actions of Help-menu
	connect(actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}