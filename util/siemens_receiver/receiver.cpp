/******************************************************************************
 * program to save images sent from a scanner using the external
 * sender ice functor
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-05-31
 *****************************************************************************/

#include"ExternalSenderImageInfo.h"

#include <winsock.h>

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

///// defaults
static const int defaultPort = 15000;
static const string defaultDir(".");
static const string defaultFilestem("img");

///// types
enum ImageFileType {RAW, NII};
static const ImageFileType defaultImageType = RAW;

// params
struct Params {
  unsigned int port;
  string dir;
  string filestem;
  ImageFileType imagetype;

  Params::Params() :
    port(defaultPort),
    dir(defaultDir),
    filestem(defaultFilestem),
    imagetype(defaultImageType) {};
};


///// prototypes
int runReceiverWin(const Params &p);
SOCKET getListenerWin(int port);
SOCKET acceptWin(SOCKET sock, sockaddr_in& remote);
string getImageFilename(const Params &p, int runNum, int imgNum, const ExternalImageInfo &info);
ExternalImageInfo *readImageInfoWin(SOCKET sock);
short *readImageDataWin(SOCKET sock, const ExternalImageInfo &info);
int saveImage(const Params &p, const ExternalImageInfo &info, short *data);
int saveRawImage(const string &filename, 
		 const ExternalImageInfo &info, short *data);
int closeWin(SOCKET sock);
void usage(string name);
Params parseArgs(int argc, char** argv);

///// functions

// controller
int main(int argc, char** argv) {

  Params p = parseArgs(argc,argv);

  cout << "trying to start scanner listener on port " << p.port << endl;
  runReceiverWin(p);
  cout << "done" << endl;

  return 0;
}


// listens for incomming images on a port until error
// out: 0 for sucess, 1 for error
int runReceiverWin(const Params &p) {
  // start winsock
  WSAData dat;
  if(0 != WSAStartup(MAKEWORD(1,1),&dat)) {
    cout << "error starting winsock" << endl;
  }

  cout << "starting the receiver...";
  SOCKET sock = getListenerWin(htons(p.port));
  if(sock == INVALID_SOCKET) {
    cout << "failed" << endl;
    return 1;
  }
  cout << "success" << endl;

  for(unsigned int imageNum = 0; true; imageNum++) {
    cout << "listening..." << endl << flush;
    sockaddr_in remote;
    SOCKET sd = acceptWin(sock, remote);
    if(sd == INVALID_SOCKET) {
      cout << "connection error" << endl;
      return 1;
    }

    // receive and build header
    ExternalImageInfo *info = readImageInfoWin(sd);

    // receive and build data
    short *data = readImageDataWin(sd,*info);

    // error check
    if(info == NULL || data == NULL) {
      cout << "error receiving image, exit" << endl;
      break;
    }

    // save
    saveImage(p, *info, data);

    // close connection
    closeWin(sd);
  }

  return 0;
}

// get a new socket on a local port to listen on
// out: SOCKET on sucess, INVALID_SOCKET on error
SOCKET getListenerWin(int port) {
  u_long addr = inet_addr("0.0.0.0");
  SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd != INVALID_SOCKET) {
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = addr;
    sin.sin_port = port;
    if(bind(sd, (sockaddr*)&sin, sizeof(sockaddr_in)) != SOCKET_ERROR) {
      listen(sd, 1);
      return sd;
    }
  }

  return INVALID_SOCKET;
}

// accept an incoming connection
// out: return value of accept()
SOCKET acceptWin(SOCKET sock, sockaddr_in& remote) {
  int nAddrSize = sizeof(remote);
  return accept(sock, (sockaddr*)&remote, &nAddrSize);
}

// read an image header from the socket
//  in
//   sock: a socket to receive on
//  out
//   image info struct on successful read (NULL otherwise)
ExternalImageInfo *readImageInfoWin(SOCKET sock) {
  char buffer[EXTERNALSENDERSIZEOF];
  int rec;

  // read header
  for(rec = 0; rec < EXTERNALSENDERSIZEOF && rec != SOCKET_ERROR;
      rec += recv(sock, buffer+rec, EXTERNALSENDERSIZEOF-rec, 0));

  // error check
  if(rec != EXTERNALSENDERSIZEOF) {
    //cout << "error receiving header" << endl;
    return NULL;
  }

  cout << "received header" << endl;
  ExternalImageInfo *info = new ExternalImageInfo(buffer, rec);

  return info;
}

// read an image info from a socket
// NOTE: performes blocking read
//  in
//   stream: a socket stream to receive on
//   info:   the last read image info struct
//  out
//   image data on successful read (NULL otherwise)
short *readImageDataWin(SOCKET sock, const ExternalImageInfo &info) {
  //cout << "receiving image " << info.iAcquisitionNumber << endl;

  int numPix
    = (int) pow((double)info.iMosaicGridSize,2)*info.nLin*info.nCol;
  char *buffer = new char[numPix*sizeof(short)]();

  // read image data
  int rec;
  for(rec = 0; rec < numPix*sizeof(short) && rec != SOCKET_ERROR;) {
    int recNow = recv(sock, buffer+rec, numPix*sizeof(short)-rec,0);
    if(recNow == SOCKET_ERROR) {
      rec = SOCKET_ERROR;
    }
    else {
      rec += recNow;
    } 
  }

  // error check
  if(rec != numPix*sizeof(short)) {
    cout << "error receiving image data" << endl;
    return NULL;
  }

  short *img = new short[numPix];
  memcpy(img,buffer,numPix*sizeof(short));

  delete [] buffer;

  return img;
}

