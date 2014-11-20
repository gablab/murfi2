#pragma once

#include <map>
#include <string>

#include <QtWidgets>
#include <QGLWidget>
#include <QOpenGLFunctions>

#include "RtMRIImage.h"

#include "Image.h"

class ImageWidget : public QGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit ImageWidget(QWidget *parent=0);
  ~ImageWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void addImage(RtData *img);

 signals:
  void clicked();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

 private:
  void makeObject();

  std::map<string, Image*> layers;
};
