##########################################################################
# 
# generic subdir makefile for the FRONTEND of real-time fMRI system
# just make all subdirs
#
# Alexey Sidorov <scopicsoftware> 2008-10-30
#
##########################################################################

############################## SUFFIXES ##############################

.DELETE_ON_ERROR:

.SUFFIXES:
.SUFFIXES:  .o .cpp

# suffix rule for subsidiary source files
# (see http://www.gnu.org/manual/make/html_chapter/make_10.html#SEC111)
$(OBJ_DIR)/%.o: %.cpp
	@$(ECHO) '[make: building $@]'
	$(CC) $(C_FLAGS) -o $@ -c $< 

SRC_FILES = $(wildcard ./*.cpp)
TMP_FILES = $(patsubst ./%,$(OBJ_DIR)/%,$(SRC_FILES))
OBJ_FILES = $(TMP_FILES:.cpp=.o) 

default: $(OBJ_FILES)

############################### CLEAN ################################

clean:
	@$(ECHO) 'make: removing object and autosave files'
	-$(RM) -f *.o *~ *_moc.cpp


######################################################################
### $Source$
### Local Variables:
### mode: makefile
### fill-column: 76
### comment-column: 0
### End: 
