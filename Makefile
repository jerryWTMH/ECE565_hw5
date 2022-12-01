TARGETS=simulator_seq

all: $(TARGETS)

simulator_seq: simulator_sequential.cpp simulator_sequential.hpp
	g++ -O3 -o simulator_seq simulator_sequential.cpp 

clean:
	rm -rf $(TARGETS)