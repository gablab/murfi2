
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<strstream>
#include<iostream>

#include<X11/Xlib.h>
#include<X11/keysym.h>

#include"nifti1_io.h"

#include"../../src/io/RtExternalSenderImageInfo.h"

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

XKeyEvent createKeyEvent(Display *display, Window &win, Window &winRoot,
                         bool press, int keycode, int modifiers){
  XKeyEvent event;

  event.display     = display;
  event.window      = win;
  event.root        = winRoot;
  event.subwindow   = None;
  event.time        = CurrentTime;
  event.x           = 1;
  event.y           = 1;
  event.x_root      = 1;
  event.y_root      = 1;
  event.same_screen = True;
  event.keycode     = XKeysymToKeycode(display, keycode);
  event.state       = modifiers;

  if(press)
    event.type = KeyPress;
   else
     event.type = KeyRelease;

  return event;
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

  #define KEYCODE XK_KP_1

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

  // Obtain the X11 display.
  Display *display = XOpenDisplay(0);
  if(display == NULL)
    return -1;

  // Get the root window for the current display.
  Window winRoot = XDefaultRootWindow(display);

  Window winFocus;
  int    revert;
  XKeyEvent event;

  // keep making new connections while we havent sent the whole series
  nifti_image* img;
  for(int i = 0; i < numImgs && (img = loadNextInSeries(niiStem,series)) != NULL
	&& !connector.connect (stream, my_addr); i++) {
    cout << "made connection, loading image" << endl;
    RtExternalImageInfo *ei = new RtExternalImageInfo();

    ei->nCol = img->dim[1];
    ei->nLin = img->dim[2];

    ei->bIsMoCo = true;

    ei->iNoOfImagesInMosaic = numSlices;
    ei->iMosaicGridSize = ceil(sqrt(numSlices));

    ei->nCol = img->dim[1]/ei->iMosaicGridSize;
    ei->nLin = img->dim[2]/ei->iMosaicGridSize;
    ei->dThick = 3.5;

    cout 
      << "nCol " <<  ei->nCol << " "
      << "nLin " <<  ei->nLin << " "
      << "iNoOfImagesInMosaic " <<  ei->iNoOfImagesInMosaic << " "
      << "iMosaicGridSize " <<  ei->iMosaicGridSize << endl;

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


    ei->lImageDataLength = img->nbyper*img->nvox;
    ei->lNumberOfPixels = img->nvox;

    ei->iAcquisitionNumber = i+1;
    cout << "sending img  " << ei->iAcquisitionNumber << endl;

    char *data = new char[ei->iSizeOfRtExternalImageInfo];
    data = ei->convertToScannerDataArray();
    cout << "sending info of size " << ei->iSizeOfRtExternalImageInfo << endl;
    stream.send_n (data, ei->iSizeOfRtExternalImageInfo);
    delete data;

    cout << "sending img of size " << ei->lImageDataLength << endl;

    stream.send_n(img->data, ei->lImageDataLength);

  // Find the window which has the current keyboard focus.
  XGetInputFocus(display, &winFocus, &revert);
  // Send a fake key press event to the window.
  event = createKeyEvent(display, winFocus, winRoot, true, KEYCODE, 0);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);

  // Send a fake key release event to the window.
  event = createKeyEvent(display, winFocus, winRoot, false, KEYCODE, 0);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
    usleep(tr);

    stream.close();

    delete ei;
    nifti_image_free(img);
  }

  if(img == NULL) {
    cout << "failed to read image" << endl;
  }
  else {
    cout << "failed to connect" << endl;
  }

  // Done.
  XCloseDisplay(display);
  return 0;

  return 0;
}
