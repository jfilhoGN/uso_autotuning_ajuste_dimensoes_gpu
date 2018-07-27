#include <stdio.h>
#include <math.h>

#define MAX_BLOCK_X 1024
#define MAX_BLOCK_Y 1024
#define MAX_BLOCK_Z 64
#define MAX_GRID_X 65535
#define MAX_GRID_Y 65535
#define MAX_GRID_Z 65535

#define max(x,y)    ((x) > (y) ? (x) : (y))
#define min(x,y)    ((x) < (y) ? (x) : (y))

void calcDimensions(unsigned long long int iterations){
	printf("echo 'Running $0, Iterations: %d.'\n", iterations);
	
	unsigned long long int bz = 1;
	unsigned long long int by = 1;
	unsigned long long int bx = 1;
	
	unsigned long long int gz = 1;
	unsigned long long int gy = 1;
	unsigned long long int gx = 1;
	unsigned long long int config = 0;
	unsigned long long int confBlock = 0;
	unsigned long long int confGrid = 0;
	
	// Tentativa de encontrar o mínimo inicial, igualar o limite inferior dos laços.
	
	unsigned long long int countConfig = 0;
	
	int dimGrid = 1;
	int dimBlock = 1;

	for(bz = 1; bz <= min(iterations,MAX_BLOCK_Z); bz++) {
		for(by = 1; by <= min((iterations / bz),MAX_BLOCK_Y); by++) {
			for(bx = 1; bx <= min(((iterations / bz) / by),MAX_BLOCK_X); bx++) {
				for(gx = 1; gx <= min((((iterations / bz) / by) / bx),MAX_GRID_X); gx++) {
					for(gy = 1; gy <= min(((((iterations / bz) / by) / bx) / gx),MAX_GRID_Y); gy++) {
						for(gz = 1; gz <= min((((((iterations / bz) / by) / bx) / gx) / gy),MAX_GRID_Z); gz++) {
							confBlock = bx * by * bz;
							confGrid =  gx * gy * gz;
							config = confBlock * confGrid ;
							// Evita divergência de kernels, blocos e warps.
							if((confBlock <= 1024) && (config == iterations) && (confBlock % 32 == 0)){
								dimBlock = 0;
								dimGrid = 0;
								// Testa a quantidade de dimensões por bloco já usados.
								dimBlock += (bx > 1) ? 1 : 0;
								dimBlock += (by > 1) ? 1 : 0;
								dimBlock += (bz > 1) ? 1 : 0;
								if (dimBlock == 0 )
									dimBlock = 1;
								
								// Testa a quantidade de grids já usados.
								dimGrid += (gx > 1) ? 1 : 0;
								dimGrid += (gy > 1) ? 1 : 0;
								dimGrid += (gz > 1) ? 1 : 0;
								if(dimGrid == 0 )
									dimGrid = 1;
								
								countConfig++;							
								
								printf("\necho 'Execution %d of <totalexecution>.'\n", countConfig);
								
								printf("check_file_status nvprof-sumvector-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-std.txt\n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
								printf("return_val=$?\n");
								printf("if [ $return_val -ge 1 ];\n");
								printf("then\n");
								printf("  nvprof --print-gpu-trace --print-api-trace --metrics all --events all --csv ./sumvector <kernel> %d %d %d %d %d %d <nx> <ny> <nz> %dD_%dD $gpuId > nvprof-sumvector-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-std.txt 2> nvprof-sumvector-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-err.txt \n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
								printf("fi\n");
							}
						}
					}
				}
			}
		}
	}
	
	printf("# Configurations: %d\n", countConfig);
}

int main(int argc, char **argv) {
	int i, iterations = 0;
	
	if (argc != 2) {
		printf("Uso: %s <iterations>\n", argv[0]);
		return 0;
	}
	else{
		printf("# argumentos (argc): %d\n", argc);
		for (i = 0; i < argc; ++i) {
			printf("# argv[%d]: %s\n", i, argv[i]);
		}
		iterations = atoi(argv[1]);
		printf("# Executando: %s %d\n", argv[0], iterations);
	}
	
	/* Recuperar as informações da GPU. */
	printf("# %s Starting...\n", argv[0]);
	
	calcDimensions(iterations);
	
	printf("echo 'Done'.\n");
	
	return 0;
}