# ########## Project setup ##########
PROJECT(data)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## data library ##########
# Sources:
SET(data_library_SRCS
    RtDataImage.cpp
    RtMaskImage.cpp
    RtActivation.cpp
    RtData.cpp
    RtEvent.cpp
    RtMRIImage.cpp
)

# Headers:
SET(data_library_HDRS
    RtEvent.h
    RtActivation.h
    RtLocker.h
    RtDataImage.h
    RtDataIDs.h
    RtData.h
    RtROI.h
    RtMaskImage.h
    RtMRIImage.h
)



# actual target:
ADD_LIBRARY(data ${data_library_SRCS} ${data_library_HDRS})
SET_TARGET_PROPERTIES(data PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS data DESTINATION lib)
