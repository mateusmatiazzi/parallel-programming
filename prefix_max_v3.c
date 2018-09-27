#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
// vector comm_sz
const int SIZE = 10;

int max(int a, int b)
{
	if(a>b)
		return a;
	return b;
}

int main()
{
	int comm_sz, rank;
	int v[SIZE];
	int s[SIZE];
	int *T = malloc(sizeof(int) * comm_sz);
	int i = 0;
	int j = 0;
	for(int i=0;i<SIZE;i++)
	{
		v[i] = rand() % 10 + 1;
		s[i] = 0;
	}

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status status;
	int tamanho_s = (int) SIZE/comm_sz;
	int aux = tamanho_s + (SIZE-tamanho_s*comm_sz);
	//Envio de dados
	if (rank == 0){

		for ( i = 0; i < comm_sz - 1; ++i)
		{
			for ( j = 0; j < tamanho_s; ++j){
				s[j]  = v[tamanho_s*i + j];
			}
			MPI_Send(s, tamanho_s, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		for (j = 0, i = comm_sz - 1; j < tamanho_s + (SIZE-tamanho_s*comm_sz); ++j)
		{
			s[j]  = v[tamanho_s*i + j];
		}

		MPI_Send(s, tamanho_s + (SIZE-tamanho_s*comm_sz), MPI_INT, i, 0, MPI_COMM_WORLD);

		for ( j = 0; j < tamanho_s; ++j){
			s[j] = v[j];
		}

	}else{
		// recebimento
		if (rank==comm_sz-1){
			MPI_Recv(s, tamanho_s + (SIZE-tamanho_s*comm_sz) , MPI_INT,0,0, MPI_COMM_WORLD, &status);
		}else{
			MPI_Recv(s, tamanho_s , MPI_INT,0,0, MPI_COMM_WORLD, &status);
		}

	}

	T[rank] = s[0];
	
	if (rank == comm_sz -1){
		for(i = 0; i < tamanho_s + (SIZE-tamanho_s*comm_sz); ++i){
			T[rank] = max(T[rank] , s[i]);
		}
	}else{
		for(i = 0; i < tamanho_s; ++i){
			T[rank] = max(T[rank] , s[i]);
		}
	}

	for (i = 0; i <= rank; ++i)
	{
		MPI_Bcast(&T[i], 1, MPI_INT, i, MPI_COMM_WORLD);
	}

	if(rank==0){
		T[0] = 0;
		for(i = 0; i < tamanho_s; ++i){
			s[i] = max(s[i] , T[0]);
			T[0] = s[i]; 
		}

	}else if (rank == comm_sz -1){
	
		for(i = 0; i < tamanho_s + (SIZE-tamanho_s*comm_sz); ++i){
			s[i] = max(s[i] , T[rank-1]);
			T[rank-1] = s[i]; 
		}
		tamanho_s = tamanho_s + (SIZE-tamanho_s*comm_sz);

	}else{
		for(i = 0; i < tamanho_s; ++i){
			s[i] = max(s[i] , T[rank-1]);
			T[rank-1] = s[i]; 
		}
	}
	
	printf("rank %d final:%d with %d elements\n", rank, s[i-1], tamanho_s );
	
	MPI_Finalize();
	return 0;
}