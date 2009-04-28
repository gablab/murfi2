/******************************************************************************
 * RtElementAccess.h declares a class that can retreive and set elements in
 * RtDataImages of double or short template type
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-02-16 
 * 
 *****************************************************************************/

#ifndef RTELEMENTACCESS_H
#define RTELEMENTACCESS_H

#include"RtData.h"
#include"RtMaskImage.h"
#include<vector>
#include"RtTypes.h"

using namespace std;

// class declaration
class RtElementAccess {

public:

  // setup to access elements of this image and compute the indices of
  // non-NaN elements (O(n))
  RtElementAccess(RtData *_data);

  // setup to access elements of this image within a mask (O(1))
  // NOTE: calling this with a NULL mask will result in no indices being
  // computed, which can be used if element index mapping is handled outside
  // this class
  RtElementAccess(RtData *_data, RtMaskImage *_mask);

  // destructor
  virtual ~RtElementAccess();

  // get an element (double)
  double getDoubleElement(unsigned int index);

  // get an element (short)
  short getShortElement(unsigned int index);

  // set element (double)
  void setElement(unsigned int index, double val);

  // set element (short)
  void setElement(unsigned int index, short val);

  // get the element indices (from mask if there is one)
  vector<unsigned int> getElementIndices();
  
protected:

  template<class T> 
  void buildElementIndices();

  //*** data members  ***//

  RtData *data;
  RtMaskImage *mask;
  
  // if there is no mask we store our own (non-NaN) indices
  vector<unsigned int> elementIndices;
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


