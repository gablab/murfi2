##########################################################################
#
# makefile in the src dir for the real-time fMRI system
# all this does is tell each subdirectory to make itself
#
# Oliver Hinds <ohinds@mit.edu> 2007-08-14
#
##########################################################################

CFLAGS += $(FRONT_FLAG)

ifeq ($(FRONTEND),1)
  MAKE_FRONT=cd ./display && $(MAKE)
else
  MAKE_FRONT=echo not compiling frontend
endif

default:
	cd ../util/svm && $(MAKE) obj_files
	cd ./executive && $(MAKE)
	cd ./io &&        $(MAKE)
	cd ./util &&      $(MAKE)
	cd ./data &&      $(MAKE)
	cd ./stream &&    $(MAKE)
	$(MAKE_FRONT)

clean:
	cd ../util/svm && $(MAKE) clean
	cd ./executive && $(MAKE) clean
	cd ./io &&        $(MAKE) clean
	cd ./util &&      $(MAKE) clean
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
