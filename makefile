##########################################################################
# 
# master makefile for the real-time fMRI system
#
# Oliver Hinds <ohinds@mit.edu> 2007-08-14
#
##########################################################################

# os we build for
OS=linux

# project name
export PROJECT = realtime

# whether to compile with debug, optimize flags
export DEBUG = 0
export PROF = 0
export OPTIM = 1
export STRIP = 1

export MTRACE = 0

export SRC_DIR = $(PWD)/src
export BIN_DIR = $(PWD)/bin
export OBJ_DIR = $(PWD)/obj

################################ APPS ################################

export RM = /bin/rm -v
export ECHO = /bin/echo
export CC = /usr/bin/g++
export PKGCONFIG = /usr/bin/pkg-config

################################ LIBS ################################

export SIGC_HOME=/usr/include/sigc++-2.0
export CCPP_LIBS=-lccgnu2 -lccext2 -ldl -lpthread

################################ FLAG ################################

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
	OPTIM_FLAG = -O3
endif

# optimize flag
ifeq ($(STRIP),1)
	STRIP_FLAG = -s
endif

# mtrace flag
ifeq ($(MTRACE),1)
	MTRACE_FLAG = -DMTRACE
endif

SUB_DIRS = -I$(SRC_DIR)/executive -I$(SRC_DIR)/data -I$(SRC_DIR)/io -I$(SRC_DIR)/stream -I$(SRC_DIR)/analysis -I$(SRC_DIR)/display

# library flags

# math

MATH_LIB = -lm 

GSL_INCS=-I/usr/include/gsl
GSL_LIB=-lgsl -lgslcblas

#DATETIME_LIB = -lboost_date_time
#THREAD_LIB = -lboost_thread
#SIGNALS_LIB = -lboost_signals -lsigc-2.0 
#SOCKETS_LIB = $(CCPP_LIBS)

#ACE_HOME=/usr/local/ACE_wrappers
#ACE_INCS=-I$(ACE_HOME)/ace
#ACE_LIB=-L$(ACE_HOME)/lib -lACE
ACE_LIB=-lACE

GLUT_LIB=-lglut

# build compiler flags

export C_INCS = -I$(SRC_DIR) $(SUB_DIRS) $(GSL_INCS) $(ACE_INCS) 

export C_FLAGS = -Wall \
	$(MTRACE_FLAG) $(PROF_FLAG) $(DEBUG_FLAG) $(OPTIM_FLAG) $(STRIP_FLAG) \
	$(C_INCS) 
#`$(PKGCONFIG_CMD)`

export C_LIBS = $(MATH_LIB) $(GSL_LIB) $(ACE_LIB) $(GLUT_LIB)

export PKGCONFIG_FLAGS = --cflags --libs sigc++-2.0
export PKGCONFIG_CMD = pkg-config $(PKGCONFIG_FLAGS)

export LDFLAGS = $(PROF_FLAG) $(C_LIBS)


############################### SUFFIXES ##############################

OBJ_FILES = $(wildcard $(OBJ_DIR)/*.o)

############################## TARGETS ###############################

default: $(PROJECT)
all:     $(PROJECT)
debug:	 
	$(MAKE) DEBUG=1 OPTIM=0 STRIP=0 $(PROJECT)
profile:	 
	$(MAKE) DEBUG=1 PROF=1 OPTIM=0 STRIP=0 $(PROJECT)

setdebug:
	export DEBUG = 1
	export OPTIM 0
	export STRIP 0

$(PROJECT): $(OBJ_FILES)
	@$(ECHO) 'make: building $@ for $(OS)...'
	cd $(SRC_DIR) && $(MAKE)
	$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(BIN_DIR)/$(PROJECT) $(LDFLAGS) 
	@$(ECHO) '############################################'
	@$(ECHO) 'make: built [$@] successfully!'
	@$(ECHO) '############################################'

############################### CLEAN ################################

clean:
	@$(ECHO) 'make: removing object and autosave files'
	cd $(SRC_DIR) && $(MAKE) clean
	-cd $(OBJ_DIR) && $(RM) -f *.o *~

######################################################################
### $Source$
### Local Variables:
### mode: makefile
### fill-column: 76
### comment-column: 0
### End: 
