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

#include"RtInputScannerImages.h"

#include<fstream>

#include"site_config.h"

#include"RtConfigFmriExperiment.h"
#include"RtDataIDs.h"
#include"RtDesignMatrix.h"
#include"RtExperiment.h"
#include"RtExternalSenderImageInfo.h"
#include"RtFramewiseDisplacement.h"
#include"RtMotion.h"
#include"RtMotionDerivative.h"

#include<vnl_matrix_fixed.h>
#include<vnl_vector.h>

using namespace std;

// defaults
static const int    DEFAULT_PORT = 15000;

// increase this size for highres acquisitions
#define MAX_BUFSIZ 256*256*256*2
static char buffer[MAX_BUFSIZ];

static bool verbose = false;

// default constructor
RtInputScannerImages::RtInputScannerImages()
    :  port(DEFAULT_PORT)
{
  addToID(":scanner:images");
  saveImagesToFile = false;
  print = true;
  unmosaicInputImages = true;
  onlyReadMoCo = false;
  alignSeries = false;

  num2Discard = 0;

  initialized = false;
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

bool RtInputScannerImages::init(RtConfigFmriRun &config) {
  bool ret = init();
  if(config.isSet("scanner:discard")) {
    num2Discard = config.get("scanner:discard");
  }

  return ret;
}

bool RtInputScannerImages::init() {
  haveStudyRefVol = false;
  haveSeriesRefVol = false;

  numDiscarded = 0;
  num2Discard = 0;

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
  for(; isOpen && acceptor.accept(stream) != -1;) {

    if(!initialized) {
      cerr << "ERROR: accepting images when scanner input not initialized!"
           << endl;
      continue;
    }

    if(verbose) {
      cout << "connection accepted" << endl;
    }

    // get the info
    ei = receiveImageInfo(stream);
    if(ei == NULL) {
      if(verbose) {
        cout << "couldn't parse image, discarding." << endl;
      }

      stream.close();
      continue;
    }

    ei->currentTR = std::max(ei->currentTR-num2Discard,
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


    if(onlyReadMoCo && !ei->isMotionCorrected) {
      cout << "ignoring non-MoCo image." << endl;
      continue;
    }
    else if(!ei->isMotionCorrected) {
      cout << "got non-MoCo image." << endl;
    }
    else {
      cout << "got MoCo image." << endl;
    }

    // build data class
    rti = new RtMRIImage(*ei, img);

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
      cout << "discarding image " << numDiscarded
           << " of " << num2Discard << endl;
      continue;
    }

    // if there is motion info add it
    if(ei->isMotionCorrected) {
      RtMotion *mot = new RtMotion(ei->mcTranslationXMM,
                                   ei->mcTranslationYMM,
                                   ei->mcTranslationZMM,
                                   ei->mcRotationXDeg,
                                   ei->mcRotationYDeg,
                                   ei->mcRotationZDeg);
      mot->getDataID().setSeriesNum(rti->getDataID().getSeriesNum());
      mot->getDataID().setTimePoint(rti->getDataID().getTimePoint());
      getDataStore().setData(mot);

      // find the last motion for derivative and framewise displacement
      RtDataID lastMotID(mot->getDataID());
      lastMotID.setTimePoint(rti->getDataID().getTimePoint() - 1);
      RtMotion *lastMot = static_cast<RtMotion*> (getDataStore().getData(lastMotID));

      RtMotionDerivative *md = NULL;
      RtFramewiseDisplacement *fd = NULL;
      if (lastMot != NULL) {
        md = new RtMotionDerivative(lastMot, mot);
        fd = new RtFramewiseDisplacement(lastMot, mot);
      }
      else {
        md = new RtMotionDerivative();
        fd = new RtFramewiseDisplacement();
      }

      md->getDataID().setSeriesNum(rti->getDataID().getSeriesNum());
      md->getDataID().setTimePoint(rti->getDataID().getTimePoint());
      getDataStore().setData(md);

      fd->getDataID().setSeriesNum(rti->getDataID().getSeriesNum());
      fd->getDataID().setTimePoint(rti->getDataID().getTimePoint());
      getDataStore().setData(fd);
    }

    // append this to a vector of gathered images
    getDataStore().setData(rti);

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

    // If it's not an EPI don't fire off the murfi processing pipeline.
    //
    // TODO: This will need to be changed in order to process other types of
    // data.
    if(!strcmp(ei->scanType, "EPI")) {
      sendCode(rti);
    } else {
      cout << "Non-EPI volume received, skipping stream processing."
           << endl;
    }

    if(saveImagesToFile) {
      saveImage(*rti);
      cout << "Saved image to file." << endl;
    }

    // log that we received the image
    infos.str("");
    infos << "received image from scanner: series "
          << rti->getDataID().getSeriesNum()
          << " acquisition " << ei->currentTR << endl;
    log(infos);

    if(print) {
      cout << "received image from scanner: series "
           << rti->getDataID().getSeriesNum()
           << " acquisition " << rti->getDataID().getTimePoint() << endl;
    }

    // clean up
    delete ei;
    delete [] img;

    if(ei->currentTR + num2Discard == ei->totalTR) {
      cout << "received last image." << endl;
      sendCode(NULL);
      init();
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

  // first 8 bytes are size of header, size of data
  int size_of_header = 0;
  stream.recv_n(&size_of_header, sizeof(int));

  int size_of_data = 0;
  stream.recv_n(&size_of_data, sizeof(int));

  for(rec = 0; rec < RtExternalImageInfo::getHeaderSize();){
    rec_delta = stream.recv_n(buffer + rec,
                              RtExternalImageInfo::getHeaderSize());
    rec += rec_delta;
    if(rec_delta <= 0) break;
  }

  // arbitrary lower limit
  if(rec != RtExternalImageInfo::getHeaderSize()) {
    return NULL;
  }

  ACE_DEBUG((LM_TRACE, ACE_TEXT("received header of size %d\n"), rec));

  // TODO implement this in a portable way
  return new RtExternalImageInfo(
      *reinterpret_cast<RtExternalImageInfo*>(buffer));
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

  // grab numPix from header (to support MEMPRAGE)
  long numPix = info.getNumVoxels();
  long imageSizeInBytes = info.getDataSize();
  if(verbose) {
    cout << "receiving image " << info.currentTR << " ("
         << numPix << " pixels; " << imageSizeInBytes << " bytes)" << endl;
  }

  for(unsigned int rec = 0; rec < imageSizeInBytes;
      rec += stream.recv_n (buffer+rec, imageSizeInBytes-rec)) {
  }

  // TODO support other datatypes
  short *img = new short[numPix];
  memcpy(img,buffer,imageSizeInBytes);

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
  return (unsigned int) info.currentTR == SERIES_FIRST_ACQ_NUM;
}
