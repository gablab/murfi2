OBJ_DIR=../../obj
VXL_HOME = /usr
VXL_INC=-I$(VXL_HOME)/include/vxlinclude/vxlcoreinclude/vxlvnlinclude/vxlalgo \
	-I$(VXL_HOME)/include/vxlinclude/vxlcoreinclude/vxlvnl \
	-I$(VXL_HOME)/include/vxlinclude/vxlcoreinclude \
	-I$(VXL_HOME)/include/vxl/vcl \
	-I$(VXL_HOME)/include/vxl/core \
	-I$(VXL_HOME)/include/vxlinclude/vxlvcl

VXL_LIB=-lvnl_algo -lvcl -lv3p_netlib -lvnl -L$(VXL_HOME)/vxl/lib


default: servenii4d servenii servedicoms



servenii4d: servenii4d.cpp
	g++ -g -o servenii4d servenii4d.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -lniftiio -I/usr/include/nifti $(VXL_INC) $(VXL_LIB) -D_REENTRANT

servescanner: servescanner.cpp
	g++ -g -o servescanner servescanner.cpp -lACE

servedicoms: servedicoms.cpp
	g++ -g 	-std=c++20 -o servedicoms servedicoms.cpp

servenii: servenii.cpp
	g++ -g -o servenii servenii.cpp $(OBJ_DIR)/RtExternalSenderImageInfo.o -lACE -lniftiio -I/usr/include/nifti -D_REENTRANT -lX11

clean:
	rm servenii servenii4d servedicoms
