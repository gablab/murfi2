#pragma once

#include <vector>

#include <QtWidgets>
#include <QGLWidget>
#include <QOpenGLFunctions>

#include "Image.h"

class ImageWidget : public QGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit ImageWidget(QWidget *parent=0);
  ~ImageWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void addImage(RtMRIImage *img);

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

  std::vector<Image> images;
};
