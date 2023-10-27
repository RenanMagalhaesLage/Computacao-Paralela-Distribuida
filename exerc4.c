#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>


/*
    2° Gerar 2 matrizes e ralizar a soma entre elas em uma terceira matriz, onde cada linha será calculada em um processo

*/


int main(int argc, char** argv) {

  // Seed the random number generator to get different results each time
  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int ROWS = world_size;
  int COLS = world_size;
    int* matriz_One = NULL;
    int* matriz_Two = NULL;

  if (world_rank == 0) {
    matriz_One = (int*)malloc(COLS * ROWS * sizeof(int));
    matriz_Two = (int*)malloc(COLS * ROWS * sizeof(int));
    for (int i = 0; i < ROWS * COLS; i++) {
        matriz_One[i] = (int)(rand() / (float)RAND_MAX * 10 + 1);
        matriz_Two[i] = (int)(rand() / (float)RAND_MAX * 10 + 1);
    }
  }
  
  // For each process, create a buffer that will hold a subset of the entire array
  int *line_for_process1 = (int *)malloc(sizeof(int) * COLS);
  assert(line_for_process1 != NULL);

  int *line_for_process2 = (int *)malloc(sizeof(int) * COLS);
  assert(line_for_process2 != NULL);
  
  // Scatter the random numbers from the root process to all processes in the MPI world
  MPI_Scatter(matriz_One, COLS, MPI_INT, line_for_process1, COLS, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(matriz_Two, COLS, MPI_INT, line_for_process2, COLS, MPI_INT, 0, MPI_COMM_WORLD);

    int* sum_lines  = (int*)malloc(COLS * sizeof(int));
   for (int i = 0; i < COLS; i++)
   {
      sum_lines[i] = line_for_process1[i] + line_for_process2[i];
   }

    // Matriz para armazenar o resultado da redução
    int* global_result = NULL;

    // Redução da soma das linhas das matrizes usando MPI_Reduce
    if (world_rank == 0) {
      global_result = (int*)malloc(COLS * ROWS * sizeof(int));
      assert(global_result != NULL);
    }
    MPI_Gather(sum_lines, COLS, MPI_INT, global_result, COLS, MPI_INT, 0, MPI_COMM_WORLD);
  //MPI_Gather(&sum_lines, 1, MPI_INT, sub_sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    printf("-------------Matriz Resultante-------------\n");
        for (int i = 0; i < ROWS; i++) {
          printf("( ");
            for (int j = 0; j < COLS; j++) {
                printf("%2d  ", global_result[i * COLS + j]);
            }
            printf(" )");
            printf("\n");
        }
        free(global_result);
  }

  // Clean up
  if (world_rank == 0) {
    free(sum_lines);
    free(matriz_One);
    free(matriz_Two);
  }
  free(line_for_process1);
  free(line_for_process2);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}