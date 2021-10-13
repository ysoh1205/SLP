CXX=gcc
CXXFLAGS=-Wall
TARGET=ex7
OBJECTS=ex7.o
$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
ex7.o: ex7.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)
