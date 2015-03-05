#pragma once

#include <QtOpenGL>
#include "RtData.h"

#ifndef TEXTURE_TYPE
#ifdef WIN32
#define TEXTURE_TYPE GL_TEXTURE_2D
#else
#ifdef GL_TEXTURE_RECTANGLE_EXT
#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT
#endif
#else
#ifdef GL_TEXTURE_RECTANGLE_NV
#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV
#endif
#endif
#endif
#define TEXTURE_TYPE GL_TEXTURE_RECTANGLE_EXT
#endif
#endif

class Image {
 public:

  Image();

  virtual ~Image() {}

  void setData(RtData *img);

  void paint();

 protected:

  virtual void updateTexture() = 0;

  RtData *data;
  bool needs_update;
  bool have_data;

  GLuint texture_id;

  int image_width;
  int image_height;
};
