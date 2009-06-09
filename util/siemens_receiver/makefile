## Visual C++
#CC = cl
#CFLAGS = -GX -Os -G5
#LFLAGS = wsock32.lib /Fe$@
#OBJEXT = obj

## Cygwin/GNU C++
CC = g++
CFLAGS = -Wall -g
LFLAGS = -lwsock32 -o $@
OBJEXT = o


.SUFFIXES: .cpp .$(OBJEXT)

## Build rule for VC++ and BC++
.cpp.$(OBJEXT):
	$(CC) -c $(CFLAGS) $<

## Build rule for Cygwin/GNU C++
#.cpp.$(OBJEXT):
#	$(CC) -c $(CFLAGS) $< -o $*.o


#### Major targets #####################################################

default: siemens_receiver.exe

ctags:
	ctags *.cpp *.h

clean:
	del *.$(OBJEXT) *.bak

distclean: clean
	del *.exe *.swp

siemens_receiver.exe: receiver.$(OBJEXT) ExternalSenderImageInfo.$(OBJEXT)
	$(CC) receiver.$(OBJEXT)  ExternalSenderImageInfo.$(OBJEXT) $(LFLAGS)

receiver.$(OBJEXT): receiver.cpp ExternalSenderImageInfo.cpp ExternalSenderImageInfo.h

