#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
// vector size
const int SIZE = 10;
int main()
{
	int v[SIZE+1], b[SIZE+1];
	int r;
	int comm_sz;
	int my_rank;
	int sum=0, z=0, aux=0;
	/* initialize random seed: */
  	srand (time(NULL));
  	/*Random vector with nnumbers in 0 to 10*/
	for(int i=1;i<SIZE+1;i++)
		v[i] = rand() % 10 + 1;
	//MPI init
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int ST[comm_sz];

	r = (SIZE+1)/comm_sz;

	for(int j=0;j<r;j++)
	{
		z += v[r*my_rank+1+j];
	}

	if(my_rank!=0)
		MPI_Send(&z, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

	if(my_rank==0)
	{
		ST[0] = z;
		for(int i=1;i<comm_sz;i++)
		{
			MPI_Recv(&aux, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			ST[i] = aux + ST[i-1];
			sum = ST[i-1];
			MPI_Send(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	if(my_rank!=0)
	{
		MPI_Recv(&z, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		b[r*my_rank] = z;
		for(int i=1;i<r;i++)
		{
			b[r*my_rank + i] = b[r*my_rank +i-1] + v[r*my_rank +i];
		}
	}
	else
	{
		b[1] = v[1];
		for(int i=2;i<r;i++)
		{
			b[i] = b[i-1] + v[i];
		}
	}

	for(int i=0;i<comm_sz;i++)
	{
		if(my_rank==i)
		{
			for(int j=1;j<r;j++)
				printf("%d ", b[i*r+j]);	
		}
	}

	MPI_Finalize();
	return 0;
}