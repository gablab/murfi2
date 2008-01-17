
#include<strstream>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<gnuplot_i_vxl.h>

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


  //  cout << "loading dicom " << filename << endl;

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

// good voxel is 17,29,15

// send a whole series
void sendDicomSeries(string dicomStem, int series) {
  long tr = 2000000;
  vector<double> ts;

  // keep making new connections while we havent sent the whole series
  DicomImage *image;
  for(int i = 0; (image = loadNextInSeries(dicomStem,series)) != NULL; i++) {

    //cout << "loading image" << endl;

    image->setMinMaxWindow();
    Uint16 *pixelData = (Uint16*)(image->getOutputData(16 /* bits */));

    ts.push_back((double) pixelData[16*32*32+28*32+14]);

    RtExternalImageInfo *ei = new RtExternalImageInfo();

    ei->nCol = image->getHeight();
    ei->nLin = image->getWidth();

    ei->lImageDataLength = image->getOutputDataSize();
    ei->lNumberOfPixels = ei->lImageDataLength/2;

    ei->iAcquisitionNumber = i;
    //cout << "sending img  " << ei->iAcquisitionNumber << endl;

    RtMRIImage mriImage(*ei, pixelData);

    process(&mriImage);

    delete ei;
    delete image;
  }

  Gnuplot g = Gnuplot("lines");
  g.plot_x(ts,"voxel timeseries");
  //  sleep(10000000);
}

// send a simple vector of eight zeros and eight ones repeating
void sendTestVector(unsigned int timepoints) {
  RtExternalImageInfo ei;

//   timepoints = 248;
//   unsigned short timeSeries[] = {
//    860,
//    849,
//    855,
//    849,
//    847,
//    844,
//    844,
//    844,
//    834,
//    850,
//    860,
//    883,
//    894,
//    904,
//    917,
//    910,
//    921,
//    913,
//    897,
//    854,
//    828,
//    826,
//    817,
//    807,
//    804,
//    847,
//    863,
//    878,
//    887,
//    891,
//    896,
//    895,
//    911,
//    906,
//    891,
//    847,
//    803,
//    796,
//    789,
//    781,
//    793,
//    827,
//    860,
//    873,
//    873,
//    888,
//    895,
//    900,
//    903,
//    904,
//    892,
//    858,
//    828,
//    817,
//    810,
//    810,
//    805,
//    832,
//    855,
//    867,
//    875,
//    891,
//    891,
//    895,
//    897,
//    902,
//    880,
//    845,
//    817,
//    813,
//    814,
//    804,
//    800,
//    827,
//    847,
//    861,
//    868,
//    882,
//    885,
//    895,
//    901,
//    898,
//    887,
//    858,
//    819,
//    815,
//    817,
//    807,
//    815,
//    842,
//    866,
//    872,
//    878,
//    893,
//    895,
//    898,
//    897,
//    896,
//    890,
//    859,
//    809,
//    811,
//    798,
//    796,
//    790,
//    809,
//    846,
//    868,
//    884,
//    892,
//    889,
//    898,
//    898,
//    906,
//    891,
//    853,
//    818,
//    810,
//    810,
//    822,
//    811,
//    829,
//    856,
//    864,
//    881,
//    886,
//    893,
//    905,
//    909,
//    908,
//    894,
//    854,
//    830,
//    822,
//    824,
//    816,
//    810,
//    847,
//    870,
//    884,
//    894,
//    891,
//    888,
//    892,
//    897,
//    912,
//    902,
//    885,
//    854,
//    822,
//    823,
//    807,
//    806,
//    829,
//    860,
//    868,
//    885,
//    896,
//    897,
//    901,
//    903,
//    901,
//    884,
//    844,
//    822,
//    821,
//    821,
//    818,
//    814,
//    839,
//    860,
//    877,
//    885,
//    893,
//    910,
//    912,
//    917,
//    913,
//    906,
//    866,
//    817,
//    805,
//    805,
//    807,
//    812,
//    830,
//    862,
//    868,
//    884,
//    883,
//    896,
//    898,
//    904,
//    902,
//    894,
//    865,
//    820,
//    808,
//    805,
//    806,
//    800,
//    812,
//    838,
//    861,
//    879,
//    881,
//    888,
//    903,
//    902,
//    911,
//    903,
//    863,
//    823,
//    808,
//    823,
//    822,
//    818,
//    834,
//    853,
//    877,
//    886,
//    891,
//    894,
//    896,
//    903,
//    909,
//    893,
//    862,
//    823,
//    806,
//    813,
//    827,
//    820,
//    837,
//    855,
//    865,
//    884,
//    886,
//    903,
//    905,
//    909,
//    910,
//    893,
//    870,
//    826,
//    812,
//    812,
//    807};

  ei.nCol = 1;
  ei.nLin = 1;
  ei.lImageDataLength = sizeof(short);
  ei.lNumberOfPixels = ei.lImageDataLength/2;


  for(unsigned int t = 1; t <= timepoints; t++) {
    ei.iAcquisitionNumber = t;
    
    unsigned short pix = (t-1)%16 > 7;
    RtMRIImage img(ei,&pix);

    process(&img);
  }
}


int main(int argc, char *argv[]) {
  if(argc > 2) { // send series
    sendDicomSeries(argv[1],atoi(argv[2]));
  }
  else { // send a test vector of length argv[1]
    sendTestVector(atoi(argv[1]));
  }  

  return 0;
}
