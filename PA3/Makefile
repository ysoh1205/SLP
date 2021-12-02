CXX = gcc
CFLAG1 = -Wall
CFLAG2 = -lpthread
TARGET = pa3.out
OBJECT = server.c

$(TARGET) : $(OBJECT)
	$(CXX) $(CFLAG1) -o $@ $^ $(CFLAG2)

clean:
	rm $(TARGET)
