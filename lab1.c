# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>

#define MASTER 0

int main ( int argc, char *argv[] );
void timestamp ( );

int main ( int argc, char *argv[] )
{
# define N 100000

    long array[N];
    long i;
    int taskid;
    int numtasks;
    MPI_Status status;
    long sum;
    long sum_all;

    MPI_Init ( &argc, &argv );

    MPI_Comm_size ( MPI_COMM_WORLD, &numtasks );
    MPI_Comm_rank ( MPI_COMM_WORLD, &taskid );
	
	if ( taskid == MASTER ) 
    {
        for (i = 0; i < N; ++i)
        {
            array[i] =  1;
        }
    }

    if ( taskid == MASTER )
    {
        timestamp ( );
        printf ( "\n" );
        printf ( "SUM - MASTER process:\n" );
        printf ( "The number of processes available is %d.\n", numtasks );
    }

    MPI_Bcast ( array, N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD );

    sum = 0;

    long left = N * taskid / numtasks;
    long right = N * (taskid + 1) / numtasks;
    long j = 0;

    for ( j = left; j < right; j++ )
    {
        sum += array[j];
    }

    printf ( "\n" );
    printf ( "SUM - Process %d:\n", taskid );
    printf ( "  Calculated value is %d\n", sum );

    if ( taskid != MASTER ) 
    {
        MPI_Send ( &sum, 1, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD );
    }
    else 
    {
        sum_all = sum;
        for ( i = 1; i < numtasks; i++ ) 
        {
            MPI_Recv ( &sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, 
                    MPI_COMM_WORLD, &status );

            sum_all = sum_all + sum;
        }
    }

    if ( taskid == MASTER )
    {
        printf ( "\n");         
        printf ( "SUM - MASTER process:\n");         
        printf ( "The total sum is %d\n", sum_all );
    }

    MPI_Finalize ( );

    if ( taskid == MASTER )
    {
        printf ( "\n");         
        printf ( "SUM - MASTER process:\n");         
        printf ( "  Normal end of execution.\n"); 
        printf ( "\n" );
        timestamp ( ); 
    }
    return 0;

# undef N
}
/******************************************************************************/

void timestamp ( )
{
# define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    const struct tm *tm;
    time_t now;

    now = time ( NULL );
    tm = localtime ( &now );

    strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

    printf ( "%s\n", time_buffer );

    return;
# undef TIME_SIZE
}