/*=========================================================================
 *  RtDataViewer.h is the header for a call to view data in the datastore
 *  after all processing has occured
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

#ifndef RTDATAVIEWER_H
#define RTDATAVIEWER_H

#include"RtStreamComponent.h"
#include<vector>
using namespace std;

// class declaration
class RtDataViewer : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtDataViewer();

  // destructor
  ~RtDataViewer();

protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
			     const map<string,string> &attr);

  // make sure we are configured properly
  bool validateComponentConfig();

  // process a single acquisition (pure virtual)
  virtual int process(ACE_Message_Block *mb);

  vector<string> dataIds;
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


