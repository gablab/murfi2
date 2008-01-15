
#include<strstream>
#include<iostream>
#include<cstring>
#include<cstdio>

#include"dcmtk/config/osconfig.h"
#include"dcmtk/dcmimgle/dcmimage.h"

#include"../../src/io/RtExternalSenderImageInfo.h"
#include"../../src/data/RtMRIImage.h"

using namespace std;

DicomImage *loadNextInSeries(string dicomStem, int series) {
  // set up series info
  static int lastseries = -1;
  static int lastframe = 0;

  int frame;

  if(series != lastseries) {
    lastseries = series;
    frame = 1;
  }
  else {
    frame = lastframe+1;
  }

  char filename[256];
  sprintf(filename, "%s-%d-%d.dcm", dicomStem.c_str(), series, frame);


  cout << "loading dicom " << filename << endl;

  DicomImage *image = new DicomImage(filename);
  if (image == NULL || image->getStatus() != EIS_Normal) {
    cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
    return NULL;
  }

  lastframe = frame;
  lastseries = series;

  return image;
}

#include"testAccumCor.cpp"
void processImage(RtMRIImage &img) {
  process(&img);
}

int main(int argc, ACE_TCHAR *argv[]) {
  string dicomStem(argc > 1 ? argv[1] : "/data/subjects/xc_ferro/TrioTim-35115-20070929-151440-250000/250000");
  int series = argc > 2 ? atoi(argv[2]) : 10;
  long tr = 2000000;

  // keep making new connections while we havent sent the whole series
  DicomImage *image;
  for(int i = 0; i < 2 && (image = loadNextInSeries(dicomStem,series)) != NULL; i++) {

    cout << "loading image" << endl;

    image->setMinMaxWindow();
    Uint16 *pixelData = (Uint16*)(image->getOutputData(16 /* bits */));
  
    RtExternalImageInfo *ei = new RtExternalImageInfo();

    ei->nCol = image->getHeight();
    ei->nLin = image->getWidth();

    ei->lImageDataLength = image->getOutputDataSize();
    ei->lNumberOfPixels = ei->lImageDataLength/2;

    ei->iAcquisitionNumber = i;
    cout << "sending img  " << ei->iAcquisitionNumber << endl;

    RtMRIImage mriImage(*ei, pixelData);

    processImage(mriImage);

    delete ei;
    delete image;
  }

  return 0;
}
