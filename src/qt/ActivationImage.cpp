#include "ActivationImage.h"

#include <algorithm>

#include "RtActivation.h"
#include "RtMRIImage.h"

using std::min;

ActivationImage::ActivationImage()
  : Image()
  , threshold(3.f)
  , ceiling(5.f)
{}

void ActivationImage::updateTexture() {
  have_data = false;

  if (glIsTexture(texture_id)) {
    glDeleteTextures(1, &texture_id);
  }

  glGenTextures(1, &texture_id);

  RtActivation *img = static_cast<RtActivation*>(data);

  glBindTexture(TEXTURE_TYPE, texture_id);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  double *data_ptr = NULL;
  if(!img->isMosaic()) {
    data_ptr = img->getMosaicedCopy();
    image_width = img->getMosaicedWidth();
    image_height = img->getMosaicedHeight();
  }
  else {
    data_ptr = img->getDataCopy();
    image_width = img->getDim(0);
    image_height = img->getDim(1);
  }

  // convert overlay data into a displayable image
  int overlay_len = 4 * image_width * image_height;
  short *overlay_data = new short[overlay_len];

  for(int i = 0; i < image_width * image_height; i++) {
    if(data_ptr[i] > threshold) {
      overlay_data[4*i+0] = SHRT_MAX; // r
      overlay_data[4*i+1] = (short) rint(
          min(1.0, ((data_ptr[i]-threshold)/ceiling))
          *SHRT_MAX); // g
      overlay_data[4*i+2] = 0; // b
      overlay_data[4*i+3] = SHRT_MAX; // a
    }
    else if(data_ptr[i] < -threshold) {
      overlay_data[4*i+0] = 0; // r
      overlay_data[4*i+1] = (short) rint(
          min(1.0, -1*((data_ptr[i]+threshold)/ceiling))
          *SHRT_MAX); // g
      overlay_data[4*i+2] = SHRT_MAX; // b
      overlay_data[4*i+3] = SHRT_MAX; // a
    }
    else {
      overlay_data[4*i+0] = 0; // r
      overlay_data[4*i+1] = 0; // g
      overlay_data[4*i+2] = 0; // b
      overlay_data[4*i+3] = 0; // a
    }
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
