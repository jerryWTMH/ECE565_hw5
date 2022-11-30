TARGETS=rainfall_seq

all: $(TARGETS)

rainfall_seq: simulator_sequential.cpp simulator_sequential.hpp
	g++ -O3 -std=c++11 -o simulator_seq simulator_sequential.cpp

clean:
	rm -rf $(TARGETS)