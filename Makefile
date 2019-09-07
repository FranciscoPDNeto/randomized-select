CC := g++
CFLAGS := -Wall -O3 -std=c++17 -fopenmp
VENDOR := -I vendor

all: bin/sequential bin/parallel

bin/sequential: build/sequential.o build/main.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^
bin/parallel: build/parallel.o build/main.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

test-sequential: bin/test-sequential
	./bin/test-sequential
test-parallel: bin/test-parallel
	./bin/test-parallel

bin/test-sequential: build/sequential.o build/test.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^
bin/test-parallel: build/parallel.o build/test.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.cpp
	mkdir -p build
	$(CC) $(CFLAGS) $(VENDOR) -c -o $@ $<

clean:
	rm bin/* build/*
