CC = g++
CFLAGS = -std=c++11 -pthread -Wall
HEADERS = AdvancedMatrix.hpp Barrier.hpp 
OBJECTS = main.o AdvancedMatrix.o Barrier.o

default: program

%.o: %.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

program: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	-rm -f  $(OBJECTS) program

