# CUDA Paths.
CUDA_HOME=/usr/local/cuda

# Compilador.
CC=gcc-4.9
CXX=g++-4.9
NVCC=$(CUDA_HOME)/bin/nvcc

# Flags.
FLAGS=-lcuda -lm
CCFLAGS := -I/usr/local/cuda/include
LDFLAGS := -L/usr/local/cuda/lib64

all: compile cuda

compile:
	$(CC) -mcmodel=large -O0 -Wall -fmessage-length=0 -o sincosc src/sincosc.c -lm

compile-2:
	$(CC) -mcmodel=large -O0 -Wall -fmessage-length=0 -c -o sincosc.o src/sincosc.c
	$(CC) -o sincosc sincosc.o -lm

cuda:
	$(NVCC) $(CCFLAGS) $(LDFLAGS) -ccbin $(CXX)  src/sincosc.cu -o sincosc-cuda $(FLAGS)
	# /home/goncalv/cuda/bin/nvcc -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -ccbin g++-4.8  src/sincosc.cu -o sincosc-cuda -lcuda -lm
	
	
clean:
	rm sincosc sincosc-cuda *.o
