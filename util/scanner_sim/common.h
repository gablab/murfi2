#ifndef COMMON_H
#define COMMON_H

void fillExternalInfo(nifti_image* img, int numSlices, int repetition,
                      int totalReps, RtExternalImageInfo* ei) {
  strcpy(ei->magic, "SIMU");

  strcpy(ei->scanType, "EPI");
  strcpy(ei->imageType, "3D");
  strcpy(ei->dataType, "int16_t");

  ei->isLittleEndian = true;
  ei->isMosaic = true;

  ei->numPixelsPhase = img->dim[1];
  ei->numPixelsRead = img->dim[2];

  int gridSize = ceil(sqrt(numSlices));
  ei->pixelSpacingReadMM = img->pixdim[1];
  ei->pixelSpacingPhaseMM = img->pixdim[2];
  ei->pixelSpacingSliceMM = img->pixdim[3];
  ei->sliceGapMM = 0;

  ei->numPixelsRead = img->dim[1] / gridSize;
  ei->numPixelsPhase = img->dim[2] / gridSize;
  ei->numSlices = numSlices;

  // TODO set the voxelToWorldMatrix

  ei->repetitionTimeMS = img->pixdim[4];
  ei->repetitionDelayMS = 0;
  ei->currentTR = repetition;
  ei->totalTR = totalReps;

  ei->isMotionCorrected = false;
}

#endif
