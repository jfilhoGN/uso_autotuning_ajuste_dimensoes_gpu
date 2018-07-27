#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>
#include <math.h>

#include "../../../dimensions.h"

#ifndef DATA_TYPE
#define DATA_TYPE float
#endif

#define CHECK(call)                                           \
{                                                             \
const cudaError_t error = call;                               \
if (error != cudaSuccess)                                     \
{                                                             \
fprintf(stderr, "Error: %s:%d, ", __FILE__, __LINE__);        \
fprintf(stderr, "code: %d, reason: %s\n", error,              \
cudaGetErrorString(error));                                   \
}                                                             \
}

/*__device__ int getGlobalIdx_1D_1D_x() {
    // Operações -> multiply: 1 add: 1 (2 FLOPs).
    // printf("getGlobalIdx_1D_1D.\n");
    return blockIdx.x * blockDim.x + threadIdx.x;
}

__device__ int getGlobalIdx_1D_1D_y() {
    // Operações -> multiply: 1 add: 1 (2 FLOPs).
    // printf("getGlobalIdx_1D_1D.\n");
    return blockIdx.y * blockDim.y + threadIdx.y;
}

__device__ int getGlobalIdx_1D_1D_z() {
    // Operações -> multiply: 1 add: 1 (2 FLOPs).
    // printf("getGlobalIdx_1D_1D.\n");
    return blockIdx.z * blockDim.z + threadIdx.z;
}

__device__ int getGlobalIdx_1D_2D() {
    // Operações -> multiply: 3 add: 2 (5 FLOPs).
    // printf("getGlobalIdx_1D_2D.\n");
    return blockIdx.x * blockDim.x * blockDim.y + threadIdx.y * blockDim.x
            + threadIdx.x;
}
__device__ int getGlobalIdx_1D_3D() {
    // Operações -> multiply: 6 add: 3 (9 FLOPs).
    // printf("getGlobalIdx_1D_3D.\n");
    return blockIdx.x * blockDim.x * blockDim.y * blockDim.z
            + threadIdx.z * blockDim.y * blockDim.x + threadIdx.y * blockDim.x
            + threadIdx.x;
}
__device__ int getGlobalIdx_2D_1D() {
    // Operações -> multiply: 2 add: 2 (4 FLOPs).
    // printf("getGlobalIdx_2D_1D.\n");
    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;
    return threadId;
}
__device__ int getGlobalIdx_2D_2D() {
    // Operações -> multiply: 4 add: 3 (7 FLOPs).
    // printf("getGlobalIdx_2D_2D.\n");
    int blockId = blockIdx.x + blockIdx.y * gridDim.x;
    int threadId = blockId * (blockDim.x * blockDim.y)
            + (threadIdx.y * blockDim.x) + threadIdx.x;
    return threadId;
}
__device__ int getGlobalIdx_2D_3D() {
    // Operações -> multiply: 7 add: 4 (11 FLOPs).
    // printf("getGlobalIdx_2D_3D.\n");
    int blockId = blockIdx.x + blockIdx.y * gridDim.x;
    int threadId = blockId * (blockDim.x * blockDim.y * blockDim.z)
    + (threadIdx.z * (blockDim.x * blockDim.y)) + (threadIdx.y * blockDim.x)
            + threadIdx.x;
    return threadId;
}
__device__ int getGlobalIdx_3D_1D() {
    // Operações -> multiply: 4 add: 3 (7 FLOPs).
    // printf("getGlobalIdx_3D_1D.\n");
    int blockId = blockIdx.x + blockIdx.y * gridDim.x
            + gridDim.x * gridDim.y * blockIdx.z;
    int threadId = blockId * blockDim.x + threadIdx.x;
    return threadId;
}
__device__ int getGlobalIdx_3D_2D() {
    // Operações -> multiply: 6 add: 4 (10 FLOPs).
    // printf("getGlobalIdx_3D_2D.\n");
    int blockId = blockIdx.x + blockIdx.y * gridDim.x
            + gridDim.x * gridDim.y * blockIdx.z;
    int threadId = blockId * (blockDim.x * blockDim.y)
            + (threadIdx.y * blockDim.x) + threadIdx.x;
    return threadId;
}
__device__ int getGlobalIdx_3D_3D() {
    // Operações -> multiply: 9 add: 5 (14 FLOPs).
    // printf("getGlobalIdx_3D_3D.\n");
    int blockId = blockIdx.x + blockIdx.y * gridDim.x
            + gridDim.x * gridDim.y * blockIdx.z;
    int threadId = blockId * (blockDim.x * blockDim.y * blockDim.z)
            + (threadIdx.z * (blockDim.x * blockDim.y))
            + (threadIdx.y * blockDim.x) + threadIdx.x;
    return threadId;
}

/* Tipo para o ponteiro de função. 
typedef int (*op_func) (void);

/* Tabela de funções para chamada parametrizada. 
__device__ op_func getGlobalIdFunc[9] = { getGlobalIdx_1D_1D, getGlobalIdx_1D_2D, getGlobalIdx_1D_3D, 
                      getGlobalIdx_2D_1D, getGlobalIdx_2D_2D, getGlobalIdx_2D_3D,
                      getGlobalIdx_3D_1D, getGlobalIdx_3D_2D, getGlobalIdx_3D_3D};*/

void init_arrays(DATA_TYPE* a, DATA_TYPE* b, int n){
    int i;
    // double invrmax = 1.0 / RAND_MAX;
    for (i = 0; i < n; i++) {
        // a[i] = rand() * invrmax;
        // b[i] = rand() * invrmax;
        a[i] = 0.5;
        b[i] = 0.5;
    }
}

