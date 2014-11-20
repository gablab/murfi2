#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

#include "RtDataID.h"
#include "RtDataIDs.h"

//
#include"RtExperiment.h"

using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // register me to get the data
  getDataStore().addListener(this);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::notify(const RtDataID &id) {
  if (id.getDataName() == NAME_SCANNERIMG_EPI) {
    ui->imageWidget->addImage(static_cast<RtMRIImage*>(
                                getDataStore().getData(id)));
  }
}

void MainWindow::newExperiment() {
  initExperiment();
}

void MainWindow::newRun() {
}
