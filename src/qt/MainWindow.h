#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private slots:
  // void newRun();
  // void openRun();
  // bool saveRun();
  // bool saveRunAs();
  // void runAgain();

 private:



  Ui::MainWindow *ui;
};
