#include "FileButton.h"

#include <QFileDialog>
#include <QStringList>
#include <QTableWidget>

FileButton::FileButton(QTableWidget* parent)
  : QPushButton("Choose file...", parent) {
  connect(this, SIGNAL(clicked()), this, SLOT(engage()));
}

void FileButton::engage() {
  QString file = QFileDialog::getOpenFileName(
    this, "Choose mask file", "mask", "Volumes (*.nii *.nii.gz)");

  if (file.isEmpty()) {
    return;
  }

  // TODO platform dependent
  QStringList dir_base = file.split('/');
  setText(dir_base.back());

  path = file;
}
