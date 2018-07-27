#include <stdio.h>
#include <math.h>

#define MAX_BLOCK_X 1024
#define MAX_BLOCK_Y 1024
#define MAX_BLOCK_Z 64

// #define MAX_GRID_X 65535
// #define MAX_GRID_X (pow(2,31) - 1)
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
	// double raiz = pow(iterations, (double) 1/6);
	
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
							// Evict kernel divergence, blocks with multiply warp size.
							if((confBlock <= 1024) && (config == iterations) && (confBlock % 32 == 0)){
							// if((confBlock <= 1024) && (config == iterations)){
								// printf("(%4d,%4d,%4d,%4d,%4d,%4d)\n", gx, gy, gz, bx, by, bz);
								dimBlock = 0;
								dimGrid = 0;
								// Test of quantity of block dimensions are used.
								dimBlock += (bx > 1) ? 1 : 0;
								dimBlock += (by > 1) ? 1 : 0;
								dimBlock += (bz > 1) ? 1 : 0;
								if (dimBlock == 0 )
									dimBlock = 1;
								
								// Test of quantity of grid dimensions are used.
								dimGrid += (gx > 1) ? 1 : 0;
								dimGrid += (gy > 1) ? 1 : 0;
								dimGrid += (gz > 1) ? 1 : 0;
								if(dimGrid == 0 )
									dimGrid = 1;
								
								countConfig++;							
								
								//./sincosc-cuda <kernel> <g.x> <g.y> <g.z> <b.x> <b.y> <b.z> <nx> <ny> <nz> <funcId>
								// printf("./sincosc-cuda <kernel> %d %d %d %d %d %d <nx> <ny> <nz> %dD_%dD\n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
								
								// nvprof ./sincosc-cuda 2 1 1 23 32 1 1 736 736 736 0 > nvprof-sincosc-cuda-2-1-1-23-32-1-1-736-736-736-0-std.txt 2> nvprof-sincosc-cuda-2-1-1-23-32-1-1-736-736-736-0-err.txt
								
								printf("\necho 'Execution %d of <totalexecution>.'\n", countConfig);
								/*printf("exec=0\n");
								printf("FILE=nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-std.txt\n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
								printf("if [ -f $FILE ];\n");
								printf("then\n");
  							printf("  echo \"File $FILE exists, verifying...\"\n");
  							printf("  check=`grep 'Done.' $FILE | wc -w`\n");
								printf("  if [ $check -eq 1 ];\n");
								printf("  then\n");
								printf("    echo ' Configuration was executed. [OK].'\n");
								printf("  else\n");
								printf("    echo ' Configuration was not executed until the end. [Reexecute].'\n");
								printf("    exec=$((exec+1))\n");
								printf("  fi\n");
								printf("else\n");
  							printf("  echo \" File $FILE does not exists. [Execute].\"\n");
								printf("  exec=$((exec+1))\n");
								printf("fi\n");
								
								printf("if [ $exec -ge 1 ];\n");
								printf("then\n");
								// printf("  nvprof ./sincosc-cuda <kernel> %d %d %d %d %d %d <nx> <ny> <nz> %dD_%dD $gpuId > nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-std.txt 2> nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-err.txt \n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
								// nvprof --print-gpu-trace --print-api-trace  --metrics all --events all --csv ./sincosc-cuda 2 1 2 16 1 1 1 32 32 32 3 2
								printf("  nvprof --print-gpu-trace --print-api-trace --metrics all --events all --csv ./sincosc-cuda <kernel> %d %d %d %d %d %d <nx> <ny> <nz> %dD_%dD $gpuId > nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-std.txt 2> nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-err.txt \n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
								printf("else\n");
								printf("  echo '  No more work. [OK].'\n");*/
								
								printf("check_file_status nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-std.txt\n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
								printf("return_val=$?\n");
								printf("if [ $return_val -ge 1 ];\n");
								printf("then\n");
								printf("  nvprof --print-gpu-trace --print-api-trace --metrics all --events all --csv ./sincosc-cuda <kernel> %d %d %d %d %d %d <nx> <ny> <nz> %dD_%dD $gpuId > nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-std.txt 2> nvprof-sincosc-cuda-<kernel>-%d-%d-%d-%d-%d-%d-<nx>-<ny>-<nz>-%dD_%dD-err.txt \n", gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock, gx, gy, gz, bx, by, bz, dimGrid, dimBlock);
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