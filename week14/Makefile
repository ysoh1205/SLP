CXX = gcc
CFLAG1 = -Wall
CFLAG2 = -lpthread
TARGET = e14
OBJECT = ex14.c

$(TARGET) : $(OBJECT)
	$(CXX) $(CFLAG1) -o $@ $^ $(CFLAG2)

clean:
	rm $(TARGET)