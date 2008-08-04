# ########## Project setup ##########
PROJECT(display)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## display library ##########
# Sources:
SET(display_library_SRCS
    RtDisplayImage.cpp
)

# Headers:
SET(display_library_HDRS
    RtDisplayImage.h
)

SUBDIRS( glutmaster)

# actual target:
ADD_LIBRARY(display ${display_library_SRCS} ${display_library_HDRS})
SET_TARGET_PROPERTIES(display PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS display DESTINATION lib)
