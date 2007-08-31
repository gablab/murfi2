/******************************************************************************
 * RtInputScannerImages.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/
static char *VERSION = "$Id$";

#include"RtInputScannerImages.h"
#include"RtDataImage.h"

// defaults
static const int    DEFAULT_PORT = 15000;
//static const char*  DEFAULT_HOST = "localhost";
static const char*  DEFAULT_SAVEDIR  = "/tmp";
static const char*  DEFAULT_SAVESTEM = "img";
static const char*  DEFAULT_SAVEEXT  = "dat";

#define MAXBUFSIZ 256*256*256*2
static char buffer[MAXBUFSIZ];

// default constructor
RtInputScannerImages::RtInputScannerImages()
                 :  port(DEFAULT_PORT), readerOpen(false) {

}

// destructor
RtInputScannerImages::~RtInputScannerImages() {
}

// configure and open
bool RtInputScannerImages::open(RtConfig &config) {
  RtInput::open(config);

  // get port from the config and try to open the socket
  port = config.get("scannerPort")==true 
    ? config.get("scannerPort") : DEFAULT_PORT;

  // build the address
  address.set(port,INADDR_ANY);
  if(acceptor.open(address) == -1) {
    cout << "failed to open" << endl;
    isOpen = false;
    return false;
  }

  isOpen = true;

  // see if we should save images to a file
  if(config.get("saveImages")==true) {
    saveImagesToFile = true;
    
    saveDir = config.get("imageDir")==true 
      ? (char*) config.get("imageDir") :  DEFAULT_SAVEDIR;
    saveFilestem = config.get("imageStem")==true 
      ? (char*) config.get("imageStem") : DEFAULT_SAVESTEM;
    saveFileext = config.get("imageExt")==true 
      ? (char*) config.get("imageExt") : DEFAULT_SAVEEXT;
  }

  return true;
}


// set the handler that should receive new data
//  in
//   handler: handler to send the image to after we receive it
bool RtInputScannerImages::setHandler(ACE_Handler &handler) {

  if(reader.open(handler) == -1) {
    cerr << "ERROR: could not register handler to accept scanner images" 
	 << endl;
    return readerOpen = false;
  }

  return readerOpen = true;
}


// close and clean up
bool RtInputScannerImages::close() {

  if(isOpen) {
    // other stuff
  }

  RtInput::close();

  return true;
}

// run the scanner input
// PLACEHOLDER FOR A BETTER THING TO BE IMPLEMENTED
int RtInputScannerImages::svc() {
  RtExternalImageInfo *ei;
  unsigned short *img;
  RtDataImage *rti;

  int imageNum = 0;

  // continuously try to accept connections
  while(isOpen && acceptor.accept(stream) != -1) {

    // read forever while we are open
    for(; acceptor.isOpen && isOpen; imageNum++) {

      // get the info
      ei = receiveImageInfo(stream);
      if(ei == NULL) {
	continue;
      }

      // get the image
      img = receiveImage(stream, *ei);

      // build data class
      rti = new RtDataImage(*ei,img);

      // print and save
      rti->printInfo(cout);

      if(saveImagesToFile) {
	saveImage(*rti);
      }

      // signal
      if(readerOpen) {
	sendImageToReader(*rti);
      }

      // clean up
      delete ei;
      delete [] img;
      delete rti;
    }

  }
  cerr << "ERROR: could not open connection to accept "
       << "images from the scanner on port " 
       << address.get_port_number() << endl;
  isOpen = false;


  return 0;
}


// read the scanner image info from a socket stream
// NOTE: performes blocking read
//  in
//   stream: a socket stream to receive on
//  out
//   image info struct on successful read (NULL otherwise)
RtExternalImageInfo *RtInputScannerImages::receiveImageInfo(ACE_SOCK_Stream &stream) {
  unsigned int rec;

  // read until we have all the bytes we need
  // ADD ERROR HANDLING HERE!!!
//  for(rec = 0; rec < sizeof(RtExternalImageInfo);
//    rec += stream.recv_n (buffer+rec, sizeof(RtExternalImageInfo)-rec));
  rec = stream.recv(buffer, MAXBUFSIZ);

  // arbitrary limit
  if(rec < 100) {
    return false;
  }

  cout << "received img header of size " << rec << endl;

  bool dumpHeader = true;
  if(dumpHeader) {
    FILE* hdr = fopen("/tmp/dat.hdr","wb");
    fwrite(buffer, 1, rec, hdr);
    fclose(hdr);
  }

  RtExternalImageInfo *info = new RtExternalImageInfo(buffer, rec);

  return info;
}

// read an image info from a socket stream
// NOTE: performes blocking read
//  in
//   stream: a socket stream to receive on
//   info:   the last read image info struct
//  out
//   image data on successful read (NULL otherwise)
unsigned short *RtInputScannerImages::receiveImage(ACE_SOCK_Stream &stream,
						   const RtExternalImageInfo &info) {

  int numPix = info.nLin * info.nCol;
  for(unsigned int rec = 0; rec < numPix*sizeof(unsigned short);
    rec += stream.recv_n (buffer+rec, numPix*sizeof(unsigned short)-rec));

  unsigned short *img = new unsigned short[numPix];
  memcpy(img,buffer,numPix*sizeof(unsigned short));

  return img;
};

// saves an image
//  in
//   img: image to save
bool RtInputScannerImages::saveImage(RtDataImage &img) {
  char imgNum[6] = "";
  sprintf(imgNum, "%06d", img.getAcquisitionNum());

  string fname = saveDir + "/" + saveFilestem + "_" + imgNum + "." + saveFileext;

  return img.write(fname);
}

// sends an image to a event handler
//  in
//   img: image to send
bool RtInputScannerImages::sendImageToReader(RtDataImage &img) {
  // make a message block
  ACE_Message_Block mb(sizeof(RtDataImageInfo)+img.getImgDataLen());
  memcpy(mb.wr_ptr(),&img.getInfo(),sizeof(RtDataImageInfo));
  memcpy(mb.wr_ptr()+sizeof(RtDataImageInfo),img.getData(),img.getImgDataLen());
  if(reader.read(mb, mb.space()) != 0) {
    cerr << "ERROR: handler failed to read scanner images" 
	 << endl;
    return readerOpen = false;
  }
}



// validate the passed host and port
bool RtInputScannerImages::validateHostAndPort(RtConfig &config) {

  // host
  if(inet_addr((char*) config.get("scannerHost")) < 1) {
    return false;
  }

  // port
//  if(port < 0 || port > 65535) {
//    return false;
//  }

  return true;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtInputScannerImages::getVersionString() {
  return VERSION;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


