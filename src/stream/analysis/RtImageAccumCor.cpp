/******************************************************************************
 * RtImageAccumCor.cpp is the implementation of a class that computes the
 * instantaneous activation in an Image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtImageAccumCor.h"

string RtImageAccumCor::moduleString("voxel-accumcor");

// default constructor
RtImageAccumCor::RtImageAccumCor() : RtStreamComponent(), windowLen(12) {
  id = moduleString;

  needsInit = true;
}

// destructor
RtImageAccumCor::~RtImageAccumCor() {
  cout << "destroyed" << endl;
}

// process a configuration option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtImageAccumCor::processOption(const string &name, const string &text) {
  // look for known options
  if(name == "regressor") {
    
    double regel;
    for(unsigned int i = 0; unsigned int i1 = 0, i2 = text.find(" "); 1; 
	i++, i1 = i2+1, i2 = text.find(" ", i1)) {

      if(!RtConfigVal::convert<double>(regel, 
		text.substr(i1,i2 == string::npos ? text.size()-i1 : i2-i1))) {
	continue;
      }
      regressor.put(i,regel);

      if(i2 == string::npos) {
	break;
      }
    }

    return true;
  }
  if(name == "trends") {
    return RtConfigVal::convert<unsigned int>(numTrends,text);
  }
  else if(name == "measurements") {
    return RtConfigVal::convert<unsigned int>(numMeas,text);
  }

  return RtStreamComponent::processOption(name, text);
}  

// process a single acquisition
int RtImageAccumCor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtImageAccumCor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtImageAccumCor::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtImageAccumCor:process: image passed is NULL\n"));
    return 0;
  }

  // initialize the computation if necessary
  if(needsInit) {

    // initialize the trend vectors
    buildTrends();

    // initialize all subsidiary variables
    C.set_size(numTrends+1,numTrends+2);
    C.fill_diagonal(10e-7);

    c.set_size(numTrends+2);
    c.put(L+1,10e-7);

    f.set_size(numTrends+2);
    f.fill(0.0);

    g.set_size(numTrends+2);
    g.fill(0.0);

    h.set_size(numTrends+2);
    h.fill(0.0);

    z.set_size(numTrends+2);
    z.fill(0.0);

    numTimepoints++;

    needsInit = false;
    return 0;
  }

  ACE_DEBUG((LM_DEBUG, "including image %d in the sliding window correlation estimate\n", img->getAcquisitionNum()));
  
  // validate sizes
  if(img->getNumPix() != a.getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtImageAccumCor::process: new image is different size than last image\n"));
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

