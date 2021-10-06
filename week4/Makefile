TARGET=ex4
CXX=gcc
CXXFLAGS=-Wall
OBJECT=ex4.o
$(TARGET) : $(OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^
ex4.o: ex4.c
	$(CXX) $(CXXFLAGS) -c $^
clean: 
	rm $(OBJECT) $(TARGET)
