#include "MaskImage.h"

#include "RtMaskImage.h"

namespace {
float color_table[][3] =
  {
    {0.0f, 1.0f, 0.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f}
  };
} // anonymous namespace

MaskImage::MaskImage(int color_index)
  : Image()
  , opacity(0.5f)
{
  for (int i = 0; i < 3; i++) {
    rgb[i] = color_table[color_index][i];
  }
}

void MaskImage::updateTexture() {
  have_data = false;

  if (glIsTexture(texture_id)) {
    glDeleteTextures(1, &texture_id);
  }

  glGenTextures(1, &texture_id);

  RtMaskImage *mask = static_cast<RtMaskImage*>(data);

  glBindTexture(TEXTURE_TYPE, texture_id);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  short *data_ptr;
  if(!mask->isMosaic()) {
    data_ptr = mask->getMosaicedCopy();
    image_width = mask->getMosaicedWidth();
    image_height = mask->getMosaicedHeight();
  }
  else {
    data_ptr = mask->getDataCopy();
    image_width = mask->getDim(0);
    image_height = mask->getDim(1);
  }

  // convert overlay data into a displayable image
  int overlay_len = 4 * image_width * image_height;
  short *overlay_data = new short[overlay_len];

  for(int i = 0; i < image_width * image_height; i++) {
    // rgb
    for (int j = 0; j < 3; j++) {
      overlay_data[4*i+j] = data_ptr[i] * SHRT_MAX * rgb[j];
    }
    overlay_data[4*i+3] = data_ptr[i] * SHRT_MAX * opacity; // a
  }
  delete [] data_ptr;

  glTexImage2D(TEXTURE_TYPE, 0, 4, image_width, image_height,
               0, GL_RGBA, GL_SHORT, overlay_data);

  delete [] overlay_data;

  if(!glIsTexture(texture_id)) {
    cerr << "ERROR: could not generate a new texture" << endl;
  }
  else {
    have_data = true;
  }
}
