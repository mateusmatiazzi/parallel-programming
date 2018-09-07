#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
// vector size
const int SIZE = 40;

const int RIGHT = 1;
const int LEFT = -1;

int main()
{
	int v[SIZE+1];
	int r, subpart;
	int comm_sz;
	int my_rank;
	int sum=0, z=0, total=0;
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

	r = sqrt(comm_sz);

	subpart = (SIZE)/comm_sz;

	for(int i=0;i<subpart;i++)
		z += v[my_rank*subpart+1+i];

	sum = z;

	for(int i=0;i<r-1;i++)
	{
		if(my_rank%r==i){
			MPI_Send(&sum, 1, MPI_INT, (my_rank+RIGHT), 0, MPI_COMM_WORLD);
		}
		if(my_rank%r==i+1){
			MPI_Recv(&z, 1, MPI_INT,(my_rank+LEFT), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		sum += z;
	}
	for(int i=0;i<r-1;i++)
	{
		if(my_rank%r==r-1 && my_rank != comm_sz-1)
			MPI_Send(&sum, 1, MPI_INT, my_rank+r, 0, MPI_COMM_WORLD);
		if(my_rank%r==r-1 && my_rank != r-1)
			MPI_Recv(&z, 1, MPI_INT, my_rank-r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum += z;
	}
	if(my_rank==comm_sz-1)
	{
		printf("Total: %d\n", sum); //Total recived by my_rank==0
		printf("Result: %d\n",total);//Total value in v[]
	}

	MPI_Finalize();
	return 0;
}