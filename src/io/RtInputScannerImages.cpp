/******************************************************************************
 * RtInputScannerImages.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/
static char *VERSION = "$Id$";

#include"RtInputScannerImages.h"
  
static const int             DEFAULT_PORT = 3999;

// default constructor
RtInputScannerImages::RtInputScannerImages() 
                 : host(NULL), port(DEFAULT_PORT) {
  
}

// destructor
RtInputScannerImages::~RtInputScannerImages() {
  delete host;
}

// configure and open
bool RtInputScannerImages::open(RtConfig &config) {
  RtInput::open(config);

  // validate
  if(!validateHostAndPort(config)) {
    cerr << "ERROR: host " << config.get("scannerHost")
	 << ":" << config.get("scannerPort") << " is invalid." << endl;
    isOpen = false;
    return false;
  }
    
  try { 
    // get the host and port from the config
    // try to open the socket
    port = config.get("scannerPort");
    
    host = new InetHostAddress((char*) config.get("scannerHost"));

    socketService = new RtSocketServiceServerScannerImage(*host,port);
    socketService->startServer();
  }
  catch(...) {
    cerr << "ERROR: could not open connection to receive "
	 << "images from the scanner." << endl 
	 << "using host " << host << " and port " << port << endl;
    isOpen = false;
    return false;
  }

  return true;
}

// close and clean up
bool RtInputScannerImages::close() {
  
  if(isOpen) {
    socketService->stopServer();
  }

  RtInput::close();  

  return true;
}

// validate the passed host and port
bool RtInputScannerImages::validateHostAndPort(RtConfig &config) {

  // host
  if(inet_addr((char*) config.get("scannerHost")) < 1) {
    return false;
  }

  // port
  if(port < 0 || port > 65535) {
    return false;
  }

  return true;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtInputScannerImages::getVersionString() {
  return VERSION;
}


//  //*** private data members  ***//
//
//  string tcpPort;
//  bool   saveImagesToFile;
//  string saveDirectory;
//  string saveFilestem;

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


