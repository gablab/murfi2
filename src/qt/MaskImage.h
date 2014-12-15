#pragma once

#include <QtOpenGL>
#include "Image.h"

class MaskImage : public Image {
 public:

  MaskImage(int color_index);

  virtual ~MaskImage() {}

 private:

  void updateTexture();

  float rgb[3];
  float opacity;
};
