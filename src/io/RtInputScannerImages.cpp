/******************************************************************************
 * RtInputScannerImages.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/
static char *VERSION = "$Id$";

#include"RtInputScannerImages.h"

#include<fstream>
using namespace std;

// defaults
static const int    DEFAULT_PORT = 15000;
static const char*  DEFAULT_SAVEDIR  = "img";
static const char*  DEFAULT_SAVESTEM = "img";
static const char*  DEFAULT_SAVEEXT  = "dat";

// increase this size for highres acquisitions
#define MAX_BUFSIZ 256*256*256*2
#define MAX_SERIESNUM 99999
static char buffer[MAX_BUFSIZ];

// default constructor
RtInputScannerImages::RtInputScannerImages()
  :  port(DEFAULT_PORT),
     savePath(""),
     saveDir(DEFAULT_SAVEDIR),
     saveFilestem(DEFAULT_SAVESTEM),
     saveFileext(DEFAULT_SAVEEXT),
     seriesNum(1),
     toBeDeleted(received.begin())
{
  id += ":scanner:images";
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
  port = config.isSet("scanner:port")
    ? config.get("scanner:port") : DEFAULT_PORT;

  // build the address
  ACE_INET_Addr address(port,INADDR_ANY);
  if(acceptor.open(address,1) == -1) {
    ACE_DEBUG((LM_INFO, ACE_TEXT("failed to open acceptor for scanner images\n")));
    isOpen = false;
    return false;
  }

  isOpen = true;

  // see if we should save images to a file
  if(config.get("scanner:saveImages")==true) {
    saveImagesToFile = true;

    // set up the directory to save to
    if(config.isSet("scanner:imageDir")) {
      saveDir = config.get("scanner:imageDir").str();
    }

    if(config.isSet("scanner:imageStem")) {
      saveFilestem = config.get("scanner:imageStem").str();
    }

    if(config.isSet("scanner:imageExt")) {
      saveFileext = config.get("scanner:imageExt").str();
    }

    // set the save path
    //string sp(config.get("studyDir").str());
    stringstream sp;
    //sp << "/home/mri/subjects/foobar/" << saveDir << "/" << saveFilestem;
    sp << config.get("study:dir").str() << saveDir << "/" << saveFilestem;
    savePath = sp.str();
  }

  return true;
}

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
  RtMRIImage *rti;
  stringstream infos;

  int imageNum = 0;

  // continuously try to accept connections
  for(; isOpen && acceptor.accept(stream) != -1; imageNum++) {
    cout << "connection accepted" << endl;

    // get the info
    ei = receiveImageInfo(stream);
    if(ei == NULL) {
      continue;
    }

    // DEBUGGING
    //ei->iAcquisitionNumber = imageNum++;

    // get the image
    img = receiveImage(stream, *ei);

    // build data class
    rti = new RtMRIImage(*ei,img);
    rti->setSeriesNum(seriesNum);

    // set the image id for handling
    //rti->addToID("scanner");

    // append this to a vector of gathered images
    received.push_back(rti);

    // signal that we got an image
    cerr << "sending event with code number " << codeNum << endl;

    sendCode(rti);

    rti->printInfo(cout);

    if(saveImagesToFile) {
      saveImage(*rti);
    }

    // log that we received the image
    infos.str("");
    infos << "received image from scanner: series " << seriesNum
	  << " acquisition " << ei->iAcquisitionNumber << endl;
    log(infos);

    // clean up
    delete ei;
    delete [] img;


    //delete rti;

    // close the stream (scanner connects anew for each image)
    stream.close();

    cout << "waiting for another image" << endl;
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

  static int acnum = 1;
  acnum++;

  // read until we have all the bytes we need
  // ADD ERROR HANDLING HERE!!!
  for(rec = 0; rec < EXTERNALSENDERSIZEOF;
    rec += stream.recv_n (buffer+rec, EXTERNALSENDERSIZEOF-rec));

  // arbitrary limit
  if(rec < 100) {
    return NULL;
  }

  ACE_DEBUG((LM_TRACE, ACE_TEXT("received header of size %d\n"), rec));

  RtExternalImageInfo *info = new RtExternalImageInfo(buffer, rec);

  //info->iAcquisitionNumber = acnum++;

  // if this is the first acquisition, get the series number
  if(isFirstInSeries(*info)) {
    seriesNum = getNextSeriesNum();
  }

  ACE_DEBUG((LM_DEBUG, "received info for %d:%d\n", seriesNum, info->iAcquisitionNumber));

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

  ACE_DEBUG((LM_DEBUG, "receiving data for %d:%d\n", seriesNum, info.iAcquisitionNumber));

  int numPix = info.nLin * info.nCol;
  for(unsigned int rec = 0; rec < numPix*sizeof(unsigned short);
      rec += stream.recv_n (buffer+rec, numPix*sizeof(unsigned short)-rec)) {
  }

  unsigned short *img = new unsigned short[numPix];
  memcpy(img,buffer,numPix*sizeof(unsigned short));

  return img;
};

// saves an image
//  in
//   img: image to save
bool RtInputScannerImages::saveImage(RtMRIImage &img) {

  cerr << "writing image number " << seriesNum << ":" << img.getAcquisitionNum() << endl;

  return img.write(getImageFilename(seriesNum, img.getAcquisitionNum()));
}

// build a filename for a given acquisition number for the current series
// number
//  in
//   acquisition number
//  out
//   absolute file string
string RtInputScannerImages::getImageFilename(int _seriesNum,
					      int _acquisitionNum) {
  // five digit filename assumption here!
  char srnum[6];
  char acnum[6];
  sprintf(srnum,"%05d",_seriesNum);
  sprintf(acnum,"%05d",_acquisitionNum);

  stringstream s;
  s << savePath << "-" << srnum << "-" << acnum << "." << saveFileext;
  string ret = s.str();

  return s.str();
}

// determines if the received image is the first image in a series or not
// examines the acquisition number for 1
//  in
//   info struct for the image to test
//  out
//   true if this image is the first in a series
bool RtInputScannerImages::isFirstInSeries(const RtExternalImageInfo &info) {
  return info.iAcquisitionNumber == 1;
}

// gets the next series number to be saved in the current image directory
// inspects the series currently in the directory and makes a new one
unsigned int RtInputScannerImages::getNextSeriesNum() {
  fstream fin;
  string fname;

  for(unsigned int sn = seriesNum; sn < MAX_SERIESNUM; sn++) {
    fname = getImageFilename(sn, 1);
    fin.open(fname.c_str());

    if(fin.fail() ) {
      fin.close();
      return sn;
    }
    fin.close();
  }

  return MAX_SERIESNUM;
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


