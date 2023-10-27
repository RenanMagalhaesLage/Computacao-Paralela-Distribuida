#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

/*
    1° Gerar um vetor de 1000 elementos, dividir em P processos e encontrar o menor valor do vetor
*/

// Creates an array of random numbers. Each number has a value from 0 - 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

float lowest_value(float *array, int num_elements){
    float lower = array[0];
    for (int i = 1; i < num_elements; i++) {
        if (array[i] < lower) {
            lower = array[i];
        }
    }
    return lower;
}

int main(int argc, char** argv) {

  // Seed the random number generator to get different results each time
  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Creating a random array of 1000 elements on the root process
  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = create_rand_nums(1000);
  }

    //Dividing the array by the number of processes to determine how many numbers each process will handle.
    int num_elements_per_proc = (int)1000 / world_size;

  // For each process, create a buffer that will hold a subset of the entire array
  float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
  assert(sub_rand_nums != NULL);

  // Scatter the random numbers from the root process to all processes in the MPI world
  MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Compute the lowest value of your subset
  float sub_lowest = lowest_value(sub_rand_nums, num_elements_per_proc);

  // Gather all partial lowests value down to the root process
  float *sub_lowests= NULL;
  if (world_rank == 0) {
    sub_lowests = (float *)malloc(sizeof(float) * world_size);
    assert(sub_lowests != NULL);
  }
  MPI_Gather(&sub_lowest, 1, MPI_FLOAT, sub_lowests, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    float menor = lowest_value(sub_lowests, world_size);
    printf("The lowest value using P process is:  %f\n", menor);
    // Compute the average across the original data for comparison
    float original_data_smaller = lowest_value(rand_nums, num_elements_per_proc * world_size);
    printf("The original lowest value is: %f\n", original_data_smaller);
  }

  // Clean up
  if (world_rank == 0) {
    free(rand_nums);
    free(sub_lowests);
  }
  free(sub_rand_nums);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}