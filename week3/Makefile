TARGET=w3.out
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=minus.o main.o plus.o
$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
main.o: main.c
	$(CXX) $(CXXFLAGS) -c $^
minus.o: minus.c
	$(CXX) $(CXXFLAGS) -c $^
plus.o: plus.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)
