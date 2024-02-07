CPP := g++
CPPFLAGS := -Wall -Wextra -g


all: block.o utils.o events.o node.o main.o
	$(CPP) $(CPPFLAGS) -o blockSim main.o block.o utils.o events.o node.o

main.o: main.cpp utils.h node.h events.h Makefile
	$(CPP) $(CPPFLAGS) -c main.cpp

block.o: utils.h block.h block.cpp Makefile
	$(CPP) $(CPPFLAGS) -c block.cpp

utils.o: utils.h utils.cpp events.h Makefile
	$(CPP) $(CPPFLAGS) -c utils.cpp

events.o: events.h block.h utils.h events.cpp node.h Makefile
	$(CPP) $(CPPFLAGS) -c events.cpp

node.o: utils.h node.h node.cpp Makefile
	$(CPP) $(CPPFLAGS) -c node.cpp

clean:
	rm *.o || true
