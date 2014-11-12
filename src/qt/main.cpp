#include "MainWindow.h"
#include <QApplication>

#include "RtExperiment.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!initializeSystem(argc, argv)) {
      return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
