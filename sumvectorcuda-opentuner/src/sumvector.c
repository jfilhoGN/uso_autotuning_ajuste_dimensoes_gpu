#include <stdio.h>
#include <time.h>

#define N 100

int sumvector(int a[N], int b[N]){
	int resultado[N], i;

	for(i=0;i<N;i++){
		resultado[i] = a[i] + b[i];
	}
	print_interacoes(resultado);
}

int print_interacoes(int resultado[N]){
	int i;
	for(i=0;i<N;i++){
		printf("resultado: %d\n", resultado[i]);
	}
}


int main(int argc, char const *argv[])
{
	int a[N];
	int b[N];
	int i;
	for(i=0;i<N;i++){
		a[i] = i;
		b[i] = i;
	}
	sumvector(a,b);
	return 0;
}
