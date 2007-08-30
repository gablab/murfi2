/******************************************************************************
 * RtDisplayImage.h declares a class for displaying single images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-29
 * 
 *****************************************************************************/

#ifndef RTDISPLAYIMAGE_H
#define RTDISPLAYIMAGE_H

#include"RtDataImage.h"
#include"RtConfig.h"
#include"glutmaster/glutWindow.h"
#include"glutmaster/glutMaster.h"

#include"ace/Task.h"
#include"ace/Asynch_IO.h"

// class declaration
class RtDisplayImage : 
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
  //   _x: initial x position of the window
  //   _y: initial y position of the window
  //   _w: initial width of the window
  //   _h: initial height of the window
  //   _title: title string of the window
  bool init(int _x, int _y, int _w, int _h, char *_title);

  // initialize the display
  //  in
  //   config: configuration class
  bool init(RtConfig &config);

  // initialize the display
  bool init();

  // sets the window running
  virtual int svc();

  // sets the image to be displayed
  void setImage(RtDataImage &img);

  // makes a texture from the image data and prepares it for display
  void makeTexture(unsigned short *data);

  // callbacks for opengl
  void CallBackDisplayFunc(void);
  void CallBackReshapeFunc(int w, int h);   
  void CallBackIdleFunc(void);

  // callback for scanner image input
  virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);

protected:

  GlutMaster glutMaster;

  int x, y, width, height;
  int imgw, imgh;
  GLuint texture;

  char title[100];
  string bottomStr;
  string topStr;

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


