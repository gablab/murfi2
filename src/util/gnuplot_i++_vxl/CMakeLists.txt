# ########## Project setup ##########
PROJECT(gnuplot_i++_vxl)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## gnuplot_i++_vxl executable ##########
# Sources:
SET(gnuplot_i++_vxl_executable_SRCS
    gnuplot_i_vxl.cpp
    example.cpp
)

# Headers:
SET(gnuplot_i++_vxl_executable_HDRS
    gnuplot_i_vxl.h
)



# actual target:
ADD_EXECUTABLE(gnuplot_i++_vxl ${gnuplot_i++_vxl_executable_SRCS})

# add install target:
INSTALL(TARGETS gnuplot_i++_vxl DESTINATION bin)
