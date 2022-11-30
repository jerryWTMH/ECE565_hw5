TARGETS=simulator_seq jerry

all: $(TARGETS)

simulator_seq: simulator_sequential.c simulator_sequential.h
	gcc -O3 -o simulator_seq simulator_sequential.c 

jerry: jerry.c
	gcc -O3 -o jerry jerry.c

clean:
	rm -rf $(TARGETS)