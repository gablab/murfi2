
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<strstream>
#include<iostream>
#include<ctime>

#include"nifti1_io.h"

#include"../../src/io/RtExternalSenderImageInfo.h"
#include"common.h"

using namespace std;

int firstImg = 0;

nifti_image *loadNextInSeries(string niiStem, int series) {
  static int lastframe = firstImg;

  char filename[256];
  sprintf(filename, "%s-%05d-%05d.nii", niiStem.c_str(), series, lastframe++);

  cout << "loading nifti " << filename << endl;

  nifti_image *img = nifti_image_read(filename, 0);
  if(img == NULL) {
    cerr << "could not open " << filename << " for reading a nifti image"
         << endl;
    return NULL;
  }

  // read the image data
  nifti_image_load(img);

  return img;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  // args
  string niiStem(argc > 1 ? argv[1] : "/data/subjects/realtime/ss_mini/img");
  int series = argc > 2 ? atoi(argv[2]) : 8;
  int numImgs = argc > 3 ? atoi(argv[3]) : 300;
  firstImg = argc > 4 ? atoi(argv[4]) : 0;
  int numSlices = argc > 5 ? atoi(argv[5]) : 32;
  long tr = 1000*(argc > 6 ? atof(argv[6]) : 1000);
  int port = argc > 7 ? atoi(argv[7]) : 15000;
  string host(argc > 8 ? argv[8] : "localhost");
  bool preHeader = argc > 9 ? strcmp(argv[9], "0") : false;

  cout << "1 using niiStem=" << niiStem << endl;
  cout << "2 using series=" << series << endl;
  cout << "3 using numImgs=" << numImgs << endl;
  cout << "4 using firstImg=" << firstImg << endl;
  cout << "5 using numSlices=" << numSlices << endl;
  cout << "6 using tr=" << tr << endl;
  cout << "7 using port=" << port << endl;
  cout << "8 using host=" << host << endl;
  cout << "9 using preHeader=" << preHeader << endl;

  // Local server address.
  ACE_INET_Addr my_addr (port, host.c_str());
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Connector connector;

  char uid[64];
  time_t tm;
  time(&tm);
  strftime(uid, 64, "%Y%m%d%H%M%S", localtime(&tm));
  unsigned int numPix, numData;

  // keep making new connections while we havent sent the whole series
  nifti_image* img;
  for(int i = 0; i < numImgs && (img = loadNextInSeries(niiStem,series)) != NULL
          && !connector.connect (stream, my_addr); i++) {
    cout << "made connection, loading image" << endl;

    RtExternalImageInfo ei;
    fillExternalInfo(img, numSlices, i+1, numImgs, &ei);
    strcpy(ei.seriesUID, uid);
    
    cout << "sending img  " << ei.currentTR << endl;

    int size_of_header = ei.getHeaderSize();
    cout << "sending info of size " << ei.getHeaderSize() << endl;

    int size_of_data = ei.getDataSize();
    cout << "sending img of size " << ei.getDataSize() << endl;

    if(preHeader) {
      stream.send_n(reinterpret_cast<char*>(&size_of_header), 4);
      stream.send_n(reinterpret_cast<char*>(&size_of_data), 4);
    }

    stream.send_n(reinterpret_cast<char*>(&ei), size_of_header);

    numPix = ei.getNumVoxels();
    numData = ei.numPixelsRead * ei.numPixelsPhase * ei.numSlices;
    short *newdata = new short[numPix];
    short *imgdata = (short*) img->data;

    unsigned int matrixSize = img->dim[1];
    unsigned int mosaicSide = (int) sqrt(matrixSize*matrixSize
                                         *pow(ceil(sqrt((double)numSlices)),2));
    unsigned int newrow, newcol, oldslc, newind;
    unsigned int sqMatrixSize = matrixSize*matrixSize;
    for(unsigned int p = 0; p < numPix; p++) {
      oldslc = p/sqMatrixSize;

      // these expressions are bracketed carefully to truncate
      // DO NOT SIMPLIFY THESE EXPRESSIONS
      newrow = (p/matrixSize)%matrixSize +
          matrixSize*(p/(mosaicSide*matrixSize));
      newcol = (matrixSize*(p/sqMatrixSize) + (p%matrixSize))%mosaicSide;
      newind = newrow*mosaicSide+newcol;

      // copy if within slices
      if(oldslc < numSlices) {
        newdata[newind] = imgdata[p];
      }
      else { // fill the blank panels with zeros
        newdata[newind] = 0;
      }
    }
    stream.send_n(newdata, ei.getDataSize());

    stream.close();

    usleep(tr);

    nifti_image_free(img);
    delete [] newdata;
  }

  if(img == NULL) {
    cout << "failed to read image" << endl;
  }
  else {
    cout << "failed to connect" << endl;
  }

  return 0;
}
