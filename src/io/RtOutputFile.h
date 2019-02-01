/*=========================================================================
 *  RtOutputfile.h declares a class that implements output operations
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

#ifndef RTOUTPUTFILE_H
#define RTOUTPUTFILE_H

#include"RtOutput.h"
#include<fstream>

using namespace std;

// class declaration
class RtOutputFile : public RtOutput {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtOutputFile();

  // destructor
  virtual ~RtOutputFile();

  // open and start accepting input
  bool open(RtConfig &config);

  // write the string from a stringstream to the output file
  void write(stringstream &ss);

  // write a string to the output file
  void write(const string &s);

  // close and clean up
  bool close();

  // write to the file
  template<class T>
  RtOutputFile &operator<<(const T &t) {
    if(isOpen) {
      outfp << t;
    }
    return *this;
  }

  // outstream operator
  void writeConfig(RtConfig &config);

  // prints the current time
  void printNow();

 protected:

  //*** private data members  ***//
  std::ofstream outfp;

};

#endif
