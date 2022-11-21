CC = g++
CFLAGS = -Wall -std=c++14 -g
DEPS = header.h
OBJ = utils.o malloc.o realloc.o calloc.o memalign.o free.o test1.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ibfs: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

.PHONY: clean

clean:
	rm -rf *.o ibfs