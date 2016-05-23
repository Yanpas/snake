CC=g++
CFLAGS=-c -g -std=c++11
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SOURCES=main.cpp Field.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=snake



all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *.o snake
