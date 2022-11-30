TARGETS=rainfall_seq

all: $(TARGETS)

rainfall_seq: simulator_sequential.c simulator_sequential.h
	gcc -O3 -o simulator_seq simulator_sequential.c 

clean:
	rm -rf $(TARGETS)