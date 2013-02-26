#ifndef RT_DISPLAY
#define RT_DISPLAY

#include "RtOutput.h"
#include"ace/Task.h"
#include"ace/Asynch_IO.h"

class RtDisplay: public RtOutput,
                 public ACE_Task_Base, public ACE_Service_Handler {
 public:
  RtDisplay(){}

 private:

};

#endif