// save an image
int saveImage(const Params &p, const ExternalImageInfo &info, short *data) {
  static int runNum = 0;
  string filename;
  int imgNum = info.iAcquisitionNumber;

  // if this is first image, find the run number of already existing
  if(imgNum == 1 && !info.bIsMoCo) {
    ifstream intest;
    do {
      runNum++;
      filename = getImageFilename(p,runNum,imgNum,info);
      intest.open(filename.c_str());
    }  while(!intest.fail());
  }

  // save the image based on type
  switch(p.imagetype) {
    case NII:
      cerr << "nifti not yet supported, sorry" << endl;
      return 1;
      break;
    case RAW:
      filename = getImageFilename(p,runNum,imgNum,info);
      saveRawImage(filename, info, data);
      break;
    default:
      cerr << "unknown image filetype. :(" << endl;
      break;
  }

  return 0;
}

string getImageFilename(const Params &p, int runNum, int imgNum, const ExternalImageInfo &info) {
  stringstream name;

  name << p.dir << "/" << p.filestem;

  if(info.bIsMoCo) {
    name << "_moco";
  }

  name << "-" << runNum << "-" << imgNum;

  switch(p.imagetype) {
    case NII:
      name << ".nii";
      break;
    case RAW:
      name << ".raw";
      break;
    default:
      break;
  }  

  return name.str();
}

// save an image in raw format
// binary file format:
// number of image dimensions (unsigned int 32)
// size of each dimension (unsigned int 32)
// data (short 16)
int saveRawImage(const string &filename, 
		 const ExternalImageInfo &info, short *data) {

  ofstream os(filename.c_str(), ios::out | ios::binary);

  if(os.fail()) {
    cerr << "could not open " << filename << " for writing an image" << endl;
    os.close();
    return 1;
  }

  if(os.fail()) return false;

  // write the header
  unsigned int num;

  /// note assumption of mosaiced image here...
  // n dims
  num = 2;
  os.write((char*) &num, sizeof(unsigned int));

  // dim sizes
  num = info.nLin * info.iMosaicGridSize;
  os.write((char*) &num, sizeof(unsigned int));

  num = info.nCol * info.iMosaicGridSize;
  os.write((char*) &num, sizeof(unsigned int));

  // write the data
  os.write((char*) data, info.lImageDataLength);
  if(!os.good()) {
    cerr << "error writing file " << filename << endl;
    return 1;
  }

  os.close();

  cout << "saved raw image " << filename << endl;

  return 0;
}

// write the image to a nifti file
int saveNiftiImage(const string &filename,
		   const ExternalImageInfo &info, short *data) {

//  // fill the nifti struct with our data
//  nifti_image *img = (nifti_image*) malloc(sizeof(nifti_image));
//
//  lock(); {
//
//    copyInfo(img);
//
//    // allocate and copy data
//    img->data = (void*) malloc(img->nbyper*img->nvox);
//    memcpy(img->data, data, img->nbyper*img->nvox);
//
//    // debugging
//    //nifti_image_infodump(img);
//
//    // write the file
//    nifti_image_write(img);
//  } unlock();
//
//  nifti_image_free(img);
//
  return 0;
}

// gracefully close a socket
int closeWin(SOCKET sock) {
  if(shutdown(sock, 1) == SOCKET_ERROR) {
    return 1;
  }

  // clear remaining data
  char buffer[1024];
  int num = 1;
  while(0 != (num = recv(sock, buffer, 1024, 0)) && num != SOCKET_ERROR);

  if(num == SOCKET_ERROR) {
    return 1;
  }

  if(closesocket(sock) == SOCKET_ERROR) {
    return 1;
  }

  return 0;
}

// usage
void usage(string name) {
  cout << "usage: " << name << " [options]" << endl
       << "  where options are:" << endl
       << "    -i imagetype: types can be 'raw' or 'nii'" << endl
       << "    -d dir:       directory to save into (default is '.')" << endl
       << "    -f filestem:  begining of filenames (default 'img')" << endl
       << "    -p port:      port to listen on (default 15000)" << endl;
  return;
}

// argument parsing
Params parseArgs(int argc, char **argv) {
  // special check for help
  if(argc == 2 && argv[1][1] == 'h') {
    usage(argv[0]);
    exit(0);
  }

  Params p;

  for(int arg = 1; arg < argc-1; arg+=2) {
    cout << argv[arg] << " " << argv[arg+1] << endl;

    // error check
    if(strlen(argv[arg]) != 2 || argv[arg][0] != '-') {
      cerr << "error parsing command line. missing option at arg "
	   << arg << endl;
      exit(0);
    }

    char opt = argv[arg][1];
    char *optval = argv[arg+1];

    // find appropriate option
    switch(opt) {

     case 'i':
       if(!strcmp(optval,"raw")) {
	 p.imagetype = RAW;
       }
       else if(!strcmp(optval,"nii")) {
	 p.imagetype = NII;
       }
       else {
	 cout << "unknown image file type " << optval << ", using default"
	      << endl;
       }
       break;

     case 'd':
       p.dir = optval;
       break;

     case 'f':
       p.filestem = optval;
       break;

     case 'p':
       p.port = atoi(optval);
       break;

     default:
       cerr << "ignoring unknown option -" << opt << endl;
       break;

    }
  }

  return p;
}
