#pragma once

#include <QtOpenGL>
#include "Image.h"

class MRImage : public Image {
 public:

  MRImage();

  virtual ~MRImage() {}

 private:

  void updateTexture();

  bool first_image;
};