__global__ void vecAdd(DATA_TYPE *a, DATA_TYPE *b, DATA_TYPE *c, int n, int funcId){
    //Thread ID
    int id = getGlobalIdFunc[funcId]();
    printf("id: %d\n", id);
    if (id < n)
        c[id] = a[id] + b[id];
}
 
int main(int argc, char **argv){
    int i;
    cudaError_t err;
    int n = 0;
    int kernel = 0;
    int funcId = 0;
    int gpuId = 0;
 	
 	if (argc != 10) {
        printf("Uso: %s <kernel> <g.x> <g.y> <g.z> <b.x> <b.y> <b.z> <n> <gpuId>\n", argv[0]);
        /*printf("     funcId:\n");
        printf("     0: 1D_1D, 1: 1D_2D, 2: 1D_3D\n");
        printf("     3: 2D_1D, 4: 2D_2D, 5: 2D_3D\n");
        printf("     6: 3D_1D, 7: 3D_2D, 8: 3D_3D\n");*/
        return 0;
    }
    else{
        printf("#argumentos (argc): %d\n", argc);
        for (i = 0; i < argc; ++i) {
           printf(" argv[%d]: %s\n", i, argv[i]);
        }
    
        kernel = atoi(argv[1]);
        n = atoi(argv[8]);
        //funcId = atoi(argv[9]);
        gpuId = atoi(argv[9]);
        printf("Executando: %s sumvector_kernel_%d grid(%d, %d, %d) block(%d, %d, %d) %d\n", argv[0], kernel, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), n);
    }
  
    /* Recuperar as informações da GPU. */
    printf("%s Starting...\n", argv[0]);
  
    int deviceCount = 0;
    cudaGetDeviceCount(&deviceCount);
  
    if (deviceCount == 0) {
        printf("Não existem dispositivos com suporte a CUDA.\n");
        return 0;
    } else {
        printf("Existem %d dispositivos com suporte a CUDA.\n", deviceCount);
        if(gpuId > (deviceCount - 1)){
            printf("Não existe um dispositivo sob o id: %d. Utilize %d a %d\n", gpuId, 0, (deviceCount - 1));
            return 0;
        }
    }
    /* Define the gpu id to work */
    cudaSetDevice(gpuId);

    /* Alocação das estruturas. */
    // Size, in bytes, of each vector
    size_t bytes = sizeof(DATA_TYPE) * n;
    printf(" sizeof(DATA_TYPE): %d\n", (int) sizeof(DATA_TYPE));
    size_t totalmem = (3 * bytes);
    printf(" Qtd bytes por estrutura: %zu total: %zu\n", bytes, totalmem);
  
    /* Dados no host. */
    printf("Allocate memory for each vector on host.\n");
    DATA_TYPE *h_a = (DATA_TYPE*) malloc(bytes);
    DATA_TYPE *h_b = (DATA_TYPE*) malloc(bytes);
    DATA_TYPE *h_c = (DATA_TYPE*) malloc(bytes);
  
    /* Dados no dispositivo. */
    DATA_TYPE *d_a;
    DATA_TYPE *d_b;
    // Device output vector.
    DATA_TYPE *d_c;
 
    printf("Allocate memory for each vector on GPU.\n");
    // Allocate memory for each vector on GPU
    cudaMalloc(&d_a, bytes);
    cudaMalloc(&d_b, bytes);
    cudaMalloc(&d_c, bytes);
 
    init_arrays(h_a, h_b, n);
  
    printf("Copy host vectors to device.\n");
    // Copy host vectors to device
    cudaMemcpy(d_a, h_a, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, bytes, cudaMemcpyHostToDevice);

    /* Definição do arranjo de threads em blocos do grid. */
    dim3 grid(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    dim3 block(atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

    funcId = calculateFunctionId(grid, block);
    printf("funcId: %d\n", funcId);

    printf("Execute the kernel.\n");
    cudaEvent_t start_event, stop_event;
    float time_kernel_execution;
    int eventflags = cudaEventBlockingSync;
    cudaEventCreateWithFlags(&start_event, eventflags);
    cudaEventCreateWithFlags(&stop_event, eventflags);

    /* O kernel e a função de calculo do id global são escolhidos conforme o parâmetros.*/
    switch (kernel){
        case 0:
            printf("Executing sumvector_kernel_%d.\n", kernel);
            vecAdd<<<grid, block>>>(d_a, d_b, d_c, n, funcId);
        break;
        default :
            printf("Invalid kernel number.\n");
    }

    err = cudaGetLastError();
  
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to launch kernel (error code %s)!\n",
        cudaGetErrorString(err));
        exit (EXIT_FAILURE);
    }
    /* Synchronize */
    cudaDeviceSynchronize();
  
    cudaEventRecord(stop_event, 0);
    cudaEventSynchronize(stop_event);
    cudaEventElapsedTime(&time_kernel_execution, start_event, stop_event);
    printf("Time Kernel Execution: %f s\n", (time_kernel_execution / 1000.0f));
    printf("Time Kernel Execution: %f ms\n", (time_kernel_execution));
  
    printf("Copy array back to host.\n");
    // Copy array back to host
    cudaMemcpy(h_c, d_c, bytes, cudaMemcpyDeviceToHost);
    // Release device memory
  
    printf("Liberando as estruturas alocadas na Memória da GPU.\n");
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    DATA_TYPE soma,media;
    soma = 0;
    for(i=0; i<n; i++){
        soma = soma + h_c[i];
        
        //printf("resultado: %f\n", h_c[n]);
    }
    media = soma / n;
    printf("resultado: %f\n", media);
    printf("Liberando as estruturas alocadas na Memória do host.\n");
    free(h_a);
    free(h_b);
    free(h_c);
  
    printf("Reset no dispositivo.\n");
    CHECK(cudaDeviceReset());
  
    printf("Done.\n");
  
    return 0;
}
