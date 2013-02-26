/*=========================================================================
 *  RtEndTask.h is the header for a class to serve as the final task in a
 *  processing stream. When it gets processed, it cleans up the stream.
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

#ifndef RTENDTASK_H
#define RTENDTASK_H

#include"RtStreamComponent.h"
#include"ace/Task.h"
#include<set>

class RtEndTask : public RtStreamComponent {
 public:

  RtEndTask(set<ACE_Message_Block*> *_openMsgs,
            RtStream* _stream,
            bool _isProcessor);

 protected:

  bool validateComponentConfig() {
    return true;
  }

  int process(ACE_Message_Block* mb);

  int nextStep(ACE_Message_Block *) {
    return 0;
  }

  set<ACE_Message_Block*> *openMsgs;

  // end processing marker
  RtMRIImage endTaskData;
  RtStream* stream;
  bool isProcessor;

};

#endif
