/******************************************************************************
 * RtAccumCor.cpp is the implementation of a class that computes the
 * instantaneous activation in an Image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtAccumCor.h"
#include"RtMRIImage.h"
#include"gsl/gsl_cdf.h"

string RtAccumCor::moduleString("accumcor");

// default constructor
RtAccumCor::RtAccumCor() : RtActivationEstimator() {
  componentID = moduleString;

  needsInit = true;

  z = f = g = h = NULL; 
}

// destructor
RtAccumCor::~RtAccumCor() {
  if(z != NULL) {
    delete z;
  }

  if(f != NULL) {
    delete f;
  }

  if(g != NULL) {
    delete g;
  }

  if(h != NULL) {
    delete h;
  }

  cout << "destroyed" << endl;
}

// process a configuration option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtAccumCor::processOption(const string &name, const string &text) {
  // look for known options

  return RtActivationEstimator::processOption(name, text);
}  

//#include "gnuplot_i_vxl.h"

// process a single acquisition
int RtAccumCor::process(ACE_Message_Block *mb) {
//  static int numComparisons = 0;
//  static vnl_vector<double> tc(248);
//  static Gnuplot gp = Gnuplot("lines");
//  gp.set_yrange(38000,41000);

  ACE_TRACE(("RtAccumCor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtAccumCor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtAccumCor:process: data passed is NULL\n"));
    return 0;
  }

//  tc.put(numTimepoints,(double) dat->getElement(16*32*32 + 28*32 + 14));
//  gp.reset_plot();
//  gp.plot_x(tc,"timeseries");
  
  numTimepoints++;

  // initialize the computation if necessary
  if(needsInit) {

    // get the number of datapoints we must process
    numData = dat->getNumEl();

    //// initialize all subsidiary variables
    C = new vnl_matrix<double>(numTrends+1,numTrends+2);
    C->fill(0.0);
    C->fill_diagonal(1e-7);

    c = new vnl_matrix<double>(numData,numTrends+2);
    c->fill(0.0);
    for(unsigned int i = 0; i < numData; i++) {
      c->put(i,numTrends+1,1e-7);
    }

    f = new vnl_vector<double>(numTrends+1);
    f->fill(0.0);

    g = new vnl_vector<double>(numTrends+1);
    g->fill(0.0);

    h = new vnl_vector<double>(numTrends+1);
    h->fill(0.0);

    z = new vnl_vector<double>(numTrends+1);
    z->fill(0.0);

    // build the mask image
    initEstimation(*dat);

    needsInit = false;
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtAccumCor::process: new data has wrong number of elements\n"));
    return -1;    
  }

  // allocate a new data image for the correlation
  RtActivation *cor = new RtActivation(*dat);
  cor->initToZeros();

  if(numTimepoints > numTrends+1) {
    cor->setThreshold(getTStatThreshold(numTimepoints-numTrends-1));
    cout << "t thresh: " << cor->getThreshold() << " (p=" 
	 << getProbThreshold() << ")" << endl;
  }

  //// element independent setup

  // build z
  for(unsigned int i = 0; i < numTrends; i++) {
    z->put(i,trends.get(numTimepoints-1,i));
  }
  z->put(numTrends,conditions.get(numTimepoints-1,0));
  
  double b_new, b_old = 1;

  // update the element independent entries of C
  for(unsigned int j = 0; j < numTrends+1; j++) {
    h->put(j, z->get(j)/C->get(j,j));
    b_new = sqrt(pow(b_old,2)+pow(h->get(j),2));
    f->put(j, b_new/b_old);
    g->put(j, h->get(j)/(b_new*b_old));
    b_old = b_new;
    
    for(unsigned int k = 0; k < numTrends+1; k++) {
      z->put(k, z->get(k) - h->get(j)*C->get(k,j));
      C->put(k,j, f->get(j)*C->get(k,j) + g->get(j)*z->get(k));
    }

  }


  //// compute t map for each element
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    if(!mask.getPixel(i)) {
      cor->setPixel(i,0.0);
      continue;
    }

    double z_hat = dat->getElement(i);
	  
    for(unsigned int j = 0; j < numTrends+1; j++) {
      z_hat = z_hat - h->get(j)*c->get(i,j);
      c->put(i,j, f->get(j)*c->get(i,j) + g->get(j)*z_hat);
    }	  
    c->put(i,numTrends+1, sqrt(pow(c->get(i,numTrends+1),2)+pow(z_hat/b_old,2)));
//    p(vi,vj,vk) = c(vi,vj,vk,L+1)/sqrt(c(vi,vj,vk,L+2)^2+c(vi,vj,vk,L+1)^2);
//    a(vi,vj,vk) = c(vi,vj,vk,L+1)/C(L+1,L+1);

    if(numTimepoints > numTrends+1) {
      cor->setPixel(i, sqrt((double)(numTimepoints-numTrends-1)) 
		    * c->get(i,numTrends)/c->get(i,numTrends+1));
//      cout << sqrt(numTimepoints-numTrends-1) 
//	* c->get(i,numTrends)/c->get(i,numTrends+1) << " ";
    }

//    if(i == 1 && numTimepoints == 2) {
//    if(i == 16*32*32 + 28*32 + 14) {
//      fprintf(stderr,"%d %d %f %f %f\n", numTimepoints, dat->getElement(i), 
//	      conditions.get(numTimepoints-1,0), z_hat, cor->getPixel(i));
//
//      cout << "z: "; printVnlVector(*z); cout << endl;
//      cout << "f: "; printVnlVector(*f); cout << endl;
//      cout << "g: "; printVnlVector(*g); cout << endl;
//      cout << "h: "; printVnlVector(*h); cout << endl;
//      cout << "C: " << endl;
//      C->print(cout);
//      cout << "c: "; printVnlVector(c->get_row(i)); cout << endl;
//
//      //if(i > 40) {
//	int trash;
//	cin >> trash;
//      //}
//    }
  }  
//  cout << endl;

  // set the image id for handling
  cor->addToID("voxel-accumcor");

  setResult(msg,cor);

  // test if this is the last measurement for mask conversion and saving
  if(numTimepoints == numMeas && saveTVol) {
    cout << "saving t vol to " << saveTVolFilename << endl;
    cout.flush();
    cor->write(saveTVolFilename);
  }
  if(numTimepoints == numMeas && savePosResultAsMask) {
    cout << "saving pos mask to " << savePosAsMaskFilename << endl;
    cout.flush();

    RtMaskImage *activationMask = cor->toMask(POS);
    activationMask->setFilename(savePosAsMaskFilename);
    activationMask->save();
  }
  if(numTimepoints == numMeas && saveNegResultAsMask) {
    cout << "saving neg mask to " << saveNegAsMaskFilename << endl;
    cout.flush();

    RtMaskImage *activationMask = cor->toMask(NEG);
    activationMask->setFilename(saveNegAsMaskFilename);
    activationMask->save();
  }

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

