////////////////////////////////////////////////////////////////
//                                                            //
// glutMaster.h                                               //
// beta version 0.3 - 9/9/97)                                 //
//                                                            //
// George Stetten and Korin Crawford                          //
// copyright given to the public domain                       //
//                                                            //
// Please email comments to email@stetten.com                 //
//                                                            //
////////////////////////////////////////////////////////////////

#ifndef __GLUT_MASTER_H__
#define __GLUT_MASTER_H__

#include <GL/glut.h>
#include "glutWindow.h"

// comment this out to make idling possible (but it'll eat all your cycles)
#define DISABLE_IDLE 1

#define MAX_NUMBER_OF_WINDOWS 256 

class GlutMaster{

private:

   static void CallBackDisplayFunc(void);

#ifndef DISABLE_IDLE
   static void CallBackIdleFunc(void); 
#endif

   static void CallBackTimerFunc(int val); 
   static void CallBackKeyboardFunc(unsigned char key, int x, int y);
   static void CallBackMotionFunc(int x, int y);
   static void CallBackMouseFunc(int button, int state, int x, int y);
   static void CallBackPassiveMotionFunc(int x, int y);
   static void CallBackReshapeFunc(int w, int h); 
   static void CallBackSpecialFunc(int key, int x, int y);   
   static void CallBackVisibilityFunc(int visible);

#ifndef DISABLE_IDLE
   static int currentIdleWindow;
   static int idleFunctionEnabled;
#endif

   static int currentTimerWindow;
   static int timerFunctionEnabled;

   static int timerVal;
   static int timerPeriod;

public:
 
   GlutMaster();
   ~GlutMaster();
    
   void CallGlutCreateWindow(char * setTitle, GlutWindow * glutWindow);
   void CallGlutMainLoop(void);

#ifndef DISABLE_IDLE
   void DisableIdleFunction(void);
   void EnableIdleFunction(void);
   int  IdleFunctionEnabled(void);
#endif

   int  IdleSetToCurrentWindow(void);
   void SetIdleToCurrentWindow(void);

   void DisableTimerFunction(void);
   void EnableTimerFunction(void);
   int  TimerFunctionEnabled(void);
   
   int  TimerSetToCurrentWindow(void);
   void SetTimerToCurrentWindow(void);
   
   int  TimerValue(void);
   void SetTimerVal(int);
   
   int  TimerPeriod(void);     
   void SetTimerPeriod(int);


};

#endif



