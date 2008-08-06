# ########## Project setup ##########
PROJECT(postprocess)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## postprocess library ##########
# Sources:
SET(postprocess_library_SRCS
    RtEventTriggerActivationDiff.cpp
    RtActivationSum.cpp
    RtEventTrigger.cpp
    RtActivationSumDifference.cpp
    RtEventTriggerActivationSums.cpp
)

# Headers:
SET(postprocess_library_HDRS
    RtEventTriggerActivationSums.h
    RtEventTriggerActivationDiff.h
    RtActivationSum.h
    RtEventTrigger.h
    RtActivationSumDifference.h
)



# actual target:
ADD_LIBRARY(postprocess ${postprocess_library_SRCS} ${postprocess_library_HDRS})
SET_TARGET_PROPERTIES(postprocess PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS postprocess DESTINATION lib)
