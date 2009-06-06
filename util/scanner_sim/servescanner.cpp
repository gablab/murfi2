
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<iostream>
#include<cstring>
#include<cstdio>

#include"../../src/io/RtExternalSenderImageInfo.h"

using namespace std;

// noise proerties
bool noise = false;
unsigned short max_noise = 50;


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  int num2send = argc > 1 ? atoi(argv[1]) : -1;

  char *data = (char*) calloc(EXTERNALSENDERSIZEOF,1);
  FILE* fp = fopen("scan_dat.hdr","rb");
  fread(data, EXTERNALSENDERSIZEOF, 1, fp);
  fclose(fp);

  // make the image info to send
  RtExternalImageInfo *ei = new RtExternalImageInfo(data, EXTERNALSENDERSIZEOF);


  char *send = (char*) calloc(EXTERNALSENDERSIZEOF,1);

  // make the test data to send
//  int i, numPix = ei->nCol * ei->nLin, 
//    imageDataSize = numPix * sizeof(unsigned short);
//  unsigned short *img = new unsigned short[numPix];
//  fp = fopen("scanner_testimg.dat","rb");
//  fread(img, sizeof(unsigned short), ei->nCol * ei->nLin, fp);
//  fclose(fp);
//  unsigned short *imgnoisy = new unsigned short[numPix];

  int i;

  delete ei;

  // Local server address.
  ACE_INET_Addr my_addr ( argc > 2 ? atoi(argv[2]) : 15000,
			  argc > 1 ? argv[1] : "localhost");
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Connector connector;

  // keep making new connections
  int numImg = 20;
  for(i = 1; (num2send < 0 || i <= num2send)  
	&& connector.connect (stream, my_addr)  != -1; i++) {
    char fn[100];
    sprintf(fn,"testimg/img%05d.dat",(i-1)%numImg+1);
    //sprintf(fn,"testimg/img00001.dat",(i-1)%numImg+1);
    //sprintf(fn,"scanner_testimg.dat");
    cout << "loading image " << fn << " to send" << endl;

    fp = fopen(fn,"rb");

    int w, h, c;

    fread(&w, sizeof(int), 1, fp);
    fread(&h, sizeof(int), 1, fp);
    fread(&c, sizeof(int), 1, fp);

    int numPix = w*h,
      imageDataSize = numPix * sizeof(unsigned short);
    unsigned short *img = new unsigned short[numPix];
    unsigned short *imgnoisy = new unsigned short[numPix];

    fread(img, sizeof(unsigned short), w*h, fp);
    fclose(fp);


    ei = new RtExternalImageInfo(data, EXTERNALSENDERSIZEOF);

    ei->nCol = h;
    ei->nLin = w;

    ei->iAcquisitionNumber = i;
    cout << "sending img  " << ei->iAcquisitionNumber << endl;
    send = ei->convertToScannerDataArray();

    //data = ei.convertToScannerDataArray();
    cout << "sending info of size " << ei->iSizeOfRtExternalImageInfo << endl;
    stream.send_n (send, ei->iSizeOfRtExternalImageInfo);
    //free(data);

    cout << "sending img of size " << imageDataSize << endl;

    // add some noise to the image
    if(noise) {
      memcpy(imgnoisy, img, sizeof(unsigned short)* ei->nCol * ei->nLin);
      for(int i = 0; i < ei->nCol * ei->nLin; i++) {
	unsigned short val;
	while (max_noise <= (val = rand() / (RAND_MAX/max_noise)));
	imgnoisy[i] = imgnoisy[i] + val;
      }

      stream.send_n (imgnoisy, imageDataSize);
    }
    else {
      stream.send_n (img, imageDataSize);
    }

    usleep(2000000);

    stream.close();
    delete ei;
    delete img;
    delete imgnoisy;
  }

  free(data);
  free(send);

  return 0;
}
