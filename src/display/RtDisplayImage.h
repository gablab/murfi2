/******************************************************************************
 * RtDisplayImage.h declares a class for displaying single images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-29
 * 
 *****************************************************************************/

#ifndef RTDISPLAYIMAGE_H
#define RTDISPLAYIMAGE_H

#include"RtDataImage.h"
#include"RtMRIImage.h"
#include"RtActivation.h"
#include"RtOutput.h"
#include"RtConfig.h"
#include"glutmaster/glutWindow.h"
#include"glutmaster/glutMaster.h"

#include"ace/Task.h"
#include"ace/Asynch_IO.h"

// class declaration
class RtDisplayImage : public RtOutput, 
  public GlutWindow, public ACE_Task_Base, public ACE_Service_Handler {

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

  // sets the window running
  virtual int svc();

  // sets the image to be displayed
  void setData(RtData *_img);

  // makes a texture from the image data and prepares it for display
  void makeTexture();

  // makes a texture from the overlay data and prepares it for display
  void makeOverlayTexture();

  // callbacks for opengl
  void CallBackDisplayFunc(void);
  void CallBackReshapeFunc(int w, int h);   
  void CallBackTimerFunc(int, int);
  void CallBackKeyboardFunc(unsigned char key, int x, int y);

protected:

  GlutMaster glutMaster;

  int x, y, width, height, numMeas;
  RtMRIImage *img;
  RtActivation *overlay;
  GLuint imageTex;
  GLuint overlayTex;

  bool needsRepaint, newTex, newOverlay, newImageType, overlayOn;

  char title[100];
  string bottomStr;
  string topStr;

  string imageDisplayType;

  // data id of the image type that will be used as an overlay
  string overlayID;

  // data id of the image type that will be used as an overlay
  string activationSumID;

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


