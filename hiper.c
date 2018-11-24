#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
// vector comm_sz
const int SIZE = 10;

int main(int argc, char *argv[])
{
	int comm_sz, rank;
	double dimension;
	int v[SIZE];
	int s[SIZE];
	int next = 0;
	int j = 0;
	int i = 0;
	int Soma = 0;
	int root = comm_sz/2 + (comm_sz%2 != 0) - 1;
	int Soma_Parcial = 0;
	MPI_Status status;

	int tam_s = (int) SIZE/comm_sz;


	for(int i=0;i<SIZE;i++)
	{
		v[i] = rand() % 10 + 1;
		s[i] = 0;
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	


	dimension = log2((double) comm_sz );

	if( ( (dimension - (int) dimension)) != 0 ){
		if (rank==0)
			printf("Precisa ser uma potencias de 2\n");
		MPI_Finalize();
		return 0;
	}

	if (rank == root){

		next = rank - 1; 

		if ( next >=0 ){


			for ( j = 0; j < tam_s; ++j){
				s[j]  = v[tam_s*next + j];
			}
			
			MPI_Send(s, tam_s, MPI_INT, next, 0, MPI_COMM_WORLD);
		}

		next = rank + 1;
		
		if ( next <= comm_sz -1){

			for ( j = 0; j < tam_s + ( (next == comm_sz -1) ? (SIZE-tam_s*comm_sz) : 0)  ; ++j){
				s[j]  = v[tam_s*next + j];
			}
			
			MPI_Send(s, tam_s + ( (next == comm_sz -1) ? (SIZE-tam_s*comm_sz) : 0) , MPI_INT, next, 0, MPI_COMM_WORLD);
		}

		for ( j = 0; j < tam_s + ( (rank == comm_sz -1) ? (SIZE-tam_s*comm_sz) : 0)  ; ++j){
			
			s[j] = v[tam_s*rank + j];
		}

	}else if(rank < root){
		

		next = rank - 1; 

		if ( next >=0 ){


			for ( j = 0; j < tam_s; ++j){
				s[j]  = v[tam_s*next + j];
			}
			
			MPI_Send(s, tam_s, MPI_INT, next, 0, MPI_COMM_WORLD);
		}

		MPI_Recv(s, tam_s, MPI_INT,rank+1,0, MPI_COMM_WORLD, &status);

	}else if(rank > root){

		next = rank + 1; 

		if ( next <= comm_sz - 1 ){


			for ( j = 0; j < tam_s + ( (next == comm_sz -1) ? (SIZE-tam_s*comm_sz) : 0)   ; ++j){
				s[j]  = v[tam_s*next + j];
			}
			
			MPI_Send(s, tam_s + ( (next == comm_sz -1) ? (SIZE-tam_s*comm_sz) : 0) , MPI_INT, next, 0, MPI_COMM_WORLD);
		}

		MPI_Recv(s, tam_s + ( (rank == comm_sz -1) ? (SIZE-tam_s*comm_sz) : 0) , MPI_INT,rank-1,0, MPI_COMM_WORLD, &status);

	}


	for(i = 0; i < tam_s  + ( (next == comm_sz -1) ? (SIZE-tam_s*comm_sz) : 0) ; ++i){
		Soma_Parcial = Soma_Parcial + s[i];
	}

	MPI_Reduce(&Soma_Parcial, &Soma, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
	
	if(rank==root)
		printf("Soma:%d\n", Soma);
	
	MPI_Finalize();
	return 0;
}