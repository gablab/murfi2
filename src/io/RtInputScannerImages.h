/******************************************************************************
 * RtInputScannerImages.h declares a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTINPUTSCANNERIMAGES_H
#define RTINPUTSCANNERIMAGES_H

#include"RtInput.h"
#include"RtConfig.h"
#include"RtExternalSenderImageInfo.h"
#include"RtMRIImage.h"
#include"RtServerSocket.h"
#include"RtFSLInterface.h"

// controls input operations to receive scanner images
class RtInputScannerImages : public RtInput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInputScannerImages(); 

  // destructor
  virtual ~RtInputScannerImages();

  // open and start accepting input
  virtual bool open(RtConfig &config);

  // set the handler that should receive new data
  //  bool setHandler(ACE_Handler &handler);

  // close and clean up
  bool close();

  // run the scanner input
  virtual int svc(); 

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // read the scanner image info from a socket stream
  // NOTE: performes blocking read
  //  in
  //   stream: a socket stream to receive on
  //  out
  //   image info struct on successful read (NULL otherwise)
  RtExternalImageInfo *receiveImageInfo(ACE_SOCK_Stream &stream);

  // read an image info from a socket stream
  // NOTE: performes blocking read
  //  in
  //   stream: a socket stream to receive on
  //   info:   the last read image info struct
  //  out
  //   image data on successful read (NULL otherwise)
  short *receiveImage(ACE_SOCK_Stream &stream, 
			       const RtExternalImageInfo &info);

  // saves an image
  //  in
  //   img: image to save
  bool saveImage(RtMRIImage &img);

  // build a filename for a given acquisition number for the current series
  // number
  //  in
  //   acquisition number
  //  out
  //   absolute file string
  //string getImageFilename(int seriesNum, int acquisitionNum);

  // build a filename for a transform file operating on a given image
  // number
  //  in
  //   acquisition number
  //  out
  //   absolute file string
  string getXfmFilename(int _seriesNum, int _acquisitionNum);

  // gets the next series number to be saved in the current image directory
  // inspects the series currently in the directory and makes a new one
  //unsigned int getNextSeriesNum();

  // sends an image to a event handler
  //  in
  //   img: image to send
  //bool sendImageToReader(RtDataImage &img);

  // deleted some received images
  //  in
  //   deleteNum: maximum number of images to delete
  void deleteReceivedImages(int deleteNum);

protected:

  // determines if the received image is the first image in a series or not
  // examines the acquisition number for 1
  //  in
  //   info struct for the image to test
  //  out
  //   true if this image is the first in a series
  bool isFirstInSeries(const RtExternalImageInfo &info);

  //*** private data members  ***//

  // port to listen on
  unsigned short port;
  RtSocketAcceptor acceptor;   
  ACE_SOCK_Stream stream;   

  // whether to only read moco images
  bool onlyReadMoCo;

  // parms for image saving to disk
  bool   saveImagesToFile;
  bool unmosaicInputImages;
//  string savePath;
//  string saveDir;
//  string saveFilestem;
//  string saveFileext;

  // scanner parms
  unsigned int matrixSize;
  unsigned int numSlices;
  double sliceGap;
  double voxDim[3];
  
  // current series number
  unsigned int seriesNum;

  // whether alignment to previous series should be performed
  bool alignSeries;
  string alignTarget;

  // vector to store received images 
  vector<RtMRIImage*> received;

  // iterator that points to the first undeleted image
  vector<RtMRIImage*>::iterator toBeDeleted;

};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

