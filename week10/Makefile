CXX = gcc
CFLAGS = -Wall
all: server client

server: server.c
	$(CXX) $(CFLAGS) -o $@ $^
client: client.c
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm $(all)
