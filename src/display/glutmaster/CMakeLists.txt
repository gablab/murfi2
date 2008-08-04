# ########## Project setup ##########
PROJECT(glutmaster)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## glutmaster library ##########
# Sources:
SET(glutmaster_library_SRCS
    glutWindow.cpp
    glutMaster.cpp
)

# Headers:
SET(glutmaster_library_HDRS
    glutMaster.h
    glutWindow.h
)



# actual target:
ADD_LIBRARY(glutmaster ${glutmaster_library_SRCS} ${glutmaster_library_HDRS})
SET_TARGET_PROPERTIES(glutmaster PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS glutmaster DESTINATION lib)
