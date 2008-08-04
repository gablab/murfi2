# ########## Project setup ##########
PROJECT(io)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## io library ##########
# Sources:
SET(io_library_SRCS
    RtOutputFile.cpp
    RtOutputSocket.cpp
    RtOutput.cpp
    RtInput.cpp
    RtServerSocket.cpp
    RtExternalSenderImageInfo.cpp
    RtInputUSBKb.cpp
    RtIO.cpp
    RtInfoServer.cpp
    RtInputScannerImages.cpp
)

# Headers:
SET(io_library_HDRS
    RtInputUSBKb.h
    RtOutputSocket.h
    RtIO.h
    RtExternalSenderImageInfo.h
    RtOutput.h
    RtInfoServer.h
    ExternalSenderImageInfo.h
    RtInputScannerImages.h
    RtOutputFile.h
    RtInput.h
    RtServerSocket.h
)



# actual target:
ADD_LIBRARY(io ${io_library_SRCS} ${io_library_HDRS})
SET_TARGET_PROPERTIES(io PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS io DESTINATION lib)
