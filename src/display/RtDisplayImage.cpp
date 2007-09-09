/******************************************************************************
 * RtDisplayImage.cpp defines a class for displaying single images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-29
 * 
 *****************************************************************************/

#include"RtDisplayImage.h"

#ifdef GL_TEXTURE_RECTANGLE_EXT
#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT 
#endif
#else
#ifdef GL_TEXTURE_RECTANGLE_NV
#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV
#endif 
//#else
//#define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_2D
//#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_2D
#endif 
#endif

// defaults
#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_W 600
#define DEFAULT_H 600
#define DEFAULT_TITLE "real-time system image display"


//*** constructors/destructors  ***//
  
// default constructor
RtDisplayImage::RtDisplayImage() 
  : x(DEFAULT_X), y(DEFAULT_Y), width(DEFAULT_W), height(DEFAULT_H), 
    img(NULL), texture(0), needsRepaint(true), newTex(false) {
}

// constructor with stuff
//  in
//   _x: initial x position of the window
//   _y: initial y position of the window
//   _w: initial width of the window
//   _h: initial height of the window
//   _title: title string of the window
RtDisplayImage::RtDisplayImage(int _x, int _y, 
			       int _w, int _h, 
			       char *_title) 
  : x(_x), y(_y), width(_w), height(_h),
    img(NULL), texture(0), needsRepaint(true), newTex(false) {

  strcpy(title,_title);
}

// destructor
RtDisplayImage::~RtDisplayImage() {
  glutDestroyWindow(windowID);
}


// initialize the display
//  in
//   config: config class to get configuration variables from
bool RtDisplayImage::open(RtConfig &config) {
  
  x = config.get("imageDisplayWinX")==true
    ? config.get("imageDisplayWinX") : DEFAULT_X;

  y = config.get("imageDisplayWinY")==true
    ? config.get("imageDisplayWinY") : DEFAULT_Y;

  width = config.get("imageDisplayWinW")==true
    ? config.get("imageDisplayWinW") : DEFAULT_W;

  height = config.get("imageDisplayWinH")==true
    ? config.get("imageDisplayWinH") : DEFAULT_H;

  strcpy(title, config.get("imageDisplayWinTitle")==true
	 ? config.get("imageDisplayWinTitle").str().c_str() : DEFAULT_TITLE);


  return init();
}

// initialize the display
//  in
//   _x: initial x position of the window
//   _y: initial y position of the window
//   _w: initial width of the window
//   _h: initial height of the window
//   _title: title string of the window
bool RtDisplayImage::init(int _x, int _y, int _w, int _h, char *_title) {
  x = _x;
  y = _y;
  width = _w;
  height = _h;
  strcpy(title,_title);

  return init();
}

// initialize the display
bool RtDisplayImage::init() {

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(x, y);
  glViewport(0, 0, width, height);

  glutMaster.CallGlutCreateWindow(title, this);
  glutMaster.EnableIdleFunction();
  glutMaster.SetIdleToCurrentWindow();

  /* erase color */
  glClearColor(0.0f, 0.0f, 0.0f, 1);
   
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (double) width, 0.0, (double) height, 1.0, -1.0);

  return true;
}

// thread entry
int RtDisplayImage::svc() {
  glutMaster.CallGlutMainLoop();

  return 0;
}

// sets the image to be displayed
void RtDisplayImage::setData(RtData *data) {
  img = (RtDataImage*) data;

  ACE_DEBUG((LM_DEBUG, "display got an image %d\n", img->getAcquisitionNum()));


  // set the info strings
  bottomStr = img->getCreationTime();

  stringstream s;
  s << img->getAcquisitionNum();
  topStr = s.str();

  newTex = true;
}

