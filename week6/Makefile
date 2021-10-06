CXX=gcc
CXXFLAGS=-Wall
TARGET=crond
OBJECT=6_cron.o
$(TARGET) : $(OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^
6_cron.o: 6_cron.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECT) $(TARGET)
