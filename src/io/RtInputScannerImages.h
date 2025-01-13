/*=========================================================================
 *  RtInputScannerImages.h declares a class that implements scanner
 *  image communication operations
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef RTINPUTSCANNERIMAGES_H
#define RTINPUTSCANNERIMAGES_H

#include"RtInput.h"
#include"RtConfig.h"
#include"RtConfigFmriRun.h"
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

  // initialize a series run
  // call this before each run
  bool init();
  bool init(RtConfigFmriRun &config);

  // close and clean up
  bool close();

  // run the scanner input
  virtual int svc();

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

  // build a filename for a transform file operating on a given image
  // number
  //  in
  //   acquisition number
  //  out
  //   absolute file string
  string getXfmFilename(int _seriesNum, int _acquisitionNum);

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

  // whether to receive the siemsens pre-header
  bool preHeader;

  bool print;

  // parms for image saving to disk
  bool   saveImagesToFile;
  bool unmosaicInputImages;

  // whether alignment to previous series should be performed
  bool alignSeries;
  string alignTarget;

  // run specific info
  bool initialized;

  unsigned int num2Discard;
  unsigned int numDiscarded;

  bool haveStudyRefVol;
  bool haveSeriesRefVol;
};

#endif
