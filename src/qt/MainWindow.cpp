#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

#include "RtDataID.h"
#include "RtDataIDs.h"

//
#include"RtExperiment.h"

#include "PlotController.h"

using std::cout;
using std::endl;

namespace {
const int NUM_COLORS = 4;
static int color_table[NUM_COLORS][3] =
{
  {  0, 255,   0},
  {255,   0, 255},
  {255, 255,   0},
  {  0, 255, 255}
};
} // anonymous namespace

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , plot_controller()
{
  ui->setupUi(this);
  plot_controller = new PlotController(this,
                                       ui->designPlotWidget,
                                       ui->roiPlotWidget,
                                       ui->motionPlotWidget);
  ui->imageWidget->setMainWindow(this);

  // register me to get the data
  getDataStore().addListener(this);

  QObject::connect(this, SIGNAL(dataReady(const QString &)),
                   plot_controller, SLOT(handleData(const QString &)),
                   Qt::QueuedConnection);
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

  ui->imageWidget->initRun(run_config);

  // TODO validation

  executeRun(run_config);
}

QColor MainWindow::getColor(int color) {
  int color_ind = color % NUM_COLORS;
  return QColor(color_table[color_ind][0],
                color_table[color_ind][1],
                color_table[color_ind][2]);
}

QColor MainWindow::getColorForName(const string &name) {

  map<string, QColor>::const_iterator it = name_colors.find(name);
  if (it == name_colors.end()) {
    int color_ind = name_colors.size() % NUM_COLORS;
    it = name_colors.insert(
      pair<string, QColor>(name, QColor(color_table[color_ind][0],
                                        color_table[color_ind][1],
                                        color_table[color_ind][2]
                                        ))).first;
  }

  return it->second;
}
