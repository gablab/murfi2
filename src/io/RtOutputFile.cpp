/*=========================================================================
 *  RtOutputfile.cpp defines a class that implements output operations
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

#include"RtOutputFile.h"
#include"RtExperiment.h"
#include<iostream>
#include<sstream>

#include"ace/Mutex.h"

// default constructor
RtOutputFile::RtOutputFile() : RtOutput() {
  addToID(":file");
}

// destructor
RtOutputFile::~RtOutputFile() {
}

// open and start accepting input
bool RtOutputFile::open(RtConfig &config) {

  if(!RtOutput::open(config)) {
    return false;
  }

  // if logging is off, return
  if(config.get("study:log:disabled")==true) {
    return true;
  }

  // open the file for output
  string logname = config.get("study:log:filename");

  // check the filename
  if(logname.empty()) {
    return false;
  }
  else if(logname.substr(0,1) != "/") {
    stringstream fs;
    fs << getExperimentConfig().get("study:log:directory") << "/"  << logname;

    logname = fs.str();
  }

  cout << "opening logfile " << logname << endl;
  outfp.open(logname.c_str(), fstream::out | fstream::app);

  // check
  if(outfp.fail()) {
    cerr << "ERROR: could not open log file " << logname << " for output"
         << endl;
    return false;
  }

  // write a header
  ACE_Mutex mutx;
  mutx.acquire();

  outfp << "################################################################"
        << endl << endl
        << "# realtime system log file" << endl
        << "created ";
  printNow();
  outfp  << endl << endl;

  outfp.flush();
  mutx.release();

  return true;
}

// write the string from a stringstream to the output file
void RtOutputFile::write(stringstream &ss) {
  write(ss.str());
}

// write a string to the output file
void RtOutputFile::write(const string &s) {
  if(!isOpen) {
    return;
  }

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  outfp << s;

  outfp.flush();

  mutx.release();
}

// close and clean up
bool RtOutputFile::close() {

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  outfp << "closed ";
  RtOutput::printNow(outfp);
  outfp << endl;
  outfp << endl;

  outfp << "################################################################"
        << endl << endl;

  outfp.close();

  isOpen = false;

  mutx.release();

  return true;
}

// prints the current time to the file
void RtOutputFile::printNow() {
  RtOutput::printNow(outfp);
}

// prints the configuration to the file
void RtOutputFile::writeConfig(RtConfig &config) {

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  outfp << "configuration:" << endl
        << "--------------" << endl;
  config.dumpConfig(outfp);
  outfp << "--------------" << endl << endl;

  mutx.release();
}
