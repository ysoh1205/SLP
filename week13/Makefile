CXX = gcc
CFLAGS = -Wall
CFLAG2 = -lpthread
TARGET = ex13
OBJECTS = ex13.c
$(TARGET) : $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^ $(CFLAG2)
clean:
	rm $(TARGET)