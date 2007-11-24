/******************************************************************************
 * RtImageSlideWinCor.cpp is the implementation of a class that computes the
 * instantaneous activation in an Image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtImageSlideWinCor.h"

string RtImageSlideWinCor::moduleString("voxel-slidewincor");

// default constructor
RtImageSlideWinCor::RtImageSlideWinCor() : RtStreamComponent(), windowLen(12) {
  id = moduleString;

  needsInit = true;
}

// destructor
RtImageSlideWinCor::~RtImageSlideWinCor() {
  cout << "destroyed" << endl;
}

// process a configuration option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtImageSlideWinCor::processOption(const string &name, const string &text) {
  // look for known options
  if(name == "regressor") {
    
    double regel;
    for(unsigned int i1 = 0, i2 = text.find(" "); 1; 
	i1 = i2+1, i2 = text.find(" ", i1)) {

      if(!RtConfigVal::convert<double>(regel, 
		text.substr(i1,i2 == string::npos ? text.size()-i1 : i2-i1))) {
	continue;
      }
      regressor.push_back(regel);

      if(i2 == string::npos) {
	break;
      }
    }

    return true;
  }
  else if(name == "windowLen") {
    return RtConfigVal::convert<unsigned int>(windowLen, text);    
  }

  return RtStreamComponent::processOption(name, text);
}  

// process a single acquisition
int RtImageSlideWinCor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtImageSlideWinCor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtImageSlideWinCor::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtImageSlideWinCor:process: image passed is NULL\n"));
    return 0;
  }

  // initialize the computation if necessary
  if(needsInit) {

    // initialize all subsidiary variables
    b = 0.0;
    d = 0.0;

    a.setInfo(*img);
    a.initToZeros();

    c.setInfo(*img);
    c.initToZeros();

    e.setInfo(*img);
    e.initToZeros();

    numTimepoints = 0;
    
    windowData.clear();
    windowData.reserve(windowLen);

    windowData.push_back(img);
    numTimepoints++;

    needsInit = false;
    return 0;
  }

  ACE_DEBUG((LM_DEBUG, "including image %d in the sliding window correlation estimate\n", img->getAcquisitionNum()));
  
  // validate sizes
  if(img->getNumPix() != a.getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtImageSlideWinCor::process: new image is different size than last image\n"));
    return -1;    
  }

  // allocate a new data image for the correlation
  RtActivation *cor = new RtActivation(*img);
  //cor->setScaleIsInverted(true);
  cor->setThreshold(0.5);

  //// setup 

  unsigned int
    regNewIndex = numTimepoints%regressor.size(),
    regOldIndex = (numTimepoints-windowLen)%regressor.size(),
    dataIndex   = numTimepoints%windowLen;

  // get the new regressor element (assume periodic stimulus)
  double regold = 0, regnew = regressor[regNewIndex];

  // treat two cases
  if(numTimepoints < windowLen) { // 1) still filling the window
    b = regnew;
    d = regnew*regnew;    
  }
  else { // 2) window full, now sliding
    regold = regressor[regOldIndex];
    b = b - regold + regnew;
    d = d - regold*regold + regnew*regnew;
  }
  
  // compute for each voxel
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    double 
      la = a.getPixel(i), 
      lc = c.getPixel(i), 
      le = e.getPixel(i), 
      xnew = img->getPixel(i),
      xold,
      n;

    // same two cases
    if(numTimepoints < windowLen) { // 1) still filling the window
      la += xnew*regnew; 
      lc += xnew;
      le += xnew*xnew; 
      n = numTimepoints;
    }
    else { // 2) sliding window
      xold = windowData[dataIndex]->getPixel(i);
      la += xnew*regnew - xold*regold; 
      lc += xnew - xold;
      le += xnew*xnew - xold*xold; 
      n = windowLen;
    }

    cor->setPixel(i, (n*la - b*lc) / sqrt((n*d - b*b) * (n*le-lc*lc)));

//    cout << cor->getPixel(i) << " ";
//	 << n << " "
//	 << la << " "
//	 << b << " "
//	 << lc << " "
//	 << d << " "
//	 << le << endl;

    a.setPixel(i,la);
    c.setPixel(i,lc);
    e.setPixel(i,le);
  }  
  cout << endl;

  // store the current image in the window
  windowData[dataIndex] = img;

  numTimepoints++;

  // set the image id for handling
  cor->addToID("voxel-slidewincor");
  setResult(msg,cor);

  return 0;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

