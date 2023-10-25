# makefile for three versions of wordcount

all: TraditionalWc.c ChildProcessingWc.c ThreadWc.C
TraditionalWc: TraditionalWc.c
	gcc -O2 TraditionalWc.c -o TraditionalWc

ChildProcessingWc: ChildProcessingWc.c
	gcc -O2 ChildProcessingWc.c -o ChildProcessingWc

ThreadWc: ThreadWc.C
	gcc -O2 ThreadWc.C -pthread -o ThreadWc
