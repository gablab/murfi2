/*=========================================================================
 *  RtEndTask.cpp is the source file for a class to serve as the final task
 *  in a processing stream. When it gets processed, it cleans up the stream.
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

#include"RtEndTask.h"

RtEndTask::RtEndTask(set<ACE_Message_Block*> *_openMsgs,
                     RtStream* _stream,
                     bool _isProcessor)
    : RtStreamComponent(), stream(_stream), isProcessor(_isProcessor) {
  componentID = "end-task";

  // end processing marker setup
  endTaskData.getDataID().setModuleID(componentID);

  openMsgs = _openMsgs;
}

int RtEndTask::process(ACE_Message_Block* mb) {
  // tell the data store that we are done processing for this TR
  if(isProcessor) {
    RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();
    if(msg) {
      RtMRIImage *dat = static_cast<RtMRIImage*>(msg->getCurrentData());
      if(dat) {
        int tp = dat->getDataID().getTimePoint();
        endTaskData.getDataID().setTimePoint(tp);
        storeData(&endTaskData);

        if (stream != NULL && tp == getConductor().getNumExpectedTimePoints()) {
          stream->doneProcessing();
        }
      }
    }
  }

  if(DEBUG_LEVEL & TIMER) {
    cout << "RtStream elapsed time: "
         << getExperimentElapsedTime() << endl;
  }

  if(openMsgs != NULL) {
    if(DEBUG_LEVEL & MODERATE) {
      cout << "RtEndTask: mb is "
           << mb << " openMsgs->size() is " << openMsgs->size() << endl;
    }
    openMsgs->erase(mb);
  }

  return 0;
}
