/*=========================================================================
 *  RtDisplayImage.h declares a class for displaying single images
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

#ifndef RTDISPLAYIMAGE_H
#define RTDISPLAYIMAGE_H

#include"RtConfigFmriExperiment.h"
#include"RtConfigFmriRun.h"
#include"RtDataImage.h"
#include"RtMRIImage.h"
#include"RtMaskImage.h"
#include"RtActivation.h"
#include"RtDataListener.h"
#include"glutmaster/glutWindow.h"
#include"glutmaster/glutMaster.h"

#include"ace/Task.h"
#include"ace/Asynch_IO.h"
//#include "add_functions.h"

class Gnuplot;

// class declaration
class RtDisplayImage 
  : public GlutWindow, 
    public RtDataListener, 
    public ACE_Task_Base, 
    public ACE_Service_Handler {

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtDisplayImage();

  // constructor with config class
  RtDisplayImage(const RtConfig &config);

  // constructor with stuff
  RtDisplayImage(int _x, int _y, 
		 int _w, int _h, 
		 char *_title);

  // destructor
  virtual ~RtDisplayImage();

  // initialize the display
  //  in
  //   config: configuration class
  bool open(RtConfig &config);

  // initialize the display
  //  in
  //   _x: initial x position of the window
  //   _y: initial y position of the window
  //   _w: initial width of the window
  //   _h: initial height of the window
  //   _title: title string of the window
  bool init(int _x, int _y, int _w, int _h, char *_title);

  // initialize the display
  bool init();

  // prepare the display for a single run
  bool prepareRun(RtConfig &config);

  // sets the window running
  virtual int svc();

  // notification that new data is available
  void notify(const RtDataID &id);

  // makes a texture from the image data and prepares it for display
  void makeTexture();

  // makes a texture from the overlay data and prepares it for display
  void makeOverlayTexture(bool pos);

  // makes a texture from the mask data and prepares it for display
  void makePosMaskTexture();

  // makes a texture from the mask data and prepares it for display
  void makeNegMaskTexture();

  // callbacks for opengl
  void action(int code);
  void CallBackDisplayFunc(void);
  void CallBackReshapeFunc(int w, int h);   
  void CallBackTimerFunc(int, int);
  void CallBackKeyboardFunc(unsigned char key, int x, int y);
  void CallBackMenuFunc(int code);

protected:

  bool initialized;

  GlutMaster glutMaster;

  // window stuff
  int x, y, width, height, numMeas;
  char title[100];
  string bottomStr;
  string topStr;

  // data 
  RtMRIImage *img;
  bool loadInitialImage;
  string initialImageFilename;

  RtMaskImage *posMask;
  bool loadInitialPosMask;
  string initialPosMaskFilename;
  bool flipInitialPosMask;

  RtMaskImage *negMask;
  bool loadInitialNegMask;
  string initialNegMaskFilename;
  bool flipInitialNegMask;

  RtActivation *posOverlay;
  RtActivation *negOverlay;
  double threshold; // for stat display
  double thresholdModAmount;

  GLuint imageTex;
  int imageW, imageH;
  GLuint posOverlayTex;
  GLuint negOverlayTex;
  GLuint posMaskTex;
  GLuint negMaskTex;

  // holds activation sum over time
  vnl_vector<double> postc;
  vnl_vector<double> negtc;
  Gnuplot *gnuPlot; // plots activation sums

  // data ids to watch for in setData
  string imageDisplayType;
  string posOverlayID;
  string posOverlayRoiID;
  string negOverlayID;
  string negOverlayRoiID;
  string posActivationSumID;
  string posActivationSumRoiID;
  string negActivationSumID;
  string negActivationSumRoiID;
  string posMaskID;
  string posMaskRoiID;
  string negMaskID;
  string negMaskRoiID;

  // flags
  bool needsRepaint;
  bool newTex;
  bool newPosOverlay;
  bool newNegOverlay;
  bool newPosMask;
  bool newNegMask;
  bool newImageType;
  bool posOverlayOn;
  bool negOverlayOn;
  bool posMaskOn;
  bool negMaskOn;

  // movie flags
  bool makeMovie;
  string framePrefix;
  short curFrame;
  string screenShotCommand;

  // draws a black box that will enclose a string of text
  void drawBlackBoxForString(const char *str, GLint x, GLint y);

  //draws a string at the specified coordinates
  void drawString(GLint x, GLint y, const char* s, float r, float g, float b);

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


