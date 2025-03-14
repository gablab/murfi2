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

#include<cstdio>
#include<filesystem>
#include<format>
#include<fstream>
#include<regex>
#include<set>
#include<thread>

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
static const int DEFAULT_PORT = 15000;
static const int DEFAULT_MAX_DCM2NIIX_TRIES = 10;
static const int DEFAULT_DICOM_READ_TRY_DELAY_MS = 20;

// increase this size for highres acquisitions
#define MAX_BUFSIZ 256*256*256*2
static char buffer[MAX_BUFSIZ];

static bool verbose = false;

// default constructor
RtInputScannerImages::RtInputScannerImages()
    :  port(DEFAULT_PORT),
       preHeader(false),
       source(VSEND),
       maxDicomReadTries(DEFAULT_MAX_DCM2NIIX_TRIES),
       dicomReadTryDelayMs(DEFAULT_DICOM_READ_TRY_DELAY_MS)
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

  if(!config.isSet("scanner:tr")) {
    cerr << "The scanner TR must be set in the configuration" << endl;
    return false;
  }
  tr = (float) config.get("scanner:tr");

  if(!config.isSet("scanner:measurements")) {
    cerr << "The number of measurements must be set in the configuration"
         << endl;
    return false;
  }
  numMeasurements = (int) config.get("scanner:measurements");

  if(config.isSet("scanner:imageSource")) {
    cout << "image source: " << config.get("scanner:imageSource").str() << endl;
     if(config.get("scanner:imageSource").str() == "DICOM") {
       source = DICOM;
     }
     else if(config.get("scanner:imageSource").str() == "VSEND") {
       source = VSEND;
     }
     else {
       cerr << "unknown image source: " << config.get("scanner:imageSource").str()
            << ", assuming VSEND" << endl;
       source = VSEND;
     }
  }
  else {
    cerr << "image source not set, assuming VSEND" << endl;
    source = VSEND;
  }

  if(source == DICOM) {
    if(!config.isSet("scanner:inputDicomDir")) {
      cerr << "DICOM image source specified but no inputDicomDir" << endl;
      isOpen = false;
      return false;
    }
    dicomDir = config.get("scanner:inputDicomDir").str();

    if(config.isSet("scanner:maxDicomReadTries")) {
      maxDicomReadTries = config.get("scanner:maxDicomReadTries");
    }

    if(config.isSet("scanner:dicomReadTryDelayMs")) {
      dicomReadTryDelayMs = config.get("scanner:dicomReadTryDelayMs");
    }
  }
  else {
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

  // whether to receive the siemens preHeader bytes
  if(config.isSet("scanner:preHeader")
     && config.get("scanner:preHeader")==true) {
    preHeader = true;
  }

  // see if we should save images to a file
  if(config.get("scanner:save")==true) {
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
  RtMRIImage *rti;
  stringstream infos;

  if(source == DICOM) {
    cout << "watching for images in DICOM folder " << dicomDir << endl;
  }
  else {
    cout << "listening for images on port " << port << endl;
  }

  // continuously try to accept connections
  while(isOpen) {

    if(!initialized) {
      continue;
    }

    if(source == DICOM) {
      rti = readImageFromDICOMFolder();
      if(rti == NULL) {
        this_thread::sleep_for(chrono::milliseconds(1));
        continue;
      }
    }
    else {
      rti = receiveImageFromSocket();
      if(rti == NULL) {
        break;
      }
    }

    //rti->printInfo(cout);

    if(onlyReadMoCo && !rti->getMoco()) {
      cout << "ignoring non-MoCo image." << endl;
      continue;
    }
    else if(!rti->getMoco()) {
      cout << "got non-MoCo image." << endl;
    }
    else {
      cout << "got MoCo image." << endl;
    }

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
    if(rti->getMoco()) {
      RtMotion *mot = new RtMotion(rti->getTranslationX(),
                                   rti->getTranslationY(),
                                   rti->getTranslationZ(),
                                   rti->getRotationX(),
                                   rti->getRotationY(),
                                   rti->getRotationZ());
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

    // fire off the murfi processing pipeline.
    sendCode(rti);

    if(saveImagesToFile) {
      saveImage(*rti);
      cout << "Saved image to file." << endl;
    }

    // log that we received the image
    infos.str("");
    infos << "received image from scanner: series "
          << rti->getDataID().getSeriesNum()
          << " acquisition " << rti->getDataID().getTimePoint() << endl;
    log(infos);

    if(print) {
      cout << "received image from scanner: series "
           << rti->getDataID().getSeriesNum()
           << " acquisition " << rti->getDataID().getTimePoint() << endl;
    }

    if(rti->getDataID().getTimePoint() + num2Discard == numMeasurements) {
      cout << "received last image." << endl;
      sendCode(NULL);
      init();
    }
  }

  return 0;
}

// receive an image from the scanner
RtMRIImage* RtInputScannerImages::receiveImageFromSocket() {
  if(acceptor.accept(stream) == -1) {
    return NULL;
  }

  if(verbose) {
    cout << "connection accepted" << endl;
  }

  // get the info
  RtExternalImageInfo* ei = receiveImageInfo(stream);
  if(ei == NULL) {
    if(verbose) {
      cout << "couldn't parse image, discarding." << endl;
    }

    stream.close();
    return NULL;
  }

  ei->currentTR = std::max(ei->currentTR-num2Discard,
                                    (unsigned int) 1);

  if(verbose) {
    cout << "image info received" << endl;
  }

  // get the image
  short *img = receiveImageData(stream, *ei);
  if(verbose) {
    cout << "image received" << endl;
  }

  // close the stream (scanner connects anew for each image)
  stream.close();

  RtMRIImage *rti = new RtMRIImage(*ei, img);

  delete ei;
  delete [] img;

  return rti;
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

  // sometimes siemens sends a pre-header
  if(preHeader) {
    // first 8 bytes are size of header, size of data
    int size_of_header = 0;
    stream.recv_n(&size_of_header, 4);

    int size_of_data = 0;
    stream.recv_n(&size_of_data, 4);
  }

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
short *RtInputScannerImages::receiveImageData(ACE_SOCK_Stream &stream,
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

// read the next unread image from disk
RtMRIImage* RtInputScannerImages::readImageFromDICOMFolder() {
  static set<string> alreadyRead;

  // first time we are called, mark all previously existing files as read
  static bool firstCall = true;
  if(firstCall) {
    for(const auto &entry: std::filesystem::recursive_directory_iterator(dicomDir)) {
      if(entry.is_directory()) {
        continue;
      }

      alreadyRead.insert(entry.path().string());
    }
    firstCall = false;
    return NULL;
  }

  // created a sorted set of the files in the dir
  set<filesystem::path> files;
  for(const auto &entry: filesystem::recursive_directory_iterator(dicomDir)) {
    if(entry.is_directory()) {
      continue;
    }

    files.insert(entry.path());
  }

  // get the set difference between the files and the already read files
  set<filesystem::path> notYetRead;
  set_difference(files.begin(), files.end(), alreadyRead.begin(), alreadyRead.end(), inserter(notYetRead, notYetRead.begin()));

  // if there are no files to read, return NULL
  if(notYetRead.empty()) {
    return NULL;
  }

  // get the first file to read
  string toRead = notYetRead.begin()->string();
  cout << "found next dicom file to read as input: " << toRead << endl;

  // even though we may fail to read the image, we don't want to try again
  // because we have no reason to think it will succeed in the future
  alreadyRead.insert(toRead);

  // try to read the image multiple times. this is a hack to deal with the
  // fact that dcm2niix sometimes isn't finished writing the image before we try
  // to read it
  string output;
  for(int tryNum = 0; tryNum < maxDicomReadTries; tryNum++) {
    stringstream ss;
    cout << "dicom read try " << tryNum + 1 << " of " << maxDicomReadTries << endl;

    // compute and print the file size and last modified time using the filesystem library
    auto last_modified = std::filesystem::last_write_time(toRead);
    auto now = chrono::system_clock::now();
    auto file_size = std::filesystem::file_size(toRead);
    cout << std::format("file size: {} bytes, last modified: {} now: {}", file_size, last_modified, now) << endl;

    // invoke dcm2niix to convert the single image in verbose mode to /tmp
    string cmd = "dcm2niix -v y -f '%f_%p_%t_%s_%u' -s y -o /tmp " + toRead + " 2>&1";
    cout << "executing: " << cmd << endl;

    FILE *pipe = popen(cmd.c_str(), "r");
    if(!pipe) {
      cerr << "failed to create pipe: " << strerror(errno) << " (errno: " << errno << ")" << endl;
      continue;
    }
    char buffer[128];
    while(fgets(buffer, sizeof(buffer), pipe) != NULL) {
      ss << buffer;
    }
    output = ss.str();

    int exit_code = WEXITSTATUS(pclose(pipe));
    if(exit_code == 0 && output.substr(0, 7) != "Error: ") {
      break;
    } else {
      cout << "dcm2niix failed with exit code " << exit_code << " and output: " << output << endl;
    }

    // sleep for a short amount of time to avoid busy-waiting
    this_thread::sleep_for(chrono::milliseconds(dicomReadTryDelayMs));
  }

  // extract some info from the dcm2niix output

  // nifti filename
  regex filenameRegex("Convert 1 DICOM as (.*) ");
  smatch match;
  regex_search(output, match, filenameRegex);
  if(match.size() < 2) {
    cerr << "failed to parse filename from dcm2niix output." << endl;
    return NULL;
  }
  string niiFile = match[1].str() + ".nii";
  string jsonFile = match[1].str() + ".json";

  // acquisition and series number
  regex acqSerRegex(" acq ([0-9]+) img [0-9]+ ser ([0-9]+)");
  regex_search(output, match, acqSerRegex);
  if(match.size() < 3) {
    cerr << "failed to parse acq and series numbers from dcm2niix output." << endl;
    return NULL;
  }
  int acqNum = stoi(match[1].str());
  int serNum = stoi(match[2].str());

  cout << "reading series " << serNum << " image " << acqNum << " from " << niiFile << endl;
  acqNum = std::max(acqNum-num2Discard, (unsigned int) 1);

  RtMRIImage *rti = new RtMRIImage(niiFile, serNum, acqNum);
  rti->setRepetitionTime(tr);

  // Read the JSON file into a string
  std::ifstream jsonStream(jsonFile);
  string jsonStr((istreambuf_iterator<char>(jsonStream)), istreambuf_iterator<char>());
  jsonStream.close();
  remove(jsonFile.c_str());

  // Check for moco
  if (jsonStr.find("Reference volume for motion correction.") != string::npos) {
    cout << "found MoCo reference volume." << endl;
    rti->setMoco(true);
    rti->setTranslationX(0);
    rti->setTranslationY(0);
    rti->setTranslationZ(0);
    rti->setRotationX(0);
    rti->setRotationY(0);
    rti->setRotationZ(0);
  } else {
    // Try to match motion parameters
    regex mocoRegex("Motion: ([-]?\\d*\\.?\\d+),([-]?\\d*\\.?\\d+),([-]?\\d*\\.?\\d+),([-]?\\d*\\.?\\d+),([-]?\\d*\\.?\\d+),([-]?\\d*\\.?\\d+)");
    regex_search(jsonStr, match, mocoRegex);
    if(match.size() < 7) {
      cout << "no motion parameters from dcm2niix output, assuming non-MoCo image." << endl;
      rti->setMoco(false);
    } else {
      cout << "got motion parameters from dcm2niix output, assuming MoCo image." << endl;
      rti->setMoco(true);
      rti->setTranslationX(stof(match[1].str()));
      rti->setTranslationY(stof(match[2].str()));
      rti->setTranslationZ(stof(match[3].str()));
      rti->setRotationX(stof(match[4].str()));
      rti->setRotationY(stof(match[5].str()));
      rti->setRotationZ(stof(match[6].str()));
    }
  }

  cout << "removing " << niiFile << " after read" << endl;
  remove(niiFile.c_str());

  return rti;
}

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
