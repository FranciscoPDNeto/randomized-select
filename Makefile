CC := g++
CFLAGS := -Wall -O3 -std=c++17 -lpthread
VENDOR := -I vendor

all: bin/sequential bin/parallel

bin/sequential: build/sequential.o
bin/parallel: build/parallel.o
bin/sequential bin/parallel: build/PoolThread.o build/randomizedSelect.o build/main.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/test-sequential: build/sequential.o
bin/test-parallel: build/parallel.o
bin/test-sequential bin/test-parallel: build/PoolThread.o build/randomizedSelect.o build/test.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

test-sequential: bin/test-sequential
test-parallel: bin/test-parallel
test-sequential test-parallel:
	./$<

build/%.o: src/%.cpp
	mkdir -p build
	$(CC) $(CFLAGS) $(VENDOR) -c -o $@ $<

clean:
	rm bin/* build/*
