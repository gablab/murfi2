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
static const char*  DEFAULT_SAVEDIR  = "/tmp";
static const char*  DEFAULT_SAVESTEM = "img";
static const char*  DEFAULT_SAVEEXT  = "dat";

// increase this size for highres acquisitions
#define MAXBUFSIZ 256*256*256*2 
static char buffer[MAXBUFSIZ];

// default constructor
RtInputScannerImages::RtInputScannerImages()
  :  port(DEFAULT_PORT), 
     saveDir(DEFAULT_SAVEDIR),
     saveFilestem(DEFAULT_SAVESTEM),
     saveFileext(DEFAULT_SAVEEXT),
     toBeDeleted(received.begin()) {
  // nothing to do
}

// destructor
RtInputScannerImages::~RtInputScannerImages() {

  // delete all remaining received images
  for(; toBeDeleted != received.end(); toBeDeleted++) {
    delete *toBeDeleted;
  }

}

// configure and open
bool RtInputScannerImages::open(RtConfig &config) {
  RtInput::open(config);

  // get port from the config and try to open the socket
  port = config.get("scannerPort")==true 
    ? config.get("scannerPort") : DEFAULT_PORT;

  // build the address  
  ACE_INET_Addr address(port,INADDR_ANY);
  if(acceptor.open(address,1) == -1) {
    cout << "failed to open" << endl;
    isOpen = false;
    return false;
  }

  isOpen = true;

  // see if we should save images to a file
  if(config.get("saveImages")==true) {
    saveImagesToFile = true;
    
    if(config.get("imageDir")==true) {
      saveDir = (char*) config.get("imageDir");
    }

    if(config.get("imageStem")==true) {
      saveFilestem = (char*) config.get("imageStem");
    }

    if(config.get("imageExt")==true) {
      saveFileext = (char*) config.get("imageExt");
    }
  }

  return true;
}


// set the handler that should receive new data
//  in
//   handler: handler to send the image to after we receive it
//bool RtInputScannerImages::setHandler(ACE_Handler &handler) {
//
//  if(reader.open(handler) == -1) {
//    cerr << "ERROR: could not register a handler for scanner images" 
//	 << endl;
//    return readerOpen = false;
//  }
//
//  return readerOpen = true;
//}


// close and clean up
//  out
//   success flag
bool RtInputScannerImages::close() {

  if(isOpen) {
    // other stuff
  }

  RtInput::close();

  return true;
}

// run the scanner input
int RtInputScannerImages::svc() {
  RtExternalImageInfo *ei;
  unsigned short *img;
  RtDataImage *rti;

  int imageNum = 0;

  // continuously try to accept connections
  for(; isOpen && acceptor.accept(stream) != -1; imageNum++) {

    // get the info
    ei = receiveImageInfo(stream);
    if(ei == NULL) {
      continue;
    }
      
    // DEBUGGING
    ei->iAcquisitionNumber = imageNum++;

    // get the image
    img = receiveImage(stream, *ei);

    // build data class
    rti = new RtDataImage(*ei,img);

    // append this to a vector of gathered images
    received.push_back(rti);

    // signal that we got an image
    cerr << "sending event with code number " << codeNum << endl;

    sendCode(rti);

//    // print and save
//    rti->printInfo(cout);
//
//    if(saveImagesToFile) {
//      saveImage(*rti);
//    }
//
//    // signal
//    if(readerOpen) {
//      sendImageToReader(*rti);
//    }

    // clean up
    delete ei;
    delete [] img;

      
    //delete rti;

    // close the stream (scanner connects anew for each image)
    stream.close();
  }
  cerr << "ERROR: could not open connection to accept "
       << "images from the scanner on port " 
       << port << endl;
    //<< "errno: " << ACE_OS::last_error();
  isOpen = false;


  return 0;
}


// deleted some received images from the heap
//  in
//   deleteNum: maximum number of images to delete
void RtInputScannerImages::deleteReceivedImages(int deleteNum) {
  
  for(int i = 0; i < deleteNum && toBeDeleted != received.end(); 
      i++, toBeDeleted++) {
    delete *toBeDeleted;
  }
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
  for(rec = 0; rec < EXTERNALSENDERSIZEOF;
    rec += stream.recv_n (buffer+rec, EXTERNALSENDERSIZEOF-rec));

  // arbitrary limit
  if(rec < 100) {
    return NULL;
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

//// sends an image to a event handler
////  in
////   img: image to send
//bool RtInputScannerImages::sendImageToReader(RtDataImage &img) {
//
//  // make a message block
//  ACE_Message_Block mb(sizeof(RtDataImageInfo)+img.getImgDataLen());
//  memcpy(mb.wr_ptr(),&img.getInfo(),sizeof(RtDataImageInfo));
//  memcpy(mb.wr_ptr()+sizeof(RtDataImageInfo),img.getData(),img.getImgDataLen());
//  if(reader.read(mb, mb.space()) != 0) {
//    cerr << "ERROR: handler failed to read scanner images" 
//	 << endl;
//    return readerOpen = false;
//  }
//
//  return true;
//}


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


