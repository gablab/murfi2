# ########## Project setup ##########
PROJECT(tinyxml)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## tinyxml library ##########
# Sources:
SET(tinyxml_library_SRCS
    tinyxmlparser.cpp
    tinyxml.cpp
    tinyxmlerror.cpp
)

# Headers:
SET(tinyxml_library_HDRS
    tinyxmlparser.h
    tinyxml.h
    tinyxmlerror.h
)

# actual target:
ADD_LIBRARY(tinyxml ${tinyxml_library_SRCS} ${tinyxml_library_HDRS})
SET_TARGET_PROPERTIES(tinyxml PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS tinyxml DESTINATION lib)
