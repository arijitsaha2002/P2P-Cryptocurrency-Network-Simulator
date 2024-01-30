CPP := g++
CPPFLAGS := -Wall -Wextra -g


block.o: utils.h block.h
	$(CPP) $(CPPFLAGS) -c block.cpp

graph: utils.o graph.o
	$(CPP) $(CPPFLAGS) -o graph graph.o utils.o

graph.o: utils.h
	$(CPP) $(CPPFLAGS) -c graph.cpp

utils.o: utils.h
	$(CPP) $(CPPFLAGS) -c utils.cpp

clean:
	rm *.o graph || true
