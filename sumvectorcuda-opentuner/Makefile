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
	$(CC) -mcmodel=large -O0 -Wall -fmessage-length=0 -o sumvector src/sumvector.c -lm

compile-2:
	$(CC) -mcmodel=large -O0 -Wall -fmessage-length=0 -c -o sumvector.o src/sumvector.c
	$(CC) -o sumvector sumvector.o -lm

cuda:
	$(NVCC) $(CCFLAGS) $(LDFLAGS) -ccbin $(CXX)  src/sumvector.cu -o sumvector-cuda $(FLAGS)
	# /home/goncalv/cuda/bin/nvcc -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -ccbin g++-4.8  src/sumvector.cu -o sumvector-cuda -lcuda -lm
	
	
clean:
	rm sumvector sumvector-cuda *.o
