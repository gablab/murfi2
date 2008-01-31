/******************************************************************************
 * RtDisplayImage.cpp defines a class for displaying single images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-29
 *
 *****************************************************************************/

#include"RtDisplayImage.h"
#include"RtDataIDs.h"

#include"ace/Mutex.h"

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
#define DEFAULT_X 400
#define DEFAULT_Y 0
#define DEFAULT_W 600
#define DEFAULT_H 600
#define DEFAULT_TITLE "real: experimentor"
#define DEFAULT_OVERLAYID "data.image.activation.voxel-singleimcor"


//*** constructors/destructors  ***//

// default constructor
RtDisplayImage::RtDisplayImage()
  : x(DEFAULT_X), y(DEFAULT_Y), width(DEFAULT_W), height(DEFAULT_H),
    img(NULL), imageTex(0), overlayTex(0), needsRepaint(true), newTex(false),
    newOverlay(false), newImageType(true), overlayOn(true),
    imageDisplayType(ID_SCANNERIMG) {

  id += ":display";
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
    img(NULL), imageTex(0), overlayTex(0), needsRepaint(true), newTex(false),
    newOverlay(false), newImageType(true), overlayOn(false),
    imageDisplayType(ID_SCANNERIMG) {

  overlayID = DEFAULT_OVERLAYID;
  strcpy(title,_title);
  id += ":display";
}

// destructor
RtDisplayImage::~RtDisplayImage() {
  glutDestroyWindow(windowID);
}


// initialize the display
//  in
//   config: config class to get configuration variables from
bool RtDisplayImage::open(RtConfig &config) {
  ACE_TRACE(("RtDisplayImage::open"));

  x = config.isSet("display:imageWinX")
    ? config.get("display:imageWinX") : DEFAULT_X;

  y = config.isSet("display:imageWinY")
    ? config.get("display:imageWinY") : DEFAULT_Y;

  width = config.isSet("display:imageWinW")
    ? config.get("display:imageWinW") : DEFAULT_W;

  height = config.isSet("display:imageWinH")
    ? config.get("display:imageWinH") : DEFAULT_H;

  strcpy(title, config.isSet("display:imageWinTitle")
	 ? config.get("display:imageWinTitle").str().c_str() : DEFAULT_TITLE);

  overlayID = config.isSet("display:overlayID")
	 ? config.get("display:overlayID").str() : DEFAULT_OVERLAYID;

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
  ACE_TRACE(("RtDisplayImage::init(..)"));

  x = _x;
  y = _y;
  width = _w;
  height = _h;
  strcpy(title,_title);

  return init();
}

// initialize the display
bool RtDisplayImage::init() {
  ACE_TRACE(("RtDisplayImage::init()"));

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(x, y);
  glViewport(0, 0, width, height);

  glutMaster.CallGlutCreateWindow(title, this);
  glutMaster.EnableTimerFunction();
  glutMaster.SetTimerToCurrentWindow();

  /* erase color */
  glClearColor(0.0f, 0.0f, 0.0f, 1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (double) width, 0.0, (double) height, 1.0, -1.0);

  return true;
}

// thread entry
int RtDisplayImage::svc() {
  ACE_TRACE(("RtDisplayImage::svc"));

  glutMaster.CallGlutMainLoop();

  return 0;
}

// sets the image to be displayed
void RtDisplayImage::setData(RtData *data) {
  ACE_TRACE(("RtDisplayImage::setData"));

  // handle overlay
  if(data->getID() == overlayID) {
    overlay = (RtActivation*) data;
    newOverlay = true;
  cout << "display got an overlay " << img->getID() << endl;
    return;
  }

  // handle background image
  if(data->getID() != imageDisplayType) {
    ACE_DEBUG((LM_DEBUG, "ignoring image of type %s\n", data->getID()));
    return;
  }

  img = (RtMRIImage*) data;

  ACE_DEBUG((LM_DEBUG, "display got an image %d\n", img->getAcquisitionNum()));
  cout << "display got an image " << img->getID() << endl;

  // set the info strings
  bottomStr = img->getID();

  stringstream s;
  s << img->getAcquisitionNum();
  topStr = s.str();

  newTex = true;
}

// makes a texture from the image data and prepares it for display
void RtDisplayImage::makeTexture() {
  ACE_TRACE(("RtDisplayImage::makeTexture"));


  /* delete the old texture if there is one */
  if(glIsTexture(imageTex)) {
    glDeleteTextures(1, &imageTex);
  }

  /* get the id for the texture */
  glGenTextures(1, &imageTex);

  if(newImageType) {

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

    cout << "image " << img->getID() << " bright: " << brightness << " contrast: " << contrast << endl;

    newImageType = false;
  }

  /* create the image texture */
  glBindTexture(GL_TEXTURE_RECTANGLE_EXT, imageTex);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, 1, img->getDim(0),
	       img->getDim(1), 0, GL_LUMINANCE,
	       GL_UNSIGNED_SHORT, img->getData());

  if(!glIsTexture(imageTex)) {
    cerr << "ERROR: could not generate a new texture" << endl;
  }

  needsRepaint = true;
  //CallBackDisplayFunc();
}

