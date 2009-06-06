
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<strstream>
#include<iostream>
#include<cstring>
#include<cstdio>

#include"dcmtk/config/osconfig.h"
#include"dcmtk/dcmimgle/dcmimage.h"

#include"../../src/io/RtExternalSenderImageInfo.h"

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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {

  // args
  string dicomStem(argc > 1 ? argv[1] : "/data/subjects/xc_ferro/TrioTim-35115-20070929-151440-250000/250000");
  int series = argc > 2 ? atoi(argv[2]) : 10;
  int numImgs = argc > 3 ? atoi(argv[3]) : 248;
  long tr = 1000*(argc > 4 ? atof(argv[4]) : 2000);
  int port = argc > 5 ? atoi(argv[5]) : 15000;
  string host(argc > 6 ? argv[6] : "localhost");

  cout << "1 using dicomStem=" << dicomStem << endl;
  cout << "2 using series=" << series << endl;
  cout << "3 using numImgs=" << numImgs << endl;
  cout << "4 using tr=" << tr << endl;
  cout << "5 using port=" << port << endl;
  cout << "6 using host=" << host << endl;

  // Local server address.
  ACE_INET_Addr my_addr (port, host.c_str());
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Connector connector;

  // keep making new connections while we havent sent the whole series
  DicomImage *image;
  for(int i = 0; i < numImgs && (image = loadNextInSeries(dicomStem,series)) != NULL
	&& !connector.connect (stream, my_addr); i++) {

    cout << "made connection, loading image" << endl;

    image->setMinMaxWindow();
    unsigned short *upixelData = (unsigned short*)(image->getOutputData(16 /* bits */));

    RtExternalImageInfo *ei = new RtExternalImageInfo();

    ei->lImageDataLength = image->getOutputDataSize();
    ei->lNumberOfPixels = ei->lImageDataLength/2;

    ei->bIsMoCo = true;
    ei->iNoOfImagesInMosaic = 32;
    ei->iMosaicGridSize = 6;

    ei->nCol = image->getHeight()/ei->iMosaicGridSize;
    ei->nLin = image->getWidth()/ei->iMosaicGridSize;
//    ei->nCol = 64;
//    ei->nLin = 64;
    ei->dThick = 3.5;

    ei->dPosSag = 2.50517;
    ei->dPosCor = -29.9335;
    ei->dPosTra = -75.1856;

    ei->dNorSag = -0.00637429;
    ei->dNorCor = 0.337923;
    ei->dNorTra = 0.941152;

    ei->dRowSag = 0.99998;
    ei->dRowCor = 0.00194039;
    ei->dRowTra = 0.00607602;

    ei->dColSag = 0.000227022;
    ei->dColCor = 0.941172;
    ei->dColTra = -0.337928;

    ei->iAcquisitionNumber = i+1;
    cout << "sending img  " << ei->iAcquisitionNumber << endl;

    char *data = new char[ei->iSizeOfRtExternalImageInfo];
    data = ei->convertToScannerDataArray();
    cout << "sending info of size " << ei->iSizeOfRtExternalImageInfo << endl;
    stream.send_n (data, ei->iSizeOfRtExternalImageInfo);
    delete data;

    cout << "sending img of size " << ei->lImageDataLength << endl;

    // shorten to 12 bits
    short *pixelData = (short*) malloc(image->getOutputDataSize());
    for(int i = 0; i < ei->lImageDataLength/2; i++) {
      pixelData[i] = upixelData[i]/16;
    }  
    stream.send_n (pixelData, ei->lImageDataLength);

    usleep(tr);

    stream.close();

    delete ei;
    delete image;
    free(pixelData);
  }

  return 0;
}
