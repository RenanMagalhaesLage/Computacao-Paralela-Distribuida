#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Faça um programa com a estrutura mestre/escravo, onde:

// - Mestre: gera uma quantidade aleatória entre 1000 a 2000 números de valores aleatórios entre 0 a 99 e envia para cada escravo.
// - Escravo: recebe os valores gerados pelo mestre, calcula a somatória dos valores e devolve para o mestre.
// - Mestre: calcula a soma total (soma de todas as somas recebidas)


int main(int argc, char** argv) {
    MPI_Status status;
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int MAX_Number = 2000;
    const int MIN_Number = 1000;
    int number_amount; //quantidade de números que será enviado para cada escravo
    int numbers[MAX_Number]; //vetor para guardar os valores do número

   if(world_rank == 0) { // zero é o mestre!!!
        int soma = 0, somaproc, somalocal = 0;
        srand(time(NULL));
        for(int j = 1; j < world_size; j++){
            number_amount =  MIN_Number + (rand()/(float)RAND_MAX) * (MAX_Number - MIN_Number);
            for(int i = 0; i < number_amount; i++){
                numbers[i] = (rand() / (float)RAND_MAX) * 99;
            }
            MPI_Send(numbers, number_amount, MPI_INT, j, 0, MPI_COMM_WORLD);
            printf("MESTER sent %d numbers to slave[%d]\n", number_amount, j);
            somalocal += number_amount;
        }
        
        for(int j = 1; j < world_size; j++){
            MPI_Recv(&somaproc, 1, MPI_INT, j, 1, MPI_COMM_WORLD, &status);
            soma += somaproc;
        }
        printf("A soma total foi %d e quantidade de números gerados foi %d\n", soma, somalocal);


    } else {
        int soma = 0;
        MPI_Recv(numbers, MAX_Number, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        //printf("Slave received! \n");
        MPI_Get_count(&status, MPI_INT, &number_amount);
        printf("%d received %d numbers from MESTER.\n",
           world_rank, number_amount);
        for(int i = 0; i < number_amount;i++){
            soma += numbers[i];
        }
        printf("%d somou %d\n", world_rank, soma);
        MPI_Send(&soma, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        
    }
    MPI_Barrier(MPI_COMM_WORLD); 
          
    MPI_Finalize();
}
