cmake_minimum_required (VERSION 2.4)
PROJECT(FASTR)

# The version number.
SET( FASTR_VERSION_MAJOR 0)
SET( FASTR_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
# configure_file (
#  "${PROJECT_SOURCE_DIR}/FastrConfig.h.in"
#  "${PROJECT_BINARY_DIR}/FastrConfig.h"
#  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
#list packages here
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
#list libraries here
m gsl gslcblas ACE glut vnl vcl netlib v3p_netlib vnl_algo gnuplot_i_vxl niftiio znz
)

SET(FASTR_CMAKE_DIR ${FASTR_SOURCE_DIR}/config/cmake/Modules)

SET(LinkLibraries
#list libraries here
io stream display tinyxml analysis preprocess postprocess glutmaster data
)

SET(VXL_DIR /software/fastr/packages/vxl)

SET(FASTR_INCLUDE_DIR ${FASTR_SOURCE_DIR}/executive ${FASTR_SOURCE_DIR}/io ${FASTR_SOURCE_DIR}/data ${FASTR_SOURCE_DIR}/stream ${FASTR_SOURCE_DIR}/stream/analysis ${FASTR_SOURCE_DIR}/stream/preprocess ${FASTR_SOURCE_DIR}/stream/postprocess ${FASTR_SOURCE_DIR}/display ${FASTR_SOURCE_DIR}/util /usr/include/gsl ${VXL_DIR}/include/vxl/core ${VXL_DIR}/include/vxl/vcl /software/fastr/packages/gnuplot_i_vxl/include /usr/include/nifti)
INCLUDE_DIRECTORIES(${FASTR_INCLUDE_DIR})

SET(FASTR_LINK_DIRS ${VXL_DIR}/lib /software/fastr/packages/gnuplot_i_vxl/lib)

ADD_DEFINITIONS(-DTIXML_USE_STL -DACE_NLOGGING=0 -DACE_NTRACE=1 -DACE_NDEBUG=0 -s -O2)

SUBDIRS( executive io data stream display)

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project
#SET(CurrentExe "fastr")
#ADD_EXECUTABLE(${CurrentExe}) 
#TARGET_LINK_LIBRARIES(${CurrentExe} ${LinkLibraries} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

#Once you're done modifying this template, you should rename it to "CMakeLists.txt"
