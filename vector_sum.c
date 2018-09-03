#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
// vector size
const int SIZE = 100;

int main()
{
	int v[101];
	int r;
	int comm_sz;
	int my_rank;
	int sum=0, z, total=0;
	/* initialize random seed: */
  	srand (time(NULL));
  	/*Random vector with nnumbers in 0 to 10*/
	for(int i=1;i<SIZE+1;i++)
	{
		v[i] = rand() % 10 + 1;
		total = total + v[i];
	}
	//MPI init
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Divide the vector by a factor 
	r = SIZE/comm_sz;

	//Sum subparts of the vector
	for(int i=1;i<=comm_sz;i++)
	{	
		z = 0;
		for(int j=0;j<r;j++)
		{
			z += v[r*(i-1)+1+j];
		}
		//first processor receive sum equal z and the other processors send the sum to the first processor 
		if(my_rank==0)
			sum = z;
		else
			MPI_Send(&z, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	if(my_rank==0)
	{
		for(int i=1;i<comm_sz;i++)
		{
			MPI_Recv(&z, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += z;
		}
		printf("Total: %d\n", sum); //Total recived by my_rank==0
		printf("Result: %d\n",total);//Total value in v[]
	}
	MPI_Finalize();
	return 0;
} 