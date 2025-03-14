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

  int gridSize = ceil(sqrt(numSlices));
  ei->pixelSpacingReadMM = img->pixdim[1];
  ei->pixelSpacingPhaseMM = img->pixdim[2];
  ei->pixelSpacingSliceMM = img->pixdim[3];
  ei->sliceGapMM = 0;

  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      ei->voxelToWorldMatrix[r][c] = img->qto_xyz.m[r][c];
    }
  }

  ei->numPixelsRead = img->dim[1];
  ei->numPixelsPhase = img->dim[2];
  ei->numSlices = numSlices;

  ei->repetitionTimeMS = img->pixdim[4];
  ei->repetitionDelayMS = 0;
  ei->currentTR = repetition;
  ei->totalTR = totalReps;

  ei->isMotionCorrected = true;

  srand(time(NULL));
  ei->mcTranslationXMM = 2 * (rand()/static_cast<double>(RAND_MAX) - 0.5);
  ei->mcTranslationYMM = 2 * (rand()/static_cast<double>(RAND_MAX) - 0.5);
  ei->mcTranslationZMM = 2 * (rand()/static_cast<double>(RAND_MAX) - 0.5);
  ei->mcRotationXDeg = 2.3 * (rand()/static_cast<double>(RAND_MAX) - 0.5);
  ei->mcRotationYDeg = 2.3 * (rand()/static_cast<double>(RAND_MAX) - 0.5);
  ei->mcRotationZDeg = 2.3 * (rand()/static_cast<double>(RAND_MAX) - 0.5);
}

#endif
