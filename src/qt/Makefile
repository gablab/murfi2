TARGET = murfi_qt

default: $(TARGET)

QT_BIN = /usr/bin
ifeq ("$(wildcard $(QT_BIN)/uic)","")
	QT_BIN = /usr/lib/x86_64-linux-gnu/qt5/bin
endif

QT_INC_DIR = /usr/include/qt
ifeq ("$(wildcard $(QT_INC_DIR)/QtCore)","")
	QT_INC_DIR = /usr/include/x86_64-linux-gnu/qt5
endif

QT_INC =  -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 \
          -I$(QT_INC_DIR) \
          -I$(QT_INC_DIR)/QtWidgets \
          -I$(QT_INC_DIR)/QtGui \
          -I$(QT_INC_DIR)/QtCore \
          -I$(QT_INC_DIR)/QtOpenGL \
          -I$(QT_INC_DIR)/QtPrintSupport \
          -I.

MOC_INC = -I/usr/include/c++/4.8 \
          -I/usr/include/x86_64-linux-gnu/c++/4.8 \
          -I/usr/include/c++/4.8/backward \
          -I/usr/lib/gcc/x86_64-linux-gnu/4.8/include \
          -I/usr/local/include \
          -I/usr/lib/gcc/x86_64-linux-gnu/4.8/include-fixed \
          -I/usr/include/x86_64-linux-gnu \
          -I/usr/include

QT_DEF = -D_REENTRANT \
         -DQT_QML_DEBUG \
         -DQT_DECLARATIVE_DEBUG \
         -DQT_WIDGETS_LIB \
         -DQT_GUI_LIB \
         -DQT_CORE_LIB

.DELETE_ON_ERROR:

.SUFFIXES:
.SUFFIXES:  .o .cpp

$(OBJ_DIR)/%.o: %.cpp ui_MainWindow.h
	@$(ECHO) '[make: building $@]'
	$(CC) $(C_FLAGS) -o $@ -c -m64 -pipe -Wall -fPIC $(QT_DEF) $(QT_INC) $<

HDR_FILES = $(wildcard *.h)
SRC_FILES = $(wildcard ./*.cpp)
TMP_FILES = $(patsubst ./%,$(OBJ_DIR)/%,$(SRC_FILES))
OBJ_FILES = $(TMP_FILES:.cpp=.o)

TO_MOC = $(patsubst %,moc_%,$(HDR_FILES))
MOC_FILES = $(patsubst %.h,%.cpp,$(TO_MOC))

ui_MainWindow.h: MainWindow.h MainWindow.ui
	$(QT_BIN)/uic ./MainWindow.ui -o ui_MainWindow.h

moc_%.cpp: %.h ui_MainWindow.h
	$(QT_BIN)/moc $(QT_DEF) $(QT_INC) $< -o $@
	sed -i 's/#include "PIE"/#include "$<"/' $@

$(TARGET): ui_MainWindow.h $(MOC_FILES) $(OBJ_FILES)
	echo $(MOC_FILES)

clean:
	-rm ui_* *.o
