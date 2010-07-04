
CC=gcc
CFLAGS=-g
LDFLAGS=-lpthread

ALL_TARGETS=ping_pong
PING_PONG_SRC=ping_pong.o

all: ping_pong 

ping_pong: $(PING_PONG_SRC)
	$(CC) -o $@ $^ $(LDFLAGS)


%o.%c:
	$(CC) $(CFLAGS) $^ 

clean:
	rm -f *.o
	rm -f $(ALL_TARGETS) 
