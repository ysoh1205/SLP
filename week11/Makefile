CXX = gcc
CFLAGS = -Wall
CFLAG2 = -lpthread
TARGET = ex11
OBJECTS = ex11.c
$(TARGET) : $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^ $(CFLAG2)
clean:
	rm $(TARGET)
