#include <stdio.h>
#include <mpi.h>

#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[]){

int arrayZero[16] = {0};
int arrayOne[16];

int fromArray[2] = {0};

int rank, size;
int fromOne;
int sum = 0;
int i;
//value recieved from process 0
int num1;


MPI_Init(&argc, &argv);
MPI_Status status;

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

srand(time(NULL));
//@Process 0: randomly initialize integers in arrayZero
//Send the 2nd element in arrayZero to process 1
if(rank==0){
    for(i=0; i<16; i++){
        arrayZero[i] = rand()%26;
        printf("arrayZero[%d] = %d\n", i, arrayZero[i]);
    }
    
MPI_Send(&arrayZero[1], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
 printf("Process: %d, the second element in the array is: %d\n", rank, arrayZero[1]);
}

//@Process 1: recieve the variable from process 0
//Print out the variable for verification 
if(rank==1){
MPI_Recv(&num1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
fromOne=num1;
printf("Process: %d, the number recieved was: %d\n", rank, fromOne);
}


//Broadcast: send the variable from process 1 to all processes
MPI_Bcast(&fromOne, 1, MPI_INT, 1, MPI_COMM_WORLD);
printf("Process %d: fromOne: %d\n", rank, fromOne);

//Reduce: calculate the sum of the fromOne variables
sum = sum + fromOne;
int result;
MPI_Reduce(&sum, &result, 8, MPI_INT, MPI_SUM, 2, MPI_COMM_WORLD);
if(rank==2){
printf("Value of the sum is %d\n:", result);
}

//Scatter: scatter arrayZero into fromArrays on every process
MPI_Scatter(arrayZero, 2, MPI_INT, fromArray, 2, MPI_INT, 0, MPI_COMM_WORLD);
   printf("Process %d:\n", rank);
   printf("Values in fromArray: ");
   printf("%d %d\n", fromArray[0], fromArray[1]);

 //Gather: gather  the fromArrays and output arrayZero and arrayOne
 MPI_Gather(fromArray, 2, MPI_INT, arrayOne, 2, MPI_INT, 0, MPI_COMM_WORLD);
   if(rank==0)
   {
      printf("Process %d:\n", rank);
      printf("Elements in arrayZero: ");
      for(i=0; i<15; i++) {
          printf("%d, ", arrayZero[i]);
      }
      printf("%d\n", arrayZero[15]);
      printf("Elements in arrayOne: ");
      for(i=0; i<15; i++)
      {
         printf("%d, ", arrayOne[i]);
      }
      printf("%d\n", arrayOne[15]);

   }
MPI_Finalize();
}
    
    
