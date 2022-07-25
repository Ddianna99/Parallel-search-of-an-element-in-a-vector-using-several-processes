#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <fstream>
using namespace std;

#define MAX 10000

int main(int argc, char* argv[])
{

	int i, n, myid, num_procs;
	int start, stop;
	double my_sum, others_sum;
	double startTime, endTime;
	int x[MAX], y[MAX];
	int numar_cautat;

	for (i = 0; i < MAX; ++i)
		x[i] = rand() % 100;


	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);


	if (myid == 0) {
		cout << "Using " << num_procs << " processes..." << endl << endl;
	}

	startTime = MPI_Wtime();

	n = (num_procs > 1) ? MAX / num_procs : 1;
	start = myid * n;

	if (myid != (num_procs - 1))
	{
		stop = start + n;
	}
	else
	{
		stop = MAX;
	}


	if (myid == 0)
	{

		int starti, stopi;
		for (i = 1; i < num_procs; i++)
		{
			n = (num_procs > 1) ? MAX / num_procs : 1;
			starti = i * n;
			if (i != (num_procs - 1))
			{
				stopi = starti + n;
			}
			else
			{
				stopi = MAX;
			}
			MPI_Send(x + starti, stopi - starti, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		cout << "\nIntroduceti un numar : "; cin >> numar_cautat;

		MPI_Bcast(&numar_cautat, 1, MPI_INT, 0, MPI_COMM_WORLD);

		int indice = 0, c[MAX];
		for (i = start + 1; i < stop; i = i + 1)
		{
			if (x[i] == numar_cautat) {
				c[indice++] = i;
			}
		}

		cout << "\nProc 0 a gasit elementul de " << indice << " ori : \n";
		for (i = 0; i < indice; i++) {
			cout << c[i] << " ";
		}
		cout << "\n\n";

		int nr_apariti = indice;

		for (i = 1; i < num_procs; i++)
		{
			MPI_Recv(&indice, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (indice) {
				MPI_Recv(&c, indice, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

			cout << "Proc " << i << " a gasit elementul de " << indice << " ori :" << endl;
			for (int j = 0; j < indice; j++) {
				cout << c[j] << " ";
			}
			cout << "\n\n";

			nr_apariti += indice;
		}

		endTime = MPI_Wtime();
		cout << "Elemntul dat a fost gasit de " << nr_apariti << " ori " << endl << endl;
		cout << "Elapsed time = " << endTime - startTime << " seconds" << endl;

	}
	else
	{
		int x_size = stop - start;
		MPI_Recv(&x, x_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Bcast(&numar_cautat, 1, MPI_INT, 0, MPI_COMM_WORLD);
		int indice = 0, c[MAX];
		for (i = 0; i < x_size; i = i + 1)
		{
			if (x[i] == numar_cautat) {
				c[indice++] = start + i;
			}
		}
		MPI_Send(&indice, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		if (indice != 0) {
			MPI_Send(&c, indice, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
}