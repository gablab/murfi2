
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<strstream>
#include<iostream>

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

  cout << "1 using niiStem=" << niiStem << endl;
  cout << "2 using series=" << series << endl;
  cout << "3 using numImgs=" << numImgs << endl;
  cout << "4 using firstImg=" << firstImg << endl;
  cout << "5 using numSlices=" << numSlices << endl;
  cout << "6 using tr=" << tr << endl;
  cout << "7 using port=" << port << endl;
  cout << "8 using host=" << host << endl;

  // Local server address.
  ACE_INET_Addr my_addr (port, host.c_str());
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Connector connector;

  // keep making new connections while we havent sent the whole series
  nifti_image* img;
  for(int i = 0; i < numImgs && (img = loadNextInSeries(niiStem,series)) != NULL
          && !connector.connect (stream, my_addr); i++) {
    cout << "made connection, loading image" << endl;

    RtExternalImageInfo ei;
    fillExternalInfo(img, numSlices, i+1, numImgs, &ei);

    cout << "sending img  " << ei.currentTR << endl;

    cout << "sending info of size " << ei.getHeaderSize() << endl;
    stream.send_n(reinterpret_cast<char*>(&ei), ei.getHeaderSize());

    cout << "sending img of size " << ei.getDataSize() << endl;
    stream.send_n(img->data, ei.getDataSize());

    usleep(tr);

    stream.close();

    nifti_image_free(img);
  }

  if(img == NULL) {
    cout << "failed to read image" << endl;
  }
  else {
    cout << "failed to connect" << endl;
  }

  return 0;
}
