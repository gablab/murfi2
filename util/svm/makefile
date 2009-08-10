# makefile for the adaptation of svm-light for fmri data stored in nifti files
#
# Oliver Hinds <ohinds@mit.edu> 2009-07-20

CC = g++

BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src

SVM_LIGHT_DIR = svm-light
NIFTI_DIR = /usr

INCL_DIRS = -I$(SVM_LIGHT_DIR) -I$(NIFTI_DIR)/include -I$(NIFTI_DIR)/include/nifti

SRC_FILES = \
	$(SVM_LIGHT_DIR)/svm_common.c \
	$(SVM_LIGHT_DIR)/svm_learn.c \
	$(SVM_LIGHT_DIR)/svm_hideo.c \
	$(SRC_DIR)/fmri_svm-light_common.cpp 

LIBS = -lniftiio

DEBUG_FLAG = -g
OPTIM_FLAG =

CPP_FLAGS = $(DEBUG_FLAG) $(OPTIM_FLAG) $(INCL_DIRS)

LD_FLAGS = $(LIBS)

.SUFFIXES:
.SUFFIXES:  .o .cpp .c

# suffix rule for subsidiary source files
%.o: %.cpp 
	@echo '[make: building $@]'
	$(CC) $(CPP_FLAGS) -o $@ -c $<

%.o: %.c %.h
	@echo '[make: building $@]'
	$(CC) $(CPP_FLAGS) -o $@ -c $<

TMP_FILES = $(SRC_FILES:.cpp=.o)
OBJ_FILES = $(TMP_FILES:.c=.o)

default: fmri_svm-light_train fmri_svm-light_test

fmri_svm-light_train: $(SRC_DIR)/fmri_svm-light_train.cpp $(SRC_FILES) $(OBJ_FILES) 
	$(CC) $(CPP_FLAGS) $(SRC_DIR)/$@.cpp $(OBJ_FILES) -o $(BIN_DIR)/$@ $(LD_FLAGS)

fmri_svm-light_test: $(SRC_DIR)/fmri_svm-light_test.cpp $(SRC_FILES) $(OBJ_FILES) 
	$(CC) $(CPP_FLAGS) $(SRC_DIR)/$@.cpp $(OBJ_FILES) -o $(BIN_DIR)/$@ $(LD_FLAGS)


clean:
	-rm $(OBJ_FILES)

nifti2svmexample: nifti2svmexample.cpp
	g++ nifti2svmexample.cpp -l niftiio -o $(BIN_DIR)/nifti2svmexample $(CPP_FLAGS)