// makes a texture from the overlay data and prepares it for display
void RtDisplayImage::makeOverlayTexture() {
  ACE_TRACE(("RtDisplayImage::makeOverlayTexture"));

  /* delete the old texture if there is one */
  if(glIsTexture(overlayTex)) {
    glDeleteTextures(1, &overlayTex);
  }

  /* get the id for the texture */
  glGenTextures(1, &overlayTex);

//  double contrast = overlay->getAutoContrast();
//  glPixelTransferf(GL_RED_SCALE,   contrast);
//  glPixelTransferf(GL_GREEN_SCALE, contrast);
//  glPixelTransferf(GL_BLUE_SCALE,  contrast);
//
//
//  /* brightness */
//  float brightness = img->getAutoBrightness();
//  glPixelTransferf(GL_RED_BIAS,   brightness);
//  glPixelTransferf(GL_GREEN_BIAS, brightness);
//  glPixelTransferf(GL_BLUE_BIAS,  brightness);
//
//  cout << "overlay " << overlay->getID() << " bright: " << brightness << " contrast: " << contrast << endl;

  // convert overlay data into a displayable image
  unsigned short *overlayImg = new unsigned short[4*overlay->getNumPix()];
  for(unsigned int i = 0; i < overlay->getNumPix(); i++) {
    // use a cheap heat colormap
    if(!overlay->getScaleIsInverted() 
       && overlay->getPixel(i) > overlay->getThreshold()) {
      overlayImg[4*i+0] = USHRT_MAX; // r
      overlayImg[4*i+1] = (unsigned short) rint(((overlay->getPixel(i)
						     -overlay->getThreshold())/*/overlay->getCeiling()*/)
						*USHRT_MAX); // g
      overlayImg[4*i+2] = 0; // b
      overlayImg[4*i+3] = USHRT_MAX; // a

    }
    else if(!overlay->getScaleIsInverted() 
	    && overlay->getPixel(i) < -overlay->getThreshold()) {
      overlayImg[4*i+0] = 0; // r
      overlayImg[4*i+1] = (unsigned short) rint(((overlay->getPixel(i)
						  -overlay->getThreshold())/*/overlay->getCeiling()*/)
						*USHRT_MAX); // g
      overlayImg[4*i+2] = USHRT_MAX; // b
      overlayImg[4*i+3] = USHRT_MAX; // a

    }
    else if(overlay->getScaleIsInverted() 
	    && fabs(overlay->getPixel(i)) < overlay->getThreshold()) {
      overlayImg[4*i+0] = USHRT_MAX; // r
      overlayImg[4*i+1] = (unsigned short) 
	(1 - (fabs(overlay->getPixel(i))/overlay->getThreshold()))
	*USHRT_MAX; // g
      overlayImg[4*i+2] = 0; // b
      overlayImg[4*i+3] = USHRT_MAX; // a
    }
    else {
      overlayImg[4*i+0] = 0; // r
      overlayImg[4*i+1] = 0; // g
      overlayImg[4*i+2] = 0; // b
      overlayImg[4*i+3] = 0; // a
    }
  }


  /* create the image texture */
  glBindTexture(GL_TEXTURE_RECTANGLE_EXT, overlayTex);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, 4, overlay->getDim(0),
	       overlay->getDim(1), 0, GL_RGBA,
	       GL_UNSIGNED_SHORT, overlayImg);

  delete overlayImg;

  if(!glIsTexture(overlayTex)) {
    cerr << "ERROR: could not generate a new overlay texture" << endl;
  }

  needsRepaint = true;
  //CallBackDisplayFunc();
}