// makes a texture from the image data and prepares it for display
void RtDisplayImage::makeTexture() {

  /* delete the old texture if there is one */
  if(glIsTexture(texture)) {
    glDeleteTextures(1, &texture);
  }

  /* get the id for the texture */
  glGenTextures(1, &texture);

  /* contrast */
  
  float contrast = img->getAutoContrast(); 
  glPixelTransferf(GL_RED_SCALE,   contrast);
  glPixelTransferf(GL_GREEN_SCALE, contrast);
  glPixelTransferf(GL_BLUE_SCALE,  contrast);
  
  
  /* brightness */
  float brightness = img->getAutoBrightness();
  glPixelTransferf(GL_RED_BIAS,   brightness);
  glPixelTransferf(GL_GREEN_BIAS, brightness);
  glPixelTransferf(GL_BLUE_BIAS,  brightness);

  /* create the image texture */
  glBindTexture(GL_TEXTURE_RECTANGLE_EXT, texture);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, 1, img->getDim(0), 
	       img->getDim(1), 0, GL_LUMINANCE, 
	       GL_UNSIGNED_SHORT, img->getData());  

  if(!glIsTexture(texture)) {
    cerr << "ERROR: could not generate a new texture" << endl;
  }  


  CallBackDisplayFunc();
}

void RtDisplayImage::CallBackDisplayFunc(void) {

  glClear(GL_COLOR_BUFFER_BIT);
   
  /* gl init stuff */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   
  // if there is no image yet, draw a message and return
  if(texture == 0 || img == NULL) {
    drawString(10,10,"no image loaded",1,0,0);
    glutSwapBuffers();
    return;
  }

  ACE_DEBUG((LM_DEBUG, "showing image %d\n", img->getAcquisitionNum()));

  int imgw = img->getDim(1);
  int imgh = img->getDim(0);

  /* turn on texture mapping */
  glEnable(GL_TEXTURE_RECTANGLE_EXT);
   
  /* draw the main texture */
  glBindTexture(GL_TEXTURE_2D, texture);
   
  /* make a quadrilateral and provide texture coords */
  glBegin(GL_QUADS); {
    glTexCoord2d(0.0,0.0);
    glVertex3f(0.0, width, 0.0);
    glTexCoord2d(imgw,0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2d(imgw, imgh);
    glVertex3f(height, 0.0, 0.0);
    glTexCoord2d(0.0,imgh);
    glVertex3f(height, width, 0.0);
  } glEnd();
   
  glDisable(GL_TEXTURE_RECTANGLE_EXT);

  // draw the strings
  drawString(10,10,bottomStr.c_str(),1,0,0);
  drawString(10,height-20,topStr.c_str(),0,1,0);

  glutSwapBuffers();
  
}

void RtDisplayImage::CallBackReshapeFunc(int w, int h){

   width = w;
   height= h;

   glViewport(0, 0, width, height); 

   CallBackDisplayFunc();
}

void RtDisplayImage::CallBackIdleFunc(void){

  if(newTex) {
    newTex = false;
    makeTexture();
  }

  if(needsRepaint) {
    needsRepaint = false;
    CallBackDisplayFunc();
  }

  //sleep(1); // to prevent cpu hogging (need to have a beeter way to do this)
}

// draws a black box that will enclose a string of text
void RtDisplayImage::drawBlackBoxForString(const char *str, GLint x, GLint y) {
  unsigned int strWid = 0, i;

  for(i = 0; i < strlen(str); i++) {
    strWid += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, str[i]);
  }

  glColor3f(0.0f,0.0f,0.0f);
  glBegin(GL_QUADS); {
    glVertex3d(x-10,y+20,0.0);
    glVertex3d(x-10,y-10,0.0);
    glVertex3d(x+strWid+10,y-10,0.0);
    glVertex3d(x+strWid+10,y+20,0.0);
  } glEnd();
}

//draws a string at the specified coordinates
void RtDisplayImage::drawString(GLint x, GLint y, const char* s, float r, float g, float b) {
  unsigned int lines, i;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  drawBlackBoxForString(s,x,y);

  glColor3f(r,g,b);
  glRasterPos2i(x, y);
  for(i = 0, lines = 0; i < strlen(s); i++) {
    if (s[i] == '\n') {
      lines++;
      glRasterPos2i(x, y-(lines*18));
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
  }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


