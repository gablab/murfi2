/******************************************************************************
 * RtInputScannerImages.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/
static char *VERSION = "$Id$";

#include"RtInputScannerImages.h"
#include<vnl/vnl_matrix_fixed.h>
#include<vnl/vnl_vector.h>
#include<fstream>
using namespace std;

// defaults
static const int    DEFAULT_PORT = 15000;
static const char*  DEFAULT_SAVEDIR  = "img";
static const char*  DEFAULT_SAVESTEM = "img";
static const char*  DEFAULT_SAVEEXT  = "nii";

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
  addToID(":scanner:images");
  saveImagesToFile = false;
  unmosaicInputImages = true;
  onlyReadMoCo = false;
  matrixSize = 64;
  numSlices = 32;
  sliceGap = 0.1;
  voxDim[0] = voxDim[1] = voxDim[2] = 1.0;
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
  ACE_INET_Addr address(port,(ACE_UINT32)INADDR_ANY);
  if(acceptor.open(address,1) == -1) {
    ACE_DEBUG((LM_INFO, ACE_TEXT("failed to open acceptor for scanner images\n")));
    isOpen = false;
    return false;
  }

  isOpen = true;


  // set image params
  if(config.isSet("scanner:matrixSize")) {
    matrixSize = config.get("scanner:matrixSize");
  }
  if(config.isSet("scanner:slices")) {
    numSlices = config.get("scanner:slices");
  }
  if(config.isSet("scanner:sliceGap")) {
    sliceGap = config.get("scanner:sliceGap");
  }
  if(config.isSet("scanner:voxdim1")) {
    voxDim[0] = config.get("scanner:voxdim1");
  }
  if(config.isSet("scanner:voxdim2")) {
    voxDim[1] = config.get("scanner:voxdim2");
  }
  if(config.isSet("scanner:voxdim3")) {
    voxDim[2] = config.get("scanner:voxdim3");
  }

  // see if we should only read moco images
  if(config.isSet("scanner:onlyReadMoCo") 
     && config.get("scanner:onlyReadMoCo")==true) {
    onlyReadMoCo = true;
  }
  else {
    onlyReadMoCo = false;
  }

 // see if we should unmosaic the images
  if(config.isSet("scanner:unmosaic")) {
      unmosaicInputImages = (bool) config.get("scanner:unmosaic");
  }
 
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
  short *img;
  RtMRIImage *rti;
  stringstream infos;

  int imageNum = 0;

  // continuously try to accept connections
  for(; isOpen && acceptor.accept(stream) != -1; imageNum++) {
    //cout << "connection accepted" << endl;

    // get the info
    ei = receiveImageInfo(stream);
    if(ei == NULL) {
      continue;
    }


    // DEBUGGING
    //ei->displayImageInfo();
    //ei->iAcquisitionNumber = imageNum++;

    // get the image
    img = receiveImage(stream, *ei);

    if(onlyReadMoCo && !ei->bIsMoCo) {
      //cout << "ignoring non-MoCo image." << endl;
      continue;
    }

    // build data class
    rti = new RtMRIImage(*ei,img);
    rti->setSeriesNum(seriesNum);
    rti->setMatrixSize(matrixSize);
    rti->setNumSlices(numSlices);


//    rti->setPixDim(0,voxDim[0]);
//    rti->setPixDim(1,voxDim[1]);
//    rti->setPixDim(2,voxDim[2]);

    if(unmosaicInputImages) {
      rti->unmosaic();
    }

    // set the image id for handling
    //rti->addToID("scanner");

    // append this to a vector of gathered images
    received.push_back(rti);

    // signal that we got an image
    //cout << "sending event with code number " << codeNum << endl;

    sendCode(rti);

    //rti->printInfo(cout);

    if(saveImagesToFile) {
      saveImage(*rti);
    }

    // log that we received the image
    infos.str("");
    infos << "received image from scanner: series " << seriesNum
	  << " acquisition " << ei->iAcquisitionNumber << endl;
    log(infos);
    
    cout << "started processing image at ";
    printNow(cout);
    cout << endl;

    // clean up
    delete ei;
    delete [] img;


    //delete rti;

    // close the stream (scanner connects anew for each image)
    stream.close();

    //cout << "waiting for another image" << endl;
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
  //rec = stream.recv_n (buffer, EXTERNALSENDERSIZEOF);

  // arbitrary limit
  if(rec < 100) {
    return NULL;
  }

  ACE_DEBUG((LM_TRACE, ACE_TEXT("received header of size %d\n"), rec));
  //cout << "received header of size " << rec << endl;

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
short *RtInputScannerImages::receiveImage(ACE_SOCK_Stream &stream,
						   const RtExternalImageInfo &info) {

  ACE_DEBUG((LM_DEBUG, "receiving data for %d:%d\n", seriesNum, info.iAcquisitionNumber));
  cout << "receiving image " << info.iAcquisitionNumber << endl;

  int numPix = (int) pow((double)info.iMosaicGridSize,2) * info.nLin * info.nCol;
  for(unsigned int rec = 0; rec < numPix*sizeof(short);
      rec += stream.recv_n (buffer+rec, numPix*sizeof(short)-rec)) {
  }

  short *img = new short[numPix];
  memcpy(img,buffer,numPix*sizeof(short));

  return img;
};

// saves an image
//  in
//   img: image to save
bool RtInputScannerImages::saveImage(RtMRIImage &img) {

  cout << "writing image number " << seriesNum << ":" << img.getAcquisitionNum() << endl;

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


