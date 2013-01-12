/*=========================================================================
 *  debug levels
 * 
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef	DEBUG_LEVELS_H
#define DEBUG_LEVELS_H

#define NONE     ((unsigned int) (0))
#define TEMP     ((unsigned int) (1 << 0))
#define BASIC    ((unsigned int) (1 << 1))
#define MODERATE ((unsigned int) (1 << 2))
#define ALLOC    ((unsigned int) (1 << 3))
#define ADVANCED ((unsigned int) (1 << 4))
#define TIMER    ((unsigned int) (1 << 5))

//#define DEBUG_LEVEL (TEMP | BASIC | MODERATE | ALLOC | ADVANCED)
//#define DEBUG_LEVEL (BASIC | ADVANCED)
//#define DEBUG_LEVEL (MODERATE)
//#define DEBUG_LEVEL (BASIC)
#define DEBUG_LEVEL (TEMP)
//#define DEBUG_LEVEL (NONE)

#endif
