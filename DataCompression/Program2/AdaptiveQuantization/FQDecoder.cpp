// FQDecoder.cpp 
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <math.h>  // round
using namespace std;

int main()
{
	unsigned char ch, parta, partb, mask = 15;
	int i=0,j=0, xres = 256, yres = 256, maxintensity = 255;
	int min = 256, max = -1, frameSize = 256, pixel = 0;
	double delta = 0, M =15.0;

	ifstream encodedFile("tFileNew.bin", ios::binary);
	ofstream tFileUnpack("tFileUnpack.txt", std::ofstream::trunc);
	ofstream of("goldhillUnpacked.pgm", std::ofstream::trunc);
	of << "P5" << endl;
	of << xres << " " << yres << endl;
	of << maxintensity << endl;

	while (!encodedFile.eof())
	{
		ch = encodedFile.get();
		min = (int)ch;
		ch = encodedFile.get();
		max = (int)ch;
		delta = (max - min) / M;

		//write min max and delta values in intermediate file
		tFileUnpack << "Min:" << min << "  " << "Max:" << max << "  " << "Delta:" << delta << endl;

		//unpack each frame
		for (j = 0; j < frameSize/2; j++)
		{
			ch = encodedFile.get();
			partb = mask & ch;
			parta = ch >> 4;
			pixel = min + round((int)parta * delta); //construct original pixel from part a
			tFileUnpack << pixel << "  ";
			of << (unsigned char)pixel;
			pixel = min + round((int)partb * delta); //construct original pixel from part b
			tFileUnpack << pixel << "  ";
			of << (unsigned char)pixel;

		}
		tFileUnpack << endl;
	}
	of.close();
	tFileUnpack.close();
	system("Pause");
	return 0;
}

