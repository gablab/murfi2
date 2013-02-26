# makefile for the adaptation of svm-multiclass for fmri data stored
# in nifti files

CC = g++

BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src

SVM_DIR = svm-multiclass
NIFTI_DIR = /usr

INCL_DIRS = -I$(SVM_DIR) -I$(NIFTI_DIR)/include -I$(NIFTI_DIR)/include/nifti

SRC_FILES = \
	$(SRC_DIR)/fmri_svm_common.cpp \
	$(SVM_DIR)/svm_struct_api.c



LIBS = -lniftiio

DEBUG_FLAG = -g
OPTIM_FLAG =

CPP_FLAGS = $(DEBUG_FLAG) $(OPTIM_FLAG) $(INCL_DIRS) -Wno-write-strings -Wall

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

SVM_OBJ_FILES = $(SVM_DIR)/svm_struct/svm_struct_learn.o \
		$(SVM_DIR)/svm_struct/svm_struct_common.o \
		$(SVM_DIR)/svm_light/svm_common.o \
		$(SVM_DIR)/svm_light/svm_learn.o \
		$(SVM_DIR)/svm_light/svm_hideo.o
#		$(SVM_DIR)/svm_struct/svm_struct_classify.o \

SVM_LIB = $(SVM_OBJ_FILES)

default: fmri_svm_train fmri_svm_test
obj_files: fmri_svm_train_obj fmri_svm_test_obj

fmri_svm_train_obj: $(SRC_FILES) $(OBJ_FILES) svm_learn_hideo_noexe svm_struct_noexe

fmri_svm_train: $(SRC_DIR)/fmri_svm_train.cpp $(SRC_FILES) $(OBJ_FILES) svm_learn_hideo_noexe svm_struct_noexe
	$(CC) $(CPP_FLAGS) $(SRC_DIR)/$@.cpp $(OBJ_FILES) $(SVM_LIB) -o $(BIN_DIR)/$@ $(LD_FLAGS)

fmri_svm_test_obj: $(SRC_FILES) $(OBJ_FILES) svm_learn_hideo_noexe svm_struct_noexe

fmri_svm_test: $(SRC_DIR)/fmri_svm_test.cpp $(SRC_FILES) $(OBJ_FILES) svm_learn_hideo_noexe svm_struct_noexe
	$(CC) $(CPP_FLAGS) $(SRC_DIR)/$@.cpp $(OBJ_FILES) $(SVM_LIB) -o $(BIN_DIR)/$@ $(LD_FLAGS)


svm_learn_hideo_noexe:
	cd $(SVM_DIR)/svm_light; make svm_learn_hideo_noexe

svm_light_clean:
	cd $(SVM_DIR)/svm_light; make clean

svm_struct_noexe:
	cd $(SVM_DIR)/svm_struct; make svm_struct_noexe

svm_struct_clean:
	cd $(SVM_DIR)/svm_struct; make clean


clean:
	-rm $(OBJ_FILES) $(SVM_OBJ_FILES);
	cd $(SVM_DIR) && $(MAKE) clean

nifti2svmexample: $(SRC_DIR)/nifti2svmexample.cpp
	g++ $(SRC_DIR)/nifti2svmexample.cpp -l niftiio -o $(BIN_DIR)/nifti2svmexample $(CPP_FLAGS)
