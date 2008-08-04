# ########## Project setup ##########
PROJECT(preprocess)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## preprocess library ##########
# Sources:
SET(preprocess_library_SRCS
    RtVar.cpp
    RtPreprocessor.cpp
    RtIntensityNorm.cpp
    RtMean.cpp
    RtDiff.cpp
)

# Headers:
SET(preprocess_library_HDRS
    RtMean.h
    RtIntensityNorm.h
    RtVar.h
    RtDiff.h
    RtPreprocessor.h
)



# actual target:
ADD_LIBRARY(preprocess ${preprocess_library_SRCS} ${preprocess_library_HDRS})
SET_TARGET_PROPERTIES(preprocess PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS preprocess DESTINATION lib)
