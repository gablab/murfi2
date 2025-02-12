##########################################################################
#
# master makefile for the real-time fMRI system murfi
#
##########################################################################


#********************* SETTABLE FLAGS *******************************#

### GENERAL

# os we build for
OS=linux

# project name
PROJECT = murfi

MAKEFLAGS := --jobs=$(shell nproc) --output-sync=target

# whether to compile with gui
export FRONTEND = 1

# directories
export SRC_DIR = $(PWD)/src
export BIN_DIR = $(PWD)/bin
export OBJ_DIR = $(PWD)/obj
INSTALL_DIR = /usr/local/bin/
INSTALL_BINARY = ./bin/$(PROJECT)

# whether to compile with debug, optimize flags
DEBUG = 0
PROF = 0
OPTIM = 1
STRIP = 1
BLANK :=

# for memory leak tracing
MTRACE = 0

# directories containing code and headers
SUB_DIRS = executive \
	   data \
	   io \
	   stream \
	   stream/analysis \
	   stream/preprocess \
	   stream/postprocess \
	   util

ifeq ($(FRONTEND),1)
  SUB_DIRS += qt
endif


### APP PATHS

export RM = /bin/rm -v
export ECHO = /bin/echo
export CC = /usr/bin/g++
export MOC = /usr/bin/moc-qt3

### LIBS AND LIB CONFIG

# global package directory
export PKG_DIR = /usr/local/

NIFTI_HOME=/usr

# vtk
VTK_HOME = /usr

# ace
ACE_NTRACE   = 1
ACE_NDEBUG   = 0
ACE_NLOGGING = 0
ACE_HOME = /usr

# boost
BOOST_HOME = /usr

# vxl
VXL_HOME = /usr

#svm
SVM_HOME = $(PWD)/util/svm

#********************* END SETTABLE FLAGS ***************************#



################################ FLAG ################################

# build with frontend flag
ifeq ($(FRONTEND),1)
	FRONT_FLAG = -DUSE_FRONTEND
endif

# debug flag
ifeq ($(DEBUG),1)
	DEBUG_FLAG = -g
endif

# profile flag
ifeq ($(PROF),1)
	PROF_FLAG = -pg
endif

# optimize flag
ifeq ($(OPTIM),1)
	OPTIM_FLAG = -O2
endif

# optimize flag
ifeq ($(STRIP),1)
	STRIP_FLAG = -s
endif

# mtrace flag
ifeq ($(MTRACE),1)
	MTRACE_FLAG = -DMTRACE
endif

# ace flags

ifeq ($(ACE_NTRACE),1)
	ACE_NTRACE_FLAG = -DACE_NTRACE=1
else
	ACE_NTRACE_FLAG = -DACE_NTRACE=0
endif

ifeq ($(ACE_NDEBUG),1)
	ACE_NDEBUG_FLAG = -DACE_NDEBUG=1
else
	ACE_NDEBUG_FLAG = -DACE_NDEBUG=0
endif

ifeq ($(ACE_NLOGGING),1)
	ACE_NLOGGING_FLAG = -DACE_NLOGGING=1
else
	ACE_NLOGGING_FLAG = -DACE_NLOGGING=0
endif

ACE_INC = -I$(ACE_HOME)/include/ace
ACE_LIB  = -L$(ACE_HOME)/lib -lACE
ACE_FLAGS=-D _REENTRANT $(ACE_NLOGGING_FLAG) $(ACE_NDEBUG_FLAG) $(ACE_NTRACE_FLAG)

# include headers from all sub directories
INC_SUB_DIRS = $(SUB_DIRS:%=-I$(SRC_DIR)/%)

# math
MATH_LIB =-lm

GSL_INC=-I/usr/include/gsl
GSL_LIB=-lgsl -lgslcblas


# computer vision
VXL_INC=-I$(VXL_HOME)/include/vxlinclude/vxlcoreinclude/vxlvnlinclude/vxlalgo \
	-I$(VXL_HOME)/include/vxlinclude/vxlcoreinclude/vxlvnl \
	-I$(VXL_HOME)/include/vxlinclude/vxlcoreinclude \
	-I$(VXL_HOME)/include/vxlinclude/vxlvcl \
	-I$(VXL_HOME)/include/vxl/vcl \
	-I$(VXL_HOME)/include/vxl/core \
	-I$(VXL_HOME)/include/vxl/core/vnl \
	-I$(VXL_HOME)/include/vxl/core/vnl/algo

VXL_LIB=-lvnl_algo -lvcl -lv3p_netlib -lvnl -L$(VXL_HOME)/vxl/lib

