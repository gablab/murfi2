#pragma once

#include <string>

#include <QPushButton>
#include <QString>

class QTableWidget;

// custom button to hold a filename
class FileButton : public QPushButton {
  Q_OBJECT

 public:

  explicit FileButton(QTableWidget* parent);

  virtual ~FileButton() {}

  std::string getPath() const {
    return path.toStdString();
  }

 public slots:

  void engage();

 private:

  QString path;
};
