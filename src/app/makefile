##########################################################################
# 
# makefile for the app controller of real-time fMRI system
#
# Oliver Hinds <ohinds@mit.edu> 2010-10-15
#
##########################################################################


.DELETE_ON_ERROR:

.SUFFIXES:
.SUFFIXES:  .o .cpp

# suffix rule for subsidiary source files
# (see http://www.gnu.org/manual/make/html_chapter/make_10.html#SEC111)
$(OBJ_DIR)/%.o: %.cpp %.h
	@$(ECHO) '[make: building $@]'
	$(CC) $(C_FLAGS) -o $@ -c $< 

#HDR_FILES = $(wildcard *.h)
SRC_FILES = $(wildcard ./*.cpp)
TMP_FILES = $(patsubst ./%,$(OBJ_DIR)/%,$(SRC_FILES))
OBJ_FILES = $(TMP_FILES:.cpp=.o) 


default:  $(OBJ_FILES)
	cd ./moc	&&	$(MAKE) -f ./makefile.moc
	cd ./moc	&&	$(MAKE)

clean:
	@$(ECHO) 'make: removing object and autosave files'
	-$(RM) -f *.o *~
	cd ./moc	&&	$(MAKE) clean