# svm
SVM_INC=-I$(SVM_HOME)/src -I$(SVM_HOME)/svm-multiclass
SVM_LIB=$(SVM_HOME)/src/*.o \
	$(SVM_HOME)/svm-multiclass/svm_struct_api.o \
	$(SVM_HOME)/svm-multiclass/svm_light/svm_common.o \
	$(SVM_HOME)/svm-multiclass/svm_light/svm_learn.o \
	$(SVM_HOME)/svm-multiclass/svm_light/svm_hideo.o \
	$(SVM_HOME)/svm-multiclass/svm_struct/svm_struct_learn.o \
	$(SVM_HOME)/svm-multiclass/svm_struct/svm_struct_common.o


# boost
BOOST_INC=-I$(BOOST_HOME)/include/
BOOST_LIB=-L$(BOOST_HOME)/lib -lboost_filesystem -lboost_system

# xml libs
TINYXML_FLAGS=-DTIXML_USE_STL

# nifti image io libs
NIFTI_INC=-I/$(NIFTI_HOME)/include/nifti
NIFTI_LIB=-lniftiio -lznz -lz -L/$(NIFTI_HOME)/lib

# gui libs
ifeq ($(FRONTEND),1)
  QT_LIB = -L/usr/X11R6/lib64 \
           -lQt5Widgets \
           -L/usr/lib/x86_64-linux-gnu \
           -lQt5Gui \
           -lQt5Core \
           -lQt5OpenGL \
           -lQt5PrintSupport \
           -lGL \
           -lpthread \
           -m64
endif

# vtk
VTK_INC = $(VTK_HOME)/include/vtk
VTK_LIB = -lglut -L$(VTK_HOME)/lib/vtk \
	-lvtkGraphics -lvtkRendering -lvtkftgl -lvtkImaging -lvtkFiltering -lvtkCommon

# build compiler flags

C_INC = -I$(SRC_DIR) \
	$(SVM_INC) \
	$(INC_SUB_DIRS) \
	$(GSL_INC) \
	$(ACE_INC) \
	$(ACE_FLAGS) \
	$(TINYXML_FLAGS) \
	$(VXL_INC) \
	$(NIFTI_INC) \
	$(BOOST_INC)

C_FLAGS = -Wall \
	-Wno-write-strings \
	-std=c++20 \
	$(FRONT_FLAG) \
	$(MTRACE_FLAG) \
	$(PROF_FLAG) \
	$(DEBUG_FLAG) \
	$(OPTIM_FLAG) \
	$(STRIP_FLAG) \
	$(C_INC)

C_LIB = $(MATH_LIB) \
	$(SVM_LIB) \
	$(GSL_LIB) \
	$(ACE_LIB) \
	$(QT_LIB) \
	$(VXL_LIB) \
	$(NIFTI_LIB) \
	$(BOOST_LIB)

LDFLAGS = $(PROF_FLAG) $(C_LIB)

# export vars for sub-makes
export C_FLAGS
export C_INC
export C_LIB
export LD_FLAGS

############################### SUFFIXES ##############################

OBJ_FILES = $(wildcard $(OBJ_DIR)/*.o)

############################## TARGETS ###############################

default: $(PROJECT)

install:
	install $(INSTALL_BINARY) $(INSTALL_DIR)

debug:
	$(MAKE) DEBUG=1 OPTIM=0 STRIP=0 $(subst debug,$(BLANK),$(MAKECMDGOALS))

profile:
	$(MAKE) DEBUG=1 PROF=1 OPTIM=0 STRIP=0 $(PROJECT)

headless:
	$(MAKE) FRONTEND=0 $(PROJECT)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

$(PROJECT): dirs $(OBJ_FILES)
	@$(ECHO) 'make: building $@ for $(OS)...'
	cd $(SRC_DIR) && $(MAKE)
	cd util/scanner_sim && $(MAKE)
	$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(BIN_DIR)/$(PROJECT) $(LDFLAGS)
	@$(ECHO) '############################################'
	@$(ECHO) 'make: built [$@] successfully!'
	@$(ECHO) '############################################'

############################### CLEAN ################################

clean:
	@$(ECHO) 'make: removing object and autosave files'
	-cd $(SRC_DIR) && $(MAKE) clean
	-cd $(OBJ_DIR) && $(RM) -f *.o *~
	-cd util/scanner_sim && $(MAKE) clean

clean_qt:
	-cd src/qt && make clean

######################################################################
### $Source$
### Local Variables:
### mode: makefile
### fill-column: 76
### comment-column: 0
### End:
