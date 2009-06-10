/******************************************************************************
 * program to save images sent from a scanner using the external
 * sender ice functor
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-05-31
 *****************************************************************************/

#include"ExternalImageInfo.h"

// platform specific includes and defines
#ifdef WIN32
  #include <winsock.h>
  #define CLOSE closesocket
  typedef int socklen_t; 
#else
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <netinet/in.h>
  #define SOCKET int
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1
  #define CLOSE close
#endif

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <csignal>
using namespace std;

///// defaults
static const int defaultPort = 15000;
static const string defaultDir(".");
static const string defaultFilestem("img");

///// types
struct Params {
 public:
  unsigned int port;
  string dir;
  string filestem;
  bool onlyReadMoco;
  bool quiet;

  Params() :
    port(defaultPort),
    dir(defaultDir),
    filestem(defaultFilestem),
    onlyReadMoco(false),
    quiet(false)
  {};
};

///// globals
static SOCKET sock = INVALID_SOCKET;

///// prototypes
int runReceiver(const Params &p);
SOCKET getListener(int port);
SOCKET acceptConnection(SOCKET sock, sockaddr_in& remote);
string getImageFilename(const Params &p, int runNum, int imgNum, const ExternalImageInfo &info);
bool fileExist(const string &filename);
ExternalImageInfo *readImageInfo(SOCKET sock);
short *readImageData(SOCKET sock, const ExternalImageInfo &info);
int saveImage(const Params &p, const ExternalImageInfo &info, short *data);
int saveRawImage(const string &filename, 
		 const ExternalImageInfo &info, short *data);
int closeSocket(SOCKET sock);
void onTerminate(int param);
void usage(string name);
Params parseArgs(int argc, char** argv);


///// functions

// controller
int main(int argc, char** argv) {

  Params p = parseArgs(argc,argv);

  // setup termination handler for clean exit
  signal(SIGTERM,onTerminate);
  signal(SIGSEGV,onTerminate);
  signal(SIGABRT,onTerminate);
  signal(SIGFPE,onTerminate);
  signal(SIGILL,onTerminate);

  if(!p.quiet) {
    cout << "trying to start scanner listener on port " << p.port << endl;
  }

  // run the receiver
  runReceiver(p);


  if(!p.quiet) {
    cout << "done" << endl;
  }

  return 0;
}


// listens for incomming images on a port until error
// out: 0 for sucess, 1 for error
int runReceiver(const Params &p) {
  #ifdef WIN32
    // start winsock
    WSAData dat;
    if(0 != WSAStartup(MAKEWORD(1,1),&dat)) {
      cerr << "error starting winsock" << endl;
      return 1;
    }
  #endif

  if(!p.quiet) {
    cout << "starting the receiver...";
  }
  sock = getListener(htons(p.port));
  if(sock == INVALID_SOCKET) {
    cerr << "failed" << endl;
    return 1;
  }
  if(!p.quiet) {
    cout << "success" << endl;
  }

  for(unsigned int imageNum = 0; true; imageNum++) {
    if(!p.quiet) {
      cout << "listening..." << endl << flush;
    }
    sockaddr_in remote;
    SOCKET sd = acceptConnection(sock, remote);
    if(sd == INVALID_SOCKET) {
      cerr << "connection error" << endl;
      return 1;
    }

    // receive and build header
    ExternalImageInfo *info = readImageInfo(sd);

    // error check
    if(info == NULL) {
      cerr << "error receiving image header, exit" << endl;
      return 1;
    }

    // receive and build data
    short *data = readImageData(sd,*info);

    // error check
    if(data == NULL) {
      cerr << "error receiving image, exit" << endl;
      return 1;
    }

    // save
    if(!p.onlyReadMoco || info->bIsMoCo) {
      saveImage(p, *info, data);
    }

    // close connection
    closeSocket(sd);
  }

  return 0;
}

