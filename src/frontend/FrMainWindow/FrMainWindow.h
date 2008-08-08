#ifndef FR_MAIN_WINDOW
#define FR_MAIN_WINDOW

#include "ui_FrMainWindow.h"


class FrMainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
public:
	FrMainWindow(QWidget *parent = 0);

private:


protected:


};

#endif