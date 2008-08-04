# ########## Project setup ##########
PROJECT(util)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## util library ##########
# Sources:
SET(util_library_SRCS
    printVnlVector.cpp
    dump.c
    printVnl44Mat.cpp
    printVnlMat.cpp
    add_functions.cpp
)

# Headers:
SET(util_library_HDRS
    RtTypes.h
    constants.h
    circular.h
    add_functions.h
)

# actual target:
ADD_LIBRARY(util ${util_library_SRCS} ${util_library_HDRS})
SET_TARGET_PROPERTIES(util PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS util DESTINATION lib)
