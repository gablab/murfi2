# ########## Project setup ##########
PROJECT(executive)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## executive library ##########
# Sources:
SET(executive_library_SRCS
    RtCode.cpp
    RtConfig.cpp
    RtConductor.cpp
)

# Headers:
SET(executive_library_HDRS
    RtCode.h
    RtConfig.h
    RtConfigVal.h
    RtConductor.h
)

SUBDIRS( tinyxml)

# actual target:

# Make sure the linker can find the Hello library once it is built.
link_directories (${FASTR_LINK_DIRS})

# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx".  The extensions are automatically found.
SET(CurrentExe "fastr")
ADD_EXECUTABLE(${CurrentExe} ${executive_library_SRCS} ${executive_library_HDRS})
TARGET_LINK_LIBRARIES(${CurrentExe} ${LinkLibraries} ${Libraries})

# Link the executable to the Hello library.
# target_link_libraries (helloDemo Hello)

# add install target:
INSTALL(TARGETS ${CurrentExe} DESTINATION bin)
