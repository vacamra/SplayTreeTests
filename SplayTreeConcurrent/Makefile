
PROGS=main
CPPFILES=bias_data_test_shuffled.cpp BiasedDataTest.cpp clear.cpp erase.cpp everything.cpp main.cpp parallel_orig_performance.cpp parallel_performance.cpp serial.cpp serial_biased.cpp singleThread.cpp test_parallel_stupid.cpp
HEADERS=splayset_serial.hpp splayset_concurrent.hpp stopwatch.hpp rc4prng.hpp
CXXFLAGS=-std=c++11 -O3
#CXXFLAGS=-std=c++11 -g
LDFLAGS=-pthread
WALL=-Wall
TBBFLAGS=-L/usr/lib/ -ltbb

all: $(PROGS)

#$(PROGS): %: %.cpp $(HEADERS)
#	g++ $(CXXFLAGS) $(LDFLAGS) $@.cpp -o $(CPPFILES) $(TBBFLAGS)

main:
	g++ $(CXXFLAGS) $(LDFLAGS) $(WALL) -o SplayTreeConcurrent $(CPPFILES) $(TBBFLAGS)

clean:
	rm serial parallel
