#include <cstdlib>
#include <iostream>

#include "MainWindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>

#include "RtExperiment.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (getenv("MURFI_SUBJECTS_DIR") == NULL ||
        getenv("MURFI_SUBJECT_NAME") == NULL) {


      QString dir = QFileDialog::getExistingDirectory(
        NULL, "Subject directory", getenv("MURFI_SUBJECTS_DIR"),
        QFileDialog::ShowDirsOnly);

      if (!dir.isEmpty()) {
        string chosen = dir.toStdString();
        size_t delim = chosen.rfind('/');
        if (delim == string::npos || delim == 0) {
          cerr << "failed to parse subject directory" << endl;
          return 1;
        }

        string subjs_dir = chosen.substr(0, delim);
        string subj = chosen.substr(delim + 1);

        cout << "Setting MURFI_SUBJECTS_DIR=" << subjs_dir << endl;
        setenv("MURFI_SUBJECTS_DIR", subjs_dir.c_str(), 1);

        cout << "Setting MURFI_SUBJECT_NAME=" << subj << endl;
        setenv("MURFI_SUBJECT_NAME", subj.c_str(), 1);
      }
      else {
        cerr << "Subjects directory must be specified" << endl;
        return 1;
      }

    }

    if (!initializeSystem(argc, argv)) {
      return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
