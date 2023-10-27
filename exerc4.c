#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>


/*
    Trabalho 2 - Gerar 2 matrizes e ralizar a MULTIPLICAÇÃO entre elas em uma terceira matriz, 
    onde cada linha será calculada em um processo.

    Aluno: Renan Magalhães Lage - R.A.: 2021.1.08.020

*/

void imprimeLinha(int cols_B){
  for(int i = 0; i < cols_B; i++){
    printf("--");
  }
}

int main(int argc, char** argv) {

  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_rank == 0) {
    if(argc != 3){
      /* Critério da quantidade de parâmetros do programa*/
        fprintf(stderr, "ERROR: Usage --> multMatriz cols_matrix1 cols_matrix2 \n");
        exit(1);
    }
  }

  int rows_A = world_size;
  int cols_A = atoi(argv[1]);
  int rows_B = cols_A;
  int cols_B = atoi(argv[2]);
  int sum = 0;

  int matriz_A[rows_A][cols_A];
  int matriz_B[rows_B][cols_B];
  int line_A[cols_A];
  int vector_AB[cols_B];
  int matriz_C[rows_A][cols_B];


  /* Gerando os conteúdos das matrizes & mostrando o seu conteúdo*/
  if (world_rank == 0) {
    printf("Size A[%d][%d]\n", rows_A, cols_A);
    printf("Size B[%d][%d]\n", rows_B, cols_B);
    for (int i = 0; i < rows_A ; i++) {
      for(int j=0; j <cols_A; j++ ){
        /* Atribuindo números aleatórios de 1 a 10 para a matriz A*/
        matriz_A[i][j] = (int)(rand() / (float)RAND_MAX * 10 + 1);
      }
    }
    for (int i = 0; i < rows_B ; i++) {
      for(int j=0; j <cols_B; j++ ){
        /* Atribuindo números aleatórios de 1 a 10 para a matriz B*/
        matriz_B[i][j] = (int)(rand() / (float)RAND_MAX * 10 + 1);
      }
    }
    /* Imprimindo as Matrizes A e B */
    imprimeLinha(cols_A);
    printf("--Matriz A--");
    imprimeLinha(cols_A);
    printf("\n"); 
    for (int i = 0; i < rows_A; i++) {
          printf("     ( ");
            for (int j = 0; j < cols_A; j++) {
                printf("%3d  ", matriz_A[i][j]);
            }
            printf(" )");
            printf("\n");
    }
    printf("\n"); 
    imprimeLinha(cols_B);
    printf("--Matriz B--");
    imprimeLinha(cols_B);
    printf("\n"); 
    for (int i = 0; i < rows_B; i++) {
          printf("     ( ");
            for (int j = 0; j < cols_B; j++) {
                printf("%2d  ", matriz_B[i][j]);
            }
            printf(" )");
            printf("\n");
    }        
  }
  
  /* Enviando cada linha da matriz A para os diferentes processos*/
  MPI_Scatter(matriz_A, cols_A, MPI_INT, line_A, cols_A, MPI_INT, 0, MPI_COMM_WORLD);

  /* Enviando a matriz B para TODOS os processos*/
  MPI_Bcast(matriz_B, rows_B*cols_B, MPI_INT, 0, MPI_COMM_WORLD);

  /* Barrier para sincronizar os processos - só avança quando todos chegam nesse ponto */
  MPI_Barrier(MPI_COMM_WORLD);

  /* Realizando operação */
  for (int i = 0; i < cols_B; i++)
    {
      for (int j = 0; j < cols_A; j++)
      {
        sum = sum + line_A[j] * matriz_B[j][i];
      }
      vector_AB[i] = sum;
      //printf("%d \n",sum );
      sum = 0;
    }


  /* Root recebe a linha de cada processo */
  MPI_Gather(vector_AB, cols_B, MPI_INT, matriz_C,cols_B, MPI_INT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    /* Imprimindo a Matriz resultante */
    printf("\n");
    imprimeLinha(cols_B);
    printf("--Matriz Resultante--");
    imprimeLinha(cols_B);
    printf("\n");
        for (int i = 0; i < rows_A; i++) {
          printf("     ( ");
            for (int j = 0; j < cols_B; j++) {
                printf("%2d  ", matriz_C[i][j]);
            }
            printf(" )");
            printf("\n");
        }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
