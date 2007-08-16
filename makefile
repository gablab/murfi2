##########################################################################
# 
# master makefile for the real-time fMRI system
#
# Oliver Hinds <ohinds@mit.edu> 2007-08-14
#
##########################################################################

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


export C_INCS = -I/usr/include/$(SIGC_HOME) -I$(SRC_DIR)
export C_LIBS = -lm

export PKGCONFIG_FLAGS = --cflags --libs sigc++-2.0
export PKGCONFIG_CMD = pkg-config $(PKGCONFIG_FLAGS)

export C_FLAGS = -Werror -Wall \
	$(MTRACE_FLAG) $(PROF_FLAG) $(DEBUG_FLAG) $(OPTIM_FLAG) $(STRIP_FLAG) \
	$(CINCL) `$(PKGCONFIG_CMD)`

export LDFLAGS = $(PROF_FLAG) -lm


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
