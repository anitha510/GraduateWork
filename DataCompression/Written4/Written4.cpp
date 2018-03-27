// Written4.cpp 
/****************************************************************************

5/19/01		CS 6715

Leann Christianson

This program implements the forward and inverse Discrete Cosine Transform
using matrix multiplication.  Source has been modified from Nelson's Data
compression routines.

The N value in the define specifies the block size (we typically use N=8)

The initialize routine creates the cosine transform matrix and its transpose
matrix based on the given blocksize

The forwardDCT expects blocks of size N (unsigned chars) and returns transform coefficients
in integer form.

The inverseDCT expects blocks of N integers and returns a block of unsigned
chars

****************************************************************************/
#include <iostream> 
#include <stdlib.h>
#include <math.h>
using namespace std;

#define N   8
#define pi   3.1414


/* function prototypes */

void initialize();
void forwardDCT(unsigned char input[N][N], int output[N][N]);
void inverseDCT(int input[N][N], unsigned char output[N][N]);

/* globale data */

double C[N][N];      /* cosine transform matrix */
double Ct[N][N];      /* transpose of cosine transform matrix */

					  /***************************************************************************
					  initialize
					  creates the cosine transform matrix C and the corresponding cosine
					  transpose matrix Ct.  The argument N reflectst the chosen block size.
					  Typically we use 8x8 blocks, therefore, N should be 8
					  ***************************************************************************/
void initialize()
{

	int i, j;

	for (j = 0; j < N; j++) 
	{
		C[0][j] = 1.0 / sqrt((double)N);
		Ct[j][0] = C[0][j];
	}
	for (i = 1; i < N; i++) 
	{
		for (j = 0; j < N; j++) 
		{
			C[i][j] = sqrt(2.0 / N) * cos(pi* (2 * j + 1) * i / (2.0 * N));
			Ct[j][i] = C[i][j];
		}
	}
}
/***************************************************************************
forwardDCT:
This routine expects and 8 points of input.  It performs
matrix multiplication on the input pixel block and the result is stored in the output matrix.  
The output matrix is the final result with the DCT coefficients (in integer format)
***************************************************************************/
void forwardDCT(int input[N], int theta[N])
{
	double temp;
	int i, j;
	
	/* matrix multiply(output, C, input); */

	for (i = 0; i < N; i++) 
	{
		temp = 0.0;
		for (j = 0; j < N; j++) 
		{
			temp += C[i][j] * input[j];
		}
		theta[i] = (int)rint(temp);
	}

}
/***************************************************************************
inverseDCT:
This routine expects and 8 points of input.  It performs
matrix multiplication on the transform coefficient block and the transpose transform matrix.  
***************************************************************************/
void inverseDCT(int theta[N], int output[N])
{
	double temp;
	int i, j;

	/* matrix multiply(output, input, Ct) */

	for (i = 0; i < N; i++) 
	{
		temp = 0.0;
		for (j = 0; j < N; j++) 
		{
			temp += theta[j] * C[j][i];
		}
		output[i] = (int)rint(temp);
	}
}

void print(double matrix[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void print(int arr[N])
{
	for (int i = 0; i < N; i++)
		cout << arr[i] << " ";
	cout << "\n";
}

int main()
{
	int input[N], theta[N], newtheta[N], output[N];
	cout << "Enter the input array of size " << N << ":\n";
	for (int i = 0; i < N; i++)
		cin >> input[i];
	
	initialize();
	cout << "Multiply with C[N][N]\n";
	print(C);
	
	forwardDCT(input, theta);
	cout << "And get the theta[N]\n";
	print(theta);

	/*if (N == 8)
	{
		newtheta[0] = theta[0];
		newtheta[N - 1] = theta[N - 1];
		for (int i = 1; i < N-1; i++)
			newtheta[i] = 0;
	}
	else if (N == 16)
	{
		newtheta[0] = theta[0];
		newtheta[1] = theta[1];
		newtheta[N - 2] = theta[N - 2];
		newtheta[N - 1] = theta[N - 1];
		for (int i = 2; i < N - 2; i++)
			newtheta[i] = 0;
	}

	cout << "New theta \n";
	print(newtheta);*/

	cout << "Multiply Ct[N][N] with theta to get output\n";
	inverseDCT(theta, output);
	print(output);

	/*cout << "Multiply Ct[N][N] with newtheta to get output\n";
	inverseDCT(newtheta, output);
	print(output);*/

	system("Pause");
    return 0;
}

