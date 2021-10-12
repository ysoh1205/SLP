CXX = gcc
CXXFLAGS = -Wall
TARGET = assignment1.out
OBJECTS = 2020312416.o
$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
2020312416.o: 2020312416.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECTS) $(TARGET)
