##########################################################################
# 
# makefile in the src dir for the real-time fMRI system
# all this does is tell each subdirectory to make itself
#
# Oliver Hinds <ohinds@mit.edu> 2007-08-14
#
##########################################################################

CFLAGS += $(FRONT_FLAG)

ifeq ($(APP),1)
  MAKE_APP=cd ./app && $(MAKE)
else
  MAKE_APP=echo not compiling app
endif

ifeq ($(FRONTEND),1)
  MAKE_FRONT=cd ./frontend && $(MAKE)
else
  MAKE_FRONT=echo not compiling frontend
endif

ifeq ($(OLDFRONTEND),1)
  MAKE_OLDFRONT=cd ./display && $(MAKE)
else
  MAKE_OLDFRONT=echo not compiling old frontend
endif

default: 
	cd ../util/svm && $(MAKE) obj_files
	cd ./executive && $(MAKE)
	cd ./io &&        $(MAKE)
	cd ./util &&      $(MAKE)
	cd ./data &&      $(MAKE)
	cd ./stream &&    $(MAKE)
	$(MAKE_APP)
	$(MAKE_FRONT)
	$(MAKE_OLDFRONT)

clean: 
	cd ../util/svm && $(MAKE) clean
	cd ./executive && $(MAKE) clean
	cd ./io &&        $(MAKE) clean
	cd ./util &&      $(MAKE) clean
	cd ./data &&      $(MAKE) clean
	cd ./stream &&    $(MAKE) clean
	cd ./app &&       $(MAKE) clean
	cd ./display &&   $(MAKE) clean
	cd ./frontend &&  $(MAKE) clean



######################################################################
### $Source$
### Local Variables:
### mode: makefile
### fill-column: 76
### comment-column: 0
### End: 
