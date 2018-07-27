#include <stdio.h>

int main(int argc, char **argv) {
  int i, j, k, indice;
  int nx = 0;
  int ny = 0;
  int nz = 0;
  
  if (argc != 4) {
    printf("Uso: %s <nx> <ny> <nz>\n", argv[0]);
    return 0;
  }
  else{
    printf("#argumentos (argc): %d\n", argc);
    for (i = 0; i < argc; ++i) {
      printf(" argv[%d]: %s\n", i, argv[i]);
    }
    
    nx = atoi(argv[1]);
    ny = atoi(argv[2]);
    nz = atoi(argv[3]);
    printf("Executando: %s %d %d %d\n", argv[0], nx, ny, nz);
  }
  
  /* Recuperar as informações da GPU. */
  printf("%s Starting...\n", argv[0]);
  
  for (i = 0; i < nx; ++i) {
    for (j = 0; j < ny; ++j) {
      for (k = 0; k < nz; ++k) {
	indice = (i * ny * nz) + (j * nz) + k;
	printf("indice: %d\n", indice);
      }
    }
  }
  
  printf("Done.\n");
  
  return 0;
}
