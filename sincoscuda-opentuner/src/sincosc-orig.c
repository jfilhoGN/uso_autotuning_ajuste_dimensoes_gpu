/*
 ============================================================================
 Name        : sincosc.c
 Author      : rag@ime.usp.br
 Version     :
 Copyright   : Your copyright notice
 Description : sincos in C.
 ============================================================================
 */
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Default problem size. */
#ifndef NX
# define NX 16
#endif
#ifndef NY
# define NY 16
#endif
#ifndef NZ
# define NZ 16
#endif

#ifndef DATA_TYPE
#define DATA_TYPE float
#endif

/*#ifndef POLYBENCH_TEST_MALLOC
DATA_TYPE x[NX * NY * NZ];
DATA_TYPE y[NX * NY * NZ];
DATA_TYPE xy[NX * NY * NZ];
#else
DATA_TYPE* x = (DATA_TYPE*) malloc(sizeof(DATA_TYPE) * NX * NY * NZ);
DATA_TYPE* y = (DATA_TYPE*) malloc(sizeof(DATA_TYPE) * NX * NY * NZ);
DATA_TYPE* xy = (DATA_TYPE*) malloc(sizeof(DATA_TYPE) * NX * NY * NZ);
#endif
*/

// ./sincos.exe 16 16 16
// 5341.616211 0.552198 1.838819

void sincos_function_(int nx, int ny, int nz, DATA_TYPE* x, DATA_TYPE* y,
		DATA_TYPE* xy) {
	int i, j, k, indice;

	for (i = 0; i < nx; ++i) {
		for (j = 0; j < ny; ++j) {
			for (k = 0; k < nz; ++k) {
				indice = (i * ny * nz) + (j * nz) + k;
				// xy(i, j, k) = sin(x(i, j, k)) + cos(y(i, j, k))
				xy[indice] = sin(x[indice]) + cos(y[indice]);
			}
		}
	}
}

float getSum(float * xy, int sz) {
	int i;
	float resSum = 0.0;
	for (i = 0; i < sz; i++)
		resSum += xy[i];
	return resSum;
}

float getMin(float * xy, int sz) {
	int i;
	float resMin = xy[0];
	for (i = 1; i < sz; i++)
		if (resMin > xy[i])
			resMin = xy[i];
	return resMin;
}

float getMax(float * xy, int sz) {
	int i;
	float resMax = xy[0];
	for (i = 1; i < sz; i++)
		if (resMax < xy[i])
			resMax = xy[i];
	return resMax;
}

void init_arrays(float* x, float* y){
	int i;
	double invrmax = 1.0 / RAND_MAX;
	for (i = 0; i < NX * NY * NZ; i++) {
		x[i] = rand() * invrmax;
		y[i] = rand() * invrmax;
	}
}

int main(int argc, char* argv[]) {

	DATA_TYPE* x = (DATA_TYPE*) malloc(sizeof(DATA_TYPE) * NX * NY * NZ);
	DATA_TYPE* y = (DATA_TYPE*) malloc(sizeof(DATA_TYPE) * NX * NY * NZ);
	DATA_TYPE* xy = (DATA_TYPE*) malloc(sizeof(DATA_TYPE) * NX * NY * NZ);

	int nx = NX;
	int ny = NY;
	int nz = NZ;

	/*if (argc != 4) {
		printf("Uso: %s <nx> <ny> <nz>\n", argv[0]);
		printf("Assumindo valores padrão: %s %d %d %d\n", argv[0], nx, ny, nz);
		// return 0;
	}
	else{
		nx = atoi(argv[1]);
		ny = atoi(argv[2]);
		nz = atoi(argv[3]);
		printf("Assumindo valores padrão: %s %d %d %d\n", argv[0], nx, ny, nz);
	}*/
	// Passando parâmetros na compilação: -DNX=1024 -DNY=1024 -DNZ=1024

	init_arrays(x, y);

	sincos_function_(nx, ny, nz, x, y, xy);

	int sizeXy = (nx * ny * nz);

	DATA_TYPE sum = getSum(xy, sizeXy);
	DATA_TYPE min = getMin(xy, sizeXy);
	DATA_TYPE max = getMax(xy, sizeXy);

	printf("%f %f %f\n", sum, min, max);

	// free(x);
	// free(y);
	// free(xy);

	return 0;
}
