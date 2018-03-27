// QuantitativeMeasure.cpp : 

#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <math.h>  // round
using namespace std;

int main()
{
	char line[80];
	unsigned char orgPixel, recPixel;
	int orgValue, recValue;
	int i = 0, j = 0, xres = 256, yres = 256, maxintensity = 255, peak = -1;
	double tmp = 0, sigma = 0, msqe = 0, snr = 0, psnr = 0, N = 256 * 256, M = 16;

	//Open and read the the original image
	ifstream orgImage("goldhill.pgm");
	orgImage.getline(line, 80);
	orgImage >> xres >> yres >> maxintensity;

	//Open and read the the recontructed image
	ifstream recImage("goldhillUnpacked.pgm");
	recImage.getline(line, 80);
	recImage >> xres >> yres >> maxintensity;

	for (i = 0; i < N; i++)
	{
		orgPixel = orgImage.get();
		recPixel = recImage.get();
		orgValue = (int)orgPixel;
		recValue = (int)recPixel;
		if (orgValue > peak)	peak = orgValue;
		sigma += recValue*recValue;
		tmp = recValue - orgValue;
		msqe += tmp*tmp;
	}

	cout << "Peak : " << peak << endl;
	sigma = sigma / N;
	cout << "Sigma : " << sigma << endl;
	msqe = msqe / N;
	cout << "MSQE : " << msqe << endl;
	snr = sigma / msqe;
	cout << "SNR : " << snr << endl;
	psnr = (peak * peak) / msqe;
	cout << "PSNR : " << psnr << endl;

	orgImage.close();
	recImage.close();

	FILE * pFile;
	long orgsize = 0;
	pFile = fopen("goldhill.pgm", "rb");
	fseek(pFile, 0, SEEK_END);  
	orgsize = ftell(pFile);
	fclose(pFile);
	cout << "Original Image Size : " << orgsize << " bytes." << endl;

	/*long recsize = 0;
	pFile = fopen("goldhillUnpacked.pgm", "rb");
	fseek(pFile, 0, SEEK_END);   
	recsize = ftell(pFile);
	fclose(pFile);
	cout << "Reconstructed Image Size : " << recsize << " bytes." << endl;
*/
	long compsize=0;
	pFile = fopen("CompressedByteFile.bin", "rb");
	fseek(pFile, 0, SEEK_END);   
	compsize = ftell(pFile);
	fclose(pFile);
	cout << "Compressed File Size : " << compsize << " bytes." << endl;

	int compRatio = orgsize / compsize;
	cout << "Compression Ratio is " << compRatio << " : 1" << endl;
	system("Pause");
	return 0;
}

