# ########## Project setup ##########
PROJECT(stream)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## stream library ##########
# Sources:
SET(stream_library_SRCS
    RtPasser.cpp
    RtStreamMessage.cpp
    RtStreamComponent.cpp
    RtStream.cpp
)

# Headers:
SET(stream_library_HDRS
    RtStreamMessage.h
    RtStream.h
    RtStreamComponent.h
    RtPasser.h
)

SUBDIRS( analysis preprocess postprocess)

# actual target:
ADD_LIBRARY(stream ${stream_library_SRCS} ${stream_library_HDRS})
SET_TARGET_PROPERTIES(stream PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS stream DESTINATION lib)
