// DCT.cpp

#include "stdafx.h"
#include "DCTEncoder.h"

/* globale data */

double C[N][N];      /* cosine transform matrix */
double Ct[N][N];      /* transpose of cosine transform matrix */
int InputImage[ROW][COL];		/* input image */

/***************************************************************************
initialize
creates the cosine transform matrix C and the corresponding cosine
transpose matrix Ct.  The argument N reflects the chosen block size.
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

void ReadImage()
{
	char line[80];
	int xres = 0, yres = 0, maxintensity = 0;

	//Open and read the the input image
	ifstream fp("input/rose.pgm");
	fp.getline(line, 80);
	if ((line[0] == 'P') && (line[1] == '5'))
		cout << "Greyscale images" << endl;
	while (fp.peek() == '#') fp.getline(line, 80); // ignore comments
	fp >> xres >> yres >> maxintensity;
	cout << "Rows : " << xres << endl;
	cout << "Columns : " << yres << endl;
	cout << "Max Value : " << maxintensity << endl;
		
	//File for saving pixel values
	ofstream ft("temp/InputPixels.txt", std::ofstream::trunc);
	int i, j;
	for (i = 0; i < 320 && !fp.eof(); i++)
	{
		for (j = 0; j < 200 && !fp.eof(); j++)
		{
			InputImage[i][j] = ((int)fp.get()) - 128;
			ft << InputImage[i][j] << " ";
		}
		ft << endl;
	}
	cout << "i : " << i << endl << "j : " << j << endl;
	fp.close();
	ft.close();
}

int main()
{
	initialize();
	ReadImage();
	DCTEncoder(InputImage, C, Ct);
	system("Pause");
    return 0;
}

