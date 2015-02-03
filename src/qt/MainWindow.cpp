#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

#include "RtDataID.h"
#include "RtDataIDs.h"
#include "RtDesignMatrix.h"

//
#include"RtExperiment.h"

#include "DesignEditor.h"
#include "PlotController.h"
#include "UnivariateRunEditor.h"

using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , plot_controller()
{
  ui->setupUi(this);
  plot_controller = new PlotController(ui->designPlotWidget,
                                       ui->roiPlotWidget,
                                       ui->motionPlotWidget);
  ui->imageWidget->setMainWindow(this);

  // register me to get the data
  getDataStore().addListener(this);

  QObject::connect(this, SIGNAL(dataReady(const QString &)),
                   plot_controller, SLOT(handleData(const QString &)),
                   Qt::QueuedConnection);

  if (!getExperimentConfigFile().empty()) {
    initExperiment();
    RtConfigFmriRun run_config;
    run_config.parseConfigFile(getExperimentConfigFile());
    ui->imageWidget->initRun(run_config);
    executeRun(run_config);
  }
}

MainWindow::~MainWindow() {
  delete ui;
  delete plot_controller;
}

void MainWindow::notify(const RtDataID &id) {
  ui->imageWidget->handleData(id);
  emit dataReady(QString(id.toString().c_str()));
}

void MainWindow::newExperiment() {
  initExperiment();
}

void MainWindow::openRun() {
  string filename = QFileDialog::getOpenFileName(
    this, tr("Open run config"), "", tr("Files (*.xml)")).toStdString();

  if (filename.empty()) {
    return;
  }

  RtConfigFmriRun run_config;
  if (!run_config.parseConfigFile(filename)) {
    cerr << "ERROR: Failed to parse config file" << endl;
    return;
  }

  ui->imageWidget->initRun(run_config);

  run_config.dumpConfig();

  executeRun(run_config);
}

const QColor& MainWindow::getColorForName(const string &name) {
  return plot_controller->getColorForName(name);
}

void MainWindow::newUnivariateRun() {
  RtConfigFmriRun run_config;
  UnivariateRunEditor editor(this, &run_config);
  editor.exec();

  if (editor.hasFinished()) {
    executeRun(run_config);
  }
}

void MainWindow::editDesign() {

  // get the design to edit
  RtDataID any_design;
  any_design.setDataName(NAME_DESIGN);
  RtDesignMatrix *design = static_cast<RtDesignMatrix*>(
    getDataStore().getData(any_design));

  // create a new design if none exists
  bool new_design = false;
  if (design == NULL) {
    design = new RtDesignMatrix();
    new_design = true;
  }

  DesignEditor design_editor(this, design);
  design_editor.exec();

  if (!design_editor.hasFinished()) {
    return;
  }

  if (new_design) {
    getDataStore().setData(design);
  }
  else {
    plot_controller->replotDesign(design);
  }
}
