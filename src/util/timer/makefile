# Specify the compiler and compiler options
CC = g++
CCOPTS = -Wall -O

# Make the source
timertest : timertest.o timer.h
	$(CC) $(CCOPTS) timertest.o -o timertest

timertest.o : timertest.cc timer.h
	$(CC) $(CCOPTS) -c timertest.cc

clean :
	rm -f timertest.o timertest