// get a new socket on a local port to listen on
// out: SOCKET on sucess, INVALID_SOCKET on error
SOCKET getListener(int port) {
  //u_long addr = inet_addr("0.0.0.0");
  SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd != INVALID_SOCKET) {
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
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
SOCKET acceptConnection(SOCKET sock, sockaddr_in& remote) {
  socklen_t nAddrSize = sizeof(remote);
  return accept(sock, (sockaddr*)&remote, &nAddrSize);
}

// read an image header from the socket
//  in
//   sock: a socket to receive on
//  out
//   image info struct on successful read (NULL otherwise)
ExternalImageInfo *readImageInfo(SOCKET sock) {
  char buffer[EXTERNALSENDERSIZEOF];
  int rec;

  // read header
  for(rec = 0; rec < EXTERNALSENDERSIZEOF && rec != SOCKET_ERROR;
      rec += recv(sock, buffer+rec, EXTERNALSENDERSIZEOF-rec, 0));

  // error check
  if(rec != EXTERNALSENDERSIZEOF) {
    return NULL;
  }

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
short *readImageData(SOCKET sock, const ExternalImageInfo &info) {
  int numPix
    = (int) pow((double)info.iMosaicGridSize,2)*info.nLin*info.nCol;
  char *buffer = new char[numPix*sizeof(short)]();

  // read image data
  int rec;
  for(rec = 0; rec < numPix*(int)sizeof(short) && rec != SOCKET_ERROR;) {
    int recNow = recv(sock, buffer+rec, numPix*sizeof(short)-rec,0);
    if(recNow == SOCKET_ERROR) {
      rec = SOCKET_ERROR;
    }
    else {
      rec += recNow;
    } 
  }

  // error check
  if(rec != numPix*(int)sizeof(short)) {
    cerr << "error receiving image data" << endl;
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
  bool found = true;
  if(imgNum == 1 && (p.onlyReadMoco || !info.bIsMoCo)) {
    do {
      runNum++;
      filename = getImageFilename(p,runNum,imgNum,info);
      found = fileExist(filename);
    } while(found);
  }

  // save the image
  filename = getImageFilename(p,runNum,imgNum,info);

  if(!saveRawImage(filename, info, data) && !p.quiet) {
    cout << "saved raw image " << filename << " of " 
	 << info.iImageDataLength << " bytes" << endl;
  }

  return 0;
}

string getImageFilename(const Params &p, int runNum, int imgNum, const ExternalImageInfo &info) {
  stringstream name;

  if(p.dir == "." || p.dir == "./") {
    name << p.filestem;
  }
  else {
    name << p.dir << "/" << p.filestem;
  }

  if(info.bIsMoCo) {
    name << "_moco";
  }

  name << "-" 
       << setw(5) << setfill('0') << runNum 
       << "-" 
       << setw(5) << setfill('0') << imgNum 
       << ".raw";

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
  os.write((char*) data, info.iImageDataLength);
  if(!os.good()) {
    cerr << "error writing file " << filename << endl;
    return 1;
  }

  os.close();

  return 0;
}


// gracefully close a socket
int closeSocket(SOCKET sock) {
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

  if(CLOSE(sock) == SOCKET_ERROR) {
    return 1;
  }

  return 0;
}

// usage
void usage(string name) {
  cout << "usage: " << name << " [options]" << endl
       << "  where options are:" << endl
       << "    -d dir:       directory to save into (default is '.')" << endl
       << "    -f filestem:  beginning of filenames (default 'img')" << endl
       << "    -p port:      port to listen on (default 15000)" << endl
       << "    -m flag:      1 or 0 whether to ignore non-moco images (default 0)" << endl
       << "    -q flag:      1 or 0 whether to suppress output (default 0)" << endl;
  return;
}

// argument parsing
Params parseArgs(int argc, char **argv) {
  // special check for help
  if(argc == 2 && (argv[1][1] == 'h' || argv[1][1] == '?')) {
    usage(argv[0]);
    exit(0);
  }

  Params p;

  for(int arg = 1; arg < argc-1; arg+=2) {
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

     case 'd':
       p.dir = optval;
       break;

     case 'f':
       p.filestem = optval;
       break;

     case 'p':
       p.port = atoi(optval);
       break;

     case 'm':
       p.onlyReadMoco = bool(atoi(optval));
       break;

     case 'q':
       p.quiet = bool(atoi(optval));
       break;

     default:
       cerr << "ignoring unknown option -" << opt << endl;
       break;

    }
  }

  return p;
}

// cleanly shutdown the socket
void onTerminate(int param) {
  closeSocket(sock);
  exit(0);
}

// check if file exists on either windoze or posix
bool fileExist(const string &filename) {
  #ifdef WIN32 
  {
    DWORD fileattr;
    
    fileattr = GetFileAttributes(filename.c_str());
    if(fileattr == 0xFFFFFFFF) {
      return false;
    }
    return true;
  } 
  #else 
  {
    ifstream intest(filename.c_str());
    return !intest.fail();
  }
  #endif
}
