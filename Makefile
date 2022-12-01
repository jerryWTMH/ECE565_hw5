TARGETS=simulator_seq simulator_th

all: $(TARGETS)

simulator_seq: simulator_sequential.cpp simulator_sequential.hpp
	g++ -O3 -o simulator_seq simulator_sequential.cpp 

simulator_th: simulator_threaded.cpp simulator_threaded.hpp
	g++ -O3 -pthread -o simulator_th simulator_threaded.cpp 

clean:
	rm -rf $(TARGETS)