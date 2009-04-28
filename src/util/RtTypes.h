/******************************************************************************
 * types.h defines utility types (enums, typedefs)
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-01-26 
 *****************************************************************************/

#ifndef RTTYPES_H
#define RTTYPES_H

// numeric sign
enum Sign {POS, NEG, POSNEG};

// norm
enum Norm {L1, L2, LINF};

enum RtElementType {
  RT_UNKNOWN_TYPE,
  RT_SHORT_TYPE,
  RT_DOUBLE_TYPE
};

// deleted functionality
// typedef enum {
//   TREND = 0,
//   MOTION,
//   EVENT
// } Nuisance;

#endif
