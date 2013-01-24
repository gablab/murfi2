/*=========================================================================
 *  RtInputScannerImages.cpp defines a class that implements scanner
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

static char *VERSION = "$Id$";

#include"RtInputScannerImages.h"

#include<fstream>

#include"site_config.h"

#include"RtConfigFmriExperiment.h"
#include"RtDataIDs.h"
#include"RtDesignMatrix.h"
#include"RtExperiment.h"

#include<vnl/vnl_matrix_fixed.h>
#include<vnl/vnl_vector.h>

using namespace std;

// defaults
static const int    DEFAULT_PORT = 15000;

// increase this size for highres acquisitions
#define MAX_BUFSIZ 256*256*256*2
#define MAX_SERIESNUM 99999
static char buffer[MAX_BUFSIZ];

static bool verbose = false;

// default constructor
RtInputScannerImages::RtInputScannerImages()
    :  port(DEFAULT_PORT),
       seriesNum(1)
{
  addToID(":scanner:images");
  saveImagesToFile = false;
  print = true;
  unmosaicInputImages = true;
  onlyReadMoCo = false;
  matrixSize = 64;
  numSlices = 32;
  sliceGap = 0.1;
  voxDim[0] = voxDim[1] = voxDim[2] = 1.0;

  alignSeries = false;

  num2Discard = 0;

  initialized = false;
  imageNum = 0;
  numImagesExpected = 0;
  haveStudyRefVol = false;
  haveSeriesRefVol = false;

}

// destructor
RtInputScannerImages::~RtInputScannerImages() {
}

// configure and open
bool RtInputScannerImages::open(RtConfig &config) {
  RtInput::open(config);

  if(config.isSet("scanner:disabled")
     && config.get("scanner:disabled")==false) {
    initialized = true;
  }
  else {
    initialized = false;
  }

  // get port from the config and try to open the socket
  port = config.isSet("scanner:port")
      ? config.get("scanner:port") : DEFAULT_PORT;

  // build the address
  ACE_INET_Addr address(port,(ACE_UINT32)INADDR_ANY);

  if(acceptor.open(address,1) == -1) {
    cerr << "failed to open acceptor for scanner images on port "
         << port << endl;
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
  }

  // whether to print when images received
  if(config.get("scanner:print")==true) {
    print = true;
  }

  if(config.get("study:teminal:verbose")==true) {
    verbose = true;
  }

  // see if we should align the series to a reference
  if(config.isSet("scanner:alignSeries")
     && config.get("scanner:alignSeries")==true) {
    alignSeries = true;
  }
  else {
    alignSeries = false;
  }

  if(config.isSet("study:terminal:verbose")
     && config.get("study:terminal:verbose")==true) {
    verbose = true;
  }

  return true;
}

// close and clean up
//  out
//   success flag
bool RtInputScannerImages::close() {

  bool ret = true;

  cout << "shutting down the scanner image listener...";

  if(isOpen) {
    // other stuff
    if(acceptor.close() != 0) {
      cout << endl;
      cerr << "WARNING: failed to close the socket" << endl;
      ret = false;
    }
  }

  ret = ret && RtInput::close();

  if(ret) {
    cout << "done" << endl;
  }

  return ret;
}

// initialize a series run
// call this before each run
bool RtInputScannerImages::init(RtConfigFmriRun &config) {
  haveStudyRefVol = false;
  haveSeriesRefVol = false;

  imageNum = 0;

  numDiscarded = 0;
  if(config.isSet("scanner:discard")) {
    num2Discard = config.get("scanner:discard");
  }
  else {
    num2Discard = 0;
  }

  numImagesExpected = config.get("scanner:measurements");
  initialized = true;

  return true;
}

// run the scanner input
int RtInputScannerImages::svc() {
  RtExternalImageInfo *ei;
  short *img;
  RtMRIImage *rti;
  stringstream infos;

  cout << "listening for images on port " << port << endl;

  // continuously try to accept connections
  for(; isOpen
          && acceptor.accept(stream) != -1;) {

    if(!initialized) {
      cerr << "ERROR: accepting images when scanner input not initialized!"
           << endl;
      continue;
    }

    if(verbose) {
      cout << "connection accepted" << endl;
    }

    if(getExperimentConfig().get("study:timeComputations") == true
       && !startComputeTimer()) {
      cout << "warning: compute timer already started, "
           << "timing will be inaccurate"
           << endl;
    }
    else {
      cout << "started compute timer" << endl;
    }

    // get the info
    ei = receiveImageInfo(stream);
    if(ei == NULL) {
      stream.close();
      continue;
    }
    ei->iAcquisitionNumber = std::max(ei->iAcquisitionNumber-num2Discard,
                                      (unsigned int) 1);

    if(verbose) {
      cout << "image info received" << endl;
    }

    // get the image
    img = receiveImage(stream, *ei);
    if(verbose) {
      cout << "image received" << endl;
    }

    // close the stream (scanner connects anew for each image)
    stream.close();


    if(onlyReadMoCo && !ei->bIsMoCo) {
      cout << "ignoring non-MoCo image." << endl;
      continue;
    }
    else if(!ei->bIsMoCo) {
      cout << "got non-MoCo image." << endl;
    }
    else {
      cout << "got MoCo image." << endl;
    }

    imageNum++;

    // build data class
    rti = new RtMRIImage(*ei,img);

    if(unmosaicInputImages) {
      cout << "Source image is mosaic'd; unmosaicing." << endl;
      rti->unmosaic();
    } else {
      cout << "Source image is not mosaic'd; no need for unmosaicing." << endl;
    }

    // if its the first image in a series save it no matter what
    if(!haveSeriesRefVol) {
      rti->write(getExperimentConfig()
                 .getSeriesRefVolFilename(rti->getDataID().getSeriesNum()));
      haveSeriesRefVol = true;

      // register with reference
      if(alignSeries) {
        RtFSLInterface::registerSameSubjEPI(
            getExperimentConfig()
            .getSeriesRefVolFilename(rti->getDataID().getSeriesNum()),
            getExperimentConfig()
            .get("study:xfm:referenceVol"),
            getExperimentConfig()
            .getSeriesXfmFilename(rti->getDataID().getSeriesNum()),
            true);
      }
    }

    if(numDiscarded < num2Discard) {
      numDiscarded++;
      cout << "discarding image " << numDiscarded << " of " << num2Discard << endl;
      continue;
    }


    // if there is motion info add it
    if(ei->bIsMoCo) {
      RtMotion *mot = new RtMotion(ei->dMoCoTransX,
                                   ei->dMoCoTransY,
                                   ei->dMoCoTransZ,
                                   ei->dMoCoRotX,
                                   ei->dMoCoRotY,
                                   ei->dMoCoRotZ);
      getDataStore().setData(mot);
    }

    // set the image id for handling
    //rti->addToID("scanner");

    // PW 2012/11/26: Debug
    cout << "---------------------------------------------" << endl;
    cout << "getDataStore().getAvailableData():" << endl;
    getDataStore().getAvailableData();
    cout << "---------------------------------------------" << endl;

    // append this to a vector of gathered images
    getDataStore().setData(rti);

    // signal that we got an image
    //cout << "sending event with code number " << codeNum << endl;

    //rti->printInfo(cout);

    // if its the first epi image in an experiment save it no matter what
    if(!haveStudyRefVol
       && rti->getDataID().getDataName() == NAME_SCANNERIMG_EPI
       ) {

      if(!getExperimentConfig().getStudyRefVolExists()) {
        if(rti->write(getExperimentConfig().get("study:xfm:referenceVol"))) {
          haveStudyRefVol = true;
        }
      }
      else {
        haveStudyRefVol = true;
      }
    }

    // If it's not a mosaic'd image (eg MPRAGE) don't fire off the murfi
    // processing pipeline.
    //
    // TODO make this a dedicated flag?
    if (ei->iNoOfImagesInMosaic==0) {
      cout << "Non-mosaic image (eg MPRAGE) received. refusing to process it."
           << endl;
    } else {
      cout << "Sending code..." << endl;
      sendCode(rti);
      cout << "   ...done!" << endl;
    }

    if(saveImagesToFile) {
      cout << "Saving image to file..." << endl;
      saveImage(*rti);
      cout << "   ...done!" << endl;
    }

    // log that we received the image
    infos.str("");
    infos << "received image from scanner: series "
          << rti->getDataID().getSeriesNum()
          << " acquisition " << ei->iAcquisitionNumber << endl;
    log(infos);

    if(print) {
      cout << "received image from scanner: series "
           << rti->getDataID().getSeriesNum()
           << " acquisition " << rti->getDataID().getTimePoint() << endl;
    }

    // clean up
    cout << "Cleaning up..." << endl;
    delete ei;
    delete [] img;
    cout << "   ... done!" << endl;

    if(imageNum == numImagesExpected) {
      cout << "received last image" << endl;
      sendCode(NULL);
      initialized = false;
    }
  }

  return 0;
}

// read the scanner image info from a socket stream
// NOTE: performes blocking read
//  in
//   stream: a socket stream to receive on
//  out
//   image info struct on successful read (NULL otherwise)
RtExternalImageInfo *RtInputScannerImages::receiveImageInfo(
    ACE_SOCK_Stream &stream) {
  int rec, rec_delta;
  // read until we have all the bytes we need
  // TODO add error handling here

  // PW 2012/11/21: Zeroing the buffer before reading header. Trying to
  // track down annoying os-dependent bug
  memset(buffer, 0, MAX_BUFSIZ);

  for(rec = 0; rec < EXTERNALSENDERSIZEOF;){
    rec_delta = stream.recv_n (buffer+rec, EXTERNALSENDERSIZEOF);
    rec += rec_delta;
    if(rec_delta <= 0) break;
  }
  // TODO validate that the correct number of bytes was received.

  // arbitrary lower limit
  if(rec < 100) {
    return NULL;
  }

  ACE_DEBUG((LM_TRACE, ACE_TEXT("received header of size %d\n"), rec));
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
short *RtInputScannerImages::receiveImage(ACE_SOCK_Stream &stream,
                                          const RtExternalImageInfo &info) {

  ACE_DEBUG((LM_DEBUG, "receiving data for %d:%d\n", seriesNum,
             info.iAcquisitionNumber));

  // PW 2012/10/11: Modified to grab numPix from header (to support MEMPRAGE)
  long numPix = info.lNumberOfPixels;
  if(verbose) {
    cout << "receiving image " << info.iAcquisitionNumber << " ("
         << numPix << " pixels; " << numPix*sizeof(short) << "bytes)" << endl;
  }

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
  return img.write(getExperimentConfig().getVolFilename(
                       img.getDataID().getSeriesNum(),
                       img.getDataID().getTimePoint()));
}

// determines if the received image is the first image in a series or not
// examines the acquisition number for 1
//  in
//   info struct for the image to test
//  out
//   true if this image is the first in a series
bool RtInputScannerImages::isFirstInSeries(const RtExternalImageInfo &info) {
  return (unsigned int) info.iAcquisitionNumber == SERIES_FIRST_ACQ_NUM;
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
