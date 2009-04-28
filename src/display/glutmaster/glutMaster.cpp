////////////////////////////////////////////////////////////////
//                                                            //
// glutMaster.c++                                             //
// beta version 0.3 - 9/9/97)                                 //
//                                                            //
// George Stetten and Korin Crawford                          //
// copyright given to the public domain                       //
//                                                            //
// Please email comments to email@stetten.com                 //
//                                                            //
////////////////////////////////////////////////////////////////

#include "glutMaster.h"
#include "glutWindow.h"


#include<iostream>
using namespace std;
                                                       
GlutWindow * viewPorts[MAX_NUMBER_OF_WINDOWS]; 

#ifndef DISABLE_IDLE
int GlutMaster::currentIdleWindow   = 0;
int GlutMaster::idleFunctionEnabled = 0;
#endif

int GlutMaster::currentTimerWindow   = 0;
int GlutMaster::timerFunctionEnabled = 0;


int GlutMaster::timerVal    = 0;
int GlutMaster::timerPeriod = 50; // msecs


GlutMaster::GlutMaster(){

   // Create dummy variables 

   char * dummy_argv[1];
   dummy_argv[0] = "run";
   int dummy_argc = 1;

   // Initialize GLUT

   glutInit(&dummy_argc, dummy_argv);
}

GlutMaster::~GlutMaster(){

}
 
void GlutMaster::CallBackDisplayFunc(void){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackDisplayFunc();
}

#ifndef DISABLE_IDLE
void GlutMaster::CallBackIdleFunc(void){

   if(idleFunctionEnabled && currentIdleWindow){
      glutSetWindow(currentIdleWindow);
      viewPorts[currentIdleWindow]->CallBackIdleFunc();
   }
}
#endif

void GlutMaster::CallBackTimerFunc(int val) {

   if(timerFunctionEnabled && currentTimerWindow){
      glutSetWindow(currentTimerWindow);
      viewPorts[currentTimerWindow]->CallBackTimerFunc(glutGet(GLUT_ELAPSED_TIME),val);
   }

   glutTimerFunc(timerPeriod, CallBackTimerFunc, timerVal); 

}
 
void GlutMaster::CallBackKeyboardFunc(unsigned char key, int x, int y){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackKeyboardFunc(key, x, y);
}

void GlutMaster::CallBackMenuFunc(int code){
   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackMenuFunc(code);
}

void GlutMaster::CallBackMotionFunc(int x, int y){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackMotionFunc(x, y);
}

void GlutMaster::CallBackMouseFunc(int button, int state, int x, int y){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackMouseFunc(button, state, x, y);
}

void GlutMaster::CallBackPassiveMotionFunc(int x, int y){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackPassiveMotionFunc(x, y);
}

void GlutMaster::CallBackReshapeFunc(int w, int h){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackReshapeFunc(w, h);
}

void GlutMaster::CallBackSpecialFunc(int key, int x, int y){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackSpecialFunc(key, x, y);
}   

void GlutMaster::CallBackVisibilityFunc(int visible){

   int windowID = glutGetWindow();
   viewPorts[windowID]->CallBackVisibilityFunc(visible);
}

void GlutMaster::CallGlutCreateWindow(char * setTitle, GlutWindow * glutWindow){
   // Open new window, record its windowID , 

   int windowID = glutCreateWindow(setTitle);

   glutWindow->SetWindowID(windowID);

   // Store the address of new window in global array 
   // so GlutMaster can send events to propoer callback functions.

   viewPorts[windowID] = glutWindow;

   // Hand address of universal static callback functions to Glut.
   // This must be for each new window, even though the address are constant.

   glutDisplayFunc(CallBackDisplayFunc);

#ifndef DISABLE_IDLE
   glutIdleFunc(CallBackIdleFunc); 
#endif

   glutTimerFunc(timerPeriod,CallBackTimerFunc,timerVal); 
   glutKeyboardFunc(CallBackKeyboardFunc);
   glutCreateMenu(CallBackMenuFunc);
   glutSpecialFunc(CallBackSpecialFunc);
   glutMouseFunc(CallBackMouseFunc);
   glutMotionFunc(CallBackMotionFunc);
   glutPassiveMotionFunc(CallBackPassiveMotionFunc);
   glutReshapeFunc(CallBackReshapeFunc); 
   glutVisibilityFunc(CallBackVisibilityFunc);
}

void GlutMaster::CallGlutMainLoop(void){

   glutMainLoop();
}
                              
void GlutMaster::CallGlutLeaveMainLoop(void){

   glutLeaveMainLoop();
}
                              
#ifndef DISABLE_IDLE

void GlutMaster::DisableIdleFunction(void){

   idleFunctionEnabled = 0;
}

void GlutMaster::EnableIdleFunction(void){

   idleFunctionEnabled = 1;
}

int GlutMaster::IdleFunctionEnabled(void){

   // Is idle function enabled?

   return(idleFunctionEnabled);
}

int GlutMaster::IdleSetToCurrentWindow(void){

   // Is current idle window same as current window?

   return( currentIdleWindow == glutGetWindow() );
}

void GlutMaster::SetIdleToCurrentWindow(void){

   currentIdleWindow = glutGetWindow();
}
#endif
                              
void GlutMaster::DisableTimerFunction(void){

   timerFunctionEnabled = 0;
}

void GlutMaster::EnableTimerFunction(void){

   timerFunctionEnabled = 1;
}

int GlutMaster::TimerFunctionEnabled(void){

   // Is timer function enabled?

   return(timerFunctionEnabled);
}

int GlutMaster::TimerSetToCurrentWindow(void){

   // Is current timer window same as current window?

   return( currentTimerWindow == glutGetWindow() );
}

void GlutMaster::SetTimerToCurrentWindow(void){

   currentTimerWindow = glutGetWindow();
}

int GlutMaster::TimerValue(void){

   return( timerVal );
}

void GlutMaster::SetTimerVal(int val){

  timerVal = val;
}


int GlutMaster::TimerPeriod(void){

   return( timerPeriod );
}

void GlutMaster::SetTimerPeriod(int period){

  timerPeriod = period;
}



























