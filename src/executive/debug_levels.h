/******************************************************************************
 * debug levels
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-19 
 *****************************************************************************/

#ifndef	DEBUG_LEVELS_H
#define DEBUG_LEVELS_H

#define NONE     ((unsigned int) (0))
#define TEMP     ((unsigned int) (1 << 0))
#define BASIC    ((unsigned int) (1 << 1))
#define MODERATE ((unsigned int) (1 << 2))
#define ALLOC    ((unsigned int) (1 << 3))
#define ADVANCED ((unsigned int) (1 << 4))

//#define DEBUG_LEVEL (TEMP | BASIC | MODERATE | ALLOC | ADVANCED)
//#define DEBUG_LEVEL (BASIC | ADVANCED)
//#define DEBUG_LEVEL (MODERATE)
//#define DEBUG_LEVEL (BASIC)
#define DEBUG_LEVEL (TEMP)
//#define DEBUG_LEVEL (NONE)

#endif
