CPP := g++
CPPFLAGS := -Wall -Wextra -g


block.o: utils.h block.h block.cpp Makefile
	$(CPP) $(CPPFLAGS) -c block.cpp

graph: utils.o graph.o Makefile
	$(CPP) $(CPPFLAGS) -o graph graph.o utils.o

graph.o: utils.h graph.cpp Makefile
	$(CPP) $(CPPFLAGS) -c graph.cpp

utils.o: utils.h utils.cpp Makefile
	$(CPP) $(CPPFLAGS) -c utils.cpp

events.o: events.h block.h utils.h events.cpp Makefile
	$(CPP) $(CPPFLAGS) -c events.cpp

clean:
	rm *.o graph || true
