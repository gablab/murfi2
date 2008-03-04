PROJECT=gnuplot_i_vxl

VXL_DIR=/usr/local

CFLAGS = -ggdb
DEFINES = -DDEBUGGA
INCLUDES = -I$(VXL_DIR)/include/vxl/core -I$(VXL_DIR)/include/vxl/vcl
LIBS = -lstdc++ -lvnl
OBJ = gnuplot_i.o
EXAMPLE = example.o gnuplot_i_vxl.o
CC=g++

MAJOR_VER=1
MINOR_VER=0
RELEASE_VER=0

INSTALL_TARGET=/usr/local
LIB_DIR=$(INSTALL_TARGET)/lib
HDR_DIR=$(INSTALL_TARGET)/include

.cpp.o:
	$(CC) -c $(CFLAGS) $(DEFINES) $(INCLUDES) $<

all::	shared

$(PROJECT).o:	$(PROJECT).cpp $(PROJECT).h
shared: $(PROJECT).o
	$(CC)  -shared  -Wl,-soname,$(PROJECT).so.$(MAJOR_VER) $(PROJECT).o -o lib$(PROJECT).so.$(MAJOR_VER).$(MINOR_VER).$(RELEASE_VER) $(LIBS) 
install: shared
	cp $(PROJECT).h $(HDR_DIR)
	cp lib$(PROJECT).so.$(MAJOR_VER).$(MINOR_VER).$(RELEASE_VER) $(LIB_DIR)
	ln -sf $(LIB_DIR)/lib$(PROJECT).so.$(MAJOR_VER).$(MINOR_VER).$(RELEASE_VER) $(LIB_DIR)/lib$(PROJECT).so.$(MAJOR_VER)
	ln -sf $(LIB_DIR)/lib$(PROJECT).so.$(MAJOR_VER).$(MINOR_VER).$(RELEASE_VER) $(LIB_DIR)/lib$(PROJECT).so
	ldconfig -n $(LIB_DIR)	

example.o:		example.cpp gnuplot_i_vxl.cpp
example: $(EXAMPLE)
	$(CC) -o $@ $(CFLAGS) $(EXAMPLE) $(LIBS)

clean:
	-rm *.o
	-rm *.so.*
	-rm *~

