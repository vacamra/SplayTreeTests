
PROGS=serial parallel
HEADERS=splayset.hpp stopwatch.hpp rc4prng.hpp
CXXFLAGS=-std=c++11 -O3
#CXXFLAGS=-std=c++11 -g
LDFLAGS=-pthread

all: $(PROGS)

$(PROGS): %: %.cpp $(HEADERS)
	g++ $(CXXFLAGS) $(LDFLAGS) $@.cpp -o $@

clean:
	rm serial parallel
