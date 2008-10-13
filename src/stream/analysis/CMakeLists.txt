# ########## Project setup ##########
PROJECT(analysis)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## analysis library ##########
# Sources:
SET(analysis_library_SRCS
    RtLeastSquaresSolve.cpp
    RtFluctuationMonitor.cpp
    RtActivationEstimator.cpp
    RtAccumCor.cpp
    RtIncrementalGLM.cpp
    RtImageZScore.cpp
    RtImageAccumCor.cpp
    RtSingleImageCor.cpp
    RtImageSlideWinCor.cpp
)

# Headers:
SET(analysis_library_HDRS
    RtIncrementalGLM.h
    RtFluctuationMonitor.h
    RtImageAccumCor.h
    RtActivationEstimator.h
    RtLeastSquaresSolve.h
    RtAccumCor.h
    RtImageSlideWinCor.h
    RtSingleImageCor.h
    RtImageZScore.h
)



# actual target:
ADD_LIBRARY(analysis ${analysis_library_SRCS} ${analysis_library_HDRS})
SET_TARGET_PROPERTIES(analysis PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS analysis DESTINATION lib)
