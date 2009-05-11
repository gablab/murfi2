##########################################################################
# 
# makefile in the src dir for the real-time fMRI system
# all this does is tell each subdirectory to make itself
#
# Oliver Hinds <ohinds@mit.edu> 2007-08-14
#
##########################################################################

CFLAGS += $(FRONT_FLAG)

ifeq ($(OLDFRONTEND),1)
  MAKE_DISP=cd ./display && $(MAKE)
else
  MAK_DISP=echo not compiling old frontend
endif

default: 
	cd ./executive && $(MAKE)
	cd ./io &&        $(MAKE)
	cd ./data &&      $(MAKE)
	cd ./stream &&    $(MAKE)
	$(MAKE_DISP)

clean: 
	cd ./executive && $(MAKE) clean
	cd ./io &&        $(MAKE) clean
	cd ./data &&      $(MAKE) clean
	cd ./stream &&    $(MAKE) clean
	cd ./display &&   $(MAKE) clean



######################################################################
### $Source$
### Local Variables:
### mode: makefile
### fill-column: 76
### comment-column: 0
### End: 
