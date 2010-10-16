/******************************************************************************
 * RtQtWindow.h class for showing real-time functional imaging results
 *
 * Oliver Hinds <ohinds@mit.edu> 2010-10-13
 *
 * based on an the 'texture' opengl example qt program,
 * which is Copyright (C) 1992-2008 Trolltech ASA.
 *
 ****************************************************************************/


#include "RtQtWindow.h"
#include "RtQtGLTexture.h"

#include "RtExperiment.h"

#include <qapplication.h>
#include <qframe.h>
#include <qfiledialog.h>
#include <qkeycode.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>


RtQtWindow::RtQtWindow(QWidget* parent, const char* name)
    : QWidget(parent, name)
{
    // frame
    QFrame* f1 = new QFrame(this, "frame1");
    f1->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    f1->setLineWidth(2);

    // opengl widget for the realtime images
    RtQtGLTexture* c = new RtQtGLTexture(f1, "rtimg");

    // menu
    QPopupMenu *file = new QPopupMenu(this);
    file->insertItem("load run config", this,
					 SLOT(onLoadRunConfig()), CTRL+Key_R);
    file->insertItem("load experiment config", this,
					 SLOT(onLoadExperimentConfig()), CTRL+Key_E);
    file->insertSeparator();
    file->insertItem("quit",  qApp, SLOT(quit()), CTRL+Key_Q);

    // menu bar
    QMenuBar *m = new QMenuBar(this);
    m->setSeparator(QMenuBar::InWindowsStyle);
    m->insertItem("&file", file);

    /// layout

    // rtimg inside the frame
    QHBoxLayout* flayout1 = new QHBoxLayout(f1, 2, 2, "flayout1");
    flayout1->addWidget(c, 1);

    // top level layout
    QHBoxLayout* hlayout = new QHBoxLayout(this, 20, 20, "hlayout");
    hlayout->setMenuBar(m);
    hlayout->addWidget(f1, 1);
}

int RtQtWindow::run(int argc, char **argv)
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a(argc,argv);

//    if ( !QGLFormat::hasOpenGL() ) {
//	qWarning( "This system has no OpenGL support. Exiting." );
//	return -1;
//    }
    
	RtQtWindow rtWin(NULL,"murfi");

    rtWin.resize(400, 400);
    a.setMainWidget(&rtWin);
    rtWin.show();

    return a.exec();
}

void RtQtWindow::notify(const RtDataID& dataId)
{
    cout << "got data: " << dataId << endl;
}

void RtQtWindow::onLoadRunConfig()
{
	// TODO: check if there is an active run and disable this

	// get filename from dialog
	QString fileName 
		= QFileDialog::getOpenFileName(getExperimentConfig().get(
										   "study:confDir").str(), 
									   tr("murfi config files (*.xml)"),
									   this, tr("open murfi run config"));

	// parse, load, run
    RtConfigFmriRun runConfig;
	runConfig.parseConfigFile(fileName.ascii());
    executeRun(runConfig);
}

void RtQtWindow::onLoadExperimentConfig()
{
	// TODO: check if there is an active run and disable this

	// get filename from dialog
	QString fileName 
		= QFileDialog::getOpenFileName(getExperimentConfig().get(
										   "study:confDir").str(), 
									   tr("murfi config files (*.xml)"),
									   this, tr("open murfi experiment config"));


    getExperimentConfig().parseConfigFile(fileName.ascii());
}
