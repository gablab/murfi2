##########################################################################
# 
# makefile in the src dir for the real-time fMRI system
# all this does is tell each subdirectory to make itself
#
# Oliver Hinds <ohinds@mit.edu> 2007-08-14
#
##########################################################################

default: 
	cd ./executive && $(MAKE)
	cd ./io &&        $(MAKE)
	cd ./data &&      $(MAKE)
	cd ./stream &&    $(MAKE)
	cd ./display &&   $(MAKE)

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
