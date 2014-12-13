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
  ui->imageWidget->addImage(getDataStore().getData(id));
}

void MainWindow::newExperiment() {
  initExperiment();
}

void MainWindow::newRun() {
}

void MainWindow::openRun() {
  string filename = QFileDialog::getOpenFileName(
    this, tr("Open run config"), "", tr("Files (*.xml)")).toStdString();

  if (filename.empty()) {
    return;
  }

  RtConfigFmriRun run_config;
  run_config.parseConfigFile(filename);

  // TODO validation

  executeRun(run_config);
}