void RtDisplayImage::CallBackDisplayFunc(void) {
  ACE_TRACE(("RtDisplayImage::CallBackDisplayFunc"));


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  //glAlphaFunc(GL_GREATER,0.1);
  //glEnable(GL_ALPHA_TEST);
  //glEnable(GL_CULL_FACE);

  //glClear(GL_COLOR_BUFFER_BIT);

  /* gl init stuff */
  glPushMatrix();
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // debug
  glColor3d(0.0,1.0,0);
  glRectd(0.0, 0.0, 10, 10);
  glRectd(0.0, height, 10, height-10);
  glRectd(width, height, width-10, height-10);
  glRectd(width, 0.0, width-10, 10);



  // if there is no image yet, draw a message and return
  if(!glIsTexture(imageTex) || img == NULL) {
    drawString(10,10,"no image loaded",1,0,0);
    glutSwapBuffers();
    return;
  }

  //  ACE_DEBUG((LM_DEBUG, "showing image %d\n", img->getAcquisitionNum()));

  int imgw = img->getDim(1);
  int imgh = img->getDim(0);

  /* turn on texture mapping */
  glEnable(GL_TEXTURE_RECTANGLE_EXT);

  /* draw the main texture */
  glBindTexture(GL_TEXTURE_RECTANGLE_EXT, imageTex);

  /* make a quadrilateral and provide texture coords */
  glBegin(GL_QUADS); {
    glTexCoord2d(0.0,0.0);
    glVertex3f(0.0, height, 0.0);
    glTexCoord2d(imgw,0.0);
    glVertex3f(width, height, 0.0);
    glTexCoord2d(imgw, imgh);
    glVertex3f(width, 0.0, 0.0);
    glTexCoord2d(0.0,imgh);
    glVertex3f(0.0, 0.0, 0.0);
  } glEnd();

  /* draw the overlay texture */
  if(overlayOn && glIsTexture(overlayTex)) {
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, overlayTex);

    /* make a quadrilateral and provide texture coords */
    glBegin(GL_QUADS); {
      glTexCoord2d(0.0,0.0);
      glVertex3f(0.0, height, 0.0);
      glTexCoord2d(imgw,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imgw, imgh);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imgh);
      glVertex3f(0.0, 0.0, 0.0);
    } glEnd();

  }

  glDisable(GL_TEXTURE_RECTANGLE_EXT);

  // draw the strings
  drawString(10,10,bottomStr.c_str(),1,0,0);
  drawString(10,height-20,topStr.c_str(),0,1,0);


  glutSwapBuffers();

}

void RtDisplayImage::CallBackReshapeFunc(int w, int h){

  if(width != w | height != h) {
    glutReshapeWindow(width,height);
  }
//
//   width = w;
//   height= h;
//
//   cout << w << " " << h << endl;
//
//   glViewport(0, 0, width, height);
//
//   CallBackDisplayFunc();
}


void RtDisplayImage::CallBackTimerFunc(int time, int val) {

  if(newTex) {
    newTex = false;
    makeTexture();
  }

  if(newOverlay) {
    newOverlay = false;
    makeOverlayTexture();
  }

  if(needsRepaint) {
    needsRepaint = false;
    CallBackDisplayFunc();
  }

}

void RtDisplayImage::CallBackKeyboardFunc(unsigned char key, int x, int y) {
  string oldImageDisplayType = imageDisplayType;

  switch(key) {
  case 'q':
    exit(0);
    break;
  case 's':
    imageDisplayType = ID_SCANNERIMG;
    break;
  case 'd':
    imageDisplayType = ID_DIFFIMG;
    break;
  case 'm':
    imageDisplayType = ID_MEANIMG;
    break;
  case 'v':
    imageDisplayType = ID_VARIMG;
    break;
  case 'z':
    overlayOn = !overlayOn;
    break;
  }

  // signal if we are displaying a new type of image
  if(imageDisplayType != oldImageDisplayType) {
    newImageType = true;
  }
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


