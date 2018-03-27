// DCTEncoder.cpp

#include "DCTEncoder.h"

/***************************************************************************
forwardDCT:
This routine expects and 8x8 block of input.  It performs
matrix multiplication on the input pixel block and the transposed matrix.
The result is stored in a temporary 8x8 block which is multipled by the
cosine transform matrix.  The result is stored in the output matrix.  The
output matrix is the final result with the DCT coefficients (in integer
format.  The values should range from -1,024 to 1,023
***************************************************************************/

ofstream tFileGen;
ofstream tFileTrim;
ofstream tFileTemp;

void ForwardDCT(int input[N][N], double C[N][N], double Ct[N][N], int output[N][N])
{
	double temp[N][N];
	double dTemp;
	int i, j, k, iTmp;

	/* matrix multiply(temp, input, Ct); */

	for (i = 0; i < N; i++) 
	{
		for (j = 0; j < N; j++) 
		{
			temp[i][j] = 0.0;
			for (k = 0; k < N; k++)
				temp[i][j] += ((int)input[i][k]) * Ct[k][j];
		}
	}

	/* matrix multiply(output, C, temp); */

	for (i = 0; i < N; i++) 
	{
		for (j = 0; j < N; j++) 
		{
			dTemp = 0.0;
			for (k = 0; k < N; k++) 
			{
				dTemp += C[i][k] * temp[k][j];
			}
			iTmp = (int)rint(dTemp);
			tFileGen << iTmp << " ";
			if(iTmp > 50 || iTmp < -50) 
				tFileTrim << iTmp << endl;
			else
				tFileTrim << "0" << endl;
			output[i][j] = iTmp;
		}
		tFileGen << endl;
	}
	tFileGen << endl;
}

void DCTEncoder(int InputImage[ROW][COL], double C[N][N], double Ct[N][N])
{
	int input[N][N], output[N][N];
	tFileTrim.open("temp/CoeffTrimmed.txt", std::ofstream::trunc);
	tFileGen.open("temp/CoeffGenerated.txt", std::ofstream::trunc);
	tFileTemp.open("temp/temp.txt", std::ofstream::trunc);
	for (int a = 0; a < ROW; a++)
		for (int b = 0; b < COL;b++)
			tFileTemp << InputImage[a][b] << " ";

	for (int i = 0; i < ROW / N; i++)
	{
		for (int j = 0; j < COL / N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				for (int l = 0; l < N; l++)
				{
					input[k][l] = InputImage[i*N + k][j*N + l];
				}
			}
			tFileGen << i << "  " << j << endl;
			ForwardDCT(input, C, Ct, output);
		}
	}
	tFileTrim.close();
	tFileGen.close();
}
