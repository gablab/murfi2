/******************************************************************************
 * RtDataImageInfo.h declares a class for generic image info
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-08
 * 
 *****************************************************************************/

#include"RtDataImageInfo.h"


// deep copy constructor
RtDataImageInfo::RtDataImageInfo(const RtDataImageInfo &info) {
  (*this) = info;

  // deep copy matrices
  gsl_matrix_memcpy(vxl2ras, info.vxl2ras);
  gsl_matrix_memcpy(ras2ref, info.ras2ref);
}


// check whether a magic number is supported by this class
bool RtDataImageInfo::validateMagicNumber(unsigned int mn) {
  return mn == MAGIC_NUMBER;
}

// set the bytes per pixel and guess the magic number
void RtDataImageInfo::setBytesPerPix(const unsigned short bpp) {
  bytesPerPix = bpp;
}

// check whether a magic number is supported by this class
//bool RtDataImageInfo::validateMagicNumber(unsigned int mn) {
//  return true;
//}

// set the magic number
void RtDataImageInfo::setMagicNumber(const unsigned int mn) {
  magicNumber = mn;
}

// print info members to cout 
void RtDataImageInfo::print() {
  print(cout);
}

// print some of the info members
void RtDataImageInfo::print(ostream &os) {
  ACE_TRACE(("RtDataImageInfo::print"));

  int wid = 30;

  os << setiosflags(ios::left);

  os << "---------------------------" << endl
     << "data info"                   << endl
     << "---------------------------" << endl
     << "magic number: " << magicNumber << endl
     << setw(wid) << "ndims: / dim1 / dim2... /" << numDims << ": / ";

  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
    os << *i << " / ";
  }

  os << endl
     << setw(wid) << "imgDataLen in bytes " << imgDataLen << endl
     << setw(wid) << "numPix" << numPix << endl
     << setw(wid) << "bytesPerPix" << bytesPerPix << endl;
    
  os << setw(wid) << "vxl2ras transform";
  for(int i = 0; i < 4; i++) {
    if(i > 0) {
      os << setw(wid) << "";
    }
    for(int j = 0; j < 4; j++) {
      os << gsl_matrix_get(vxl2ras,i,j) << " ";
    }
    os << endl;
  }

  os << setw(wid) << "ras2ref transform";
  for(int i = 0; i < 4; i++) {
    if(i > 0) {
      os << setw(wid) << "";
    }
    for(int j = 0; j < 4; j++) {
      os << gsl_matrix_get(ras2ref,i,j) << " ";
    }
    os << endl;
  }

  os << "---------------------------" << endl;
}

// convert a siemens hhmmss.xxxxxx time string to an ACE_Date_Time type
ACE_Date_Time RtDataImageInfo::siemensTime2ACE_Date_Time(const char *tc) {
  string timeStr = tc;
  ACE_Date_Time t;
  t.hour(atol(timeStr.substr(0,2).c_str()));
  t.minute(atol(timeStr.substr(2,2).c_str()));
  t.second(atol(timeStr.substr(4,2).c_str()));
  t.microsec(atol(timeStr.substr(7,6).c_str()));
  return t;
}

// convert an ACE_Date_Time type to  siemens hhmmss.xxxxxx time string
string RtDataImageInfo::ACE_Date_Time2SiemensTime(const ACE_Date_Time &t) {
  char str[] = "hhmmss.xxxxxx";
  sprintf(str,"%02ld%02ld%02ld.%06ld", 
	  t.hour(), t.minute(), t.second(), t.microsec());

  string s(str);
  return s;
}
