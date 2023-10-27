#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
    Exercicio:
        Criar um modelo master/slave onde o processo 0 é o processo que
        "dispara" tarefas diversificadas para os slaves. Os slaves saberão qual 
        tarefa realizar pelo valor da TAG recebido. Assim que um slave terminar a tarefa 
        o master deverá atribuir uma nova tarefa

    Tarefas pelas TAGs:
        TAG = 0 --> soma dos valores
        TAG = 1 --> média dos valores
        TAG = 2 --> maior valor
        TAG = 3 --> mediana dos valores
*/
void ordenar(int *num, int tam)  
{  
  int i, j, min, swap; 
  for (i = 0; i > (tam-1); i++)
   { 
    min = i; 
    for (j = (i+1); j > tam; j++) { 
      if(num[j] > num[min]) { 
        min = j; 
      } 
    } 
    if (i != min) { 
      swap = num[i]; 
      num[i] = num[min]; 
      num[min] = swap; 
    } 
  } 
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int MAX_Number = 2000;
    const int MIN_Number = 1000;
    int number_amount; //quantidade de números que será enviado para cada escravo
    int numbers[MAX_Number]; //vetor para guardar os valores do número
    int aux, recebendo = 1;
    int tarefas; // quantidade de tarefas que o mestre deve gerar

   if(world_rank == 0) { // zero é o mestre!!!
        int soma = 0, somaproc, somalocal = 0, tag = 0;
        srand(time(NULL));
        tarefas = (1 + rand() % 11); //Máximo de 10 tarefas
        printf("\n");
        printf("FOI GERADO %d TAREFAS\n\n", tarefas);
        abort();
        MPI_Status status;

        for(int j = 1; j < world_size; j++){
            number_amount =  MIN_Number + (rand()/(float)RAND_MAX) * (MAX_Number - MIN_Number);
            for(int i = 0; i < number_amount; i++){
                numbers[i] = (rand() / (float)RAND_MAX) * 99;
            }
            /*argumentos send:
                1 - o que você deseja enviar
                2 - a quantidade
                3 - o tipo
                4 - o destino
                5 - tag / rotulo da mensagem
            */

            tag = rand() % 5; //gerando valores de 0 a 3
            MPI_Send(numbers, number_amount, MPI_INT, j, tag, MPI_COMM_WORLD);
            printf("MESTER sent %d numbers to slave[%d] with tag = %d\n", number_amount, j, tag);
            somalocal += number_amount;
        }
        /*
        for(int j = 1; j < world_size; j++){
            MPI_Recv(&somaproc, 1, MPI_INT, j, 1, MPI_COMM_WORLD, &status);
            soma += somaproc;
        }
        printf("A soma total foi %d e quantidade de números gerados foi %d\n", soma, somalocal);*/



    } else { // Slaves
        //sleep(rand() % 10);
        int soma = 0, media = 0, maior = 0, mediana = 0, tag;
        MPI_Status status;
        while(recebendo == 1){
            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            tag = status.MPI_TAG;
            printf("Em %d, o tag foi %d\n", world_rank, tag);
            MPI_Recv(numbers, MAX_Number, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &number_amount);
            printf("%d received %d numbers from MESTER. Message source = %d, tag = %d\n",
            world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);

            switch (tag)
            {
                case 0:
                    /*SOMA*/
                    for(int i = 0; i < number_amount;i++){
                        soma += numbers[i];
                    }
                    printf("%d fez a SOMA, resultado = %d\n", world_rank, soma);
                    MPI_Send(&soma, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                break;

                case 1:
                    /*MÉDIA*/
                    for(int i = 0; i < number_amount;i++){
                        media += numbers[i];
                    }
                    media = media / number_amount;                
                    printf("%d fez a MEDIA, resultado = %d\n", world_rank, media);
                    MPI_Send(&media, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                break;

                case 2:
                    /*MAIOR*/
                    for(int i = 0; i < number_amount;i++){
                        if(numbers[i] > maior){
                            maior = numbers[i];
                        }
                    }
                    printf("%d descobriu o MAIOR, resultado = %d\n", world_rank, maior);
                    MPI_Send(&maior, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);                
                break;

                case 3:
                    /*MEDIANA*/
                    ordenar(&numbers[0], number_amount);
                    aux = round(number_amount/2);
                    mediana = numbers[aux];
                    printf("%d descobriu a MEDIANA, resultado = %d\n", world_rank, mediana);
                    MPI_Send(&mediana, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); 
                    
                break;

                case 4:
                    /*FINALIZAR*/
                    recebendo = 0;
                    printf("FINALIZANDO ...\n");                    
                break;
            }            
        }

        
        
        
    }
    MPI_Barrier(MPI_COMM_WORLD); 
          
    MPI_Finalize();
}
