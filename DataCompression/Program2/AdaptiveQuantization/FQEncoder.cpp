// FQEncoder.cpp 
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <math.h>  // round
using namespace std;
struct packed 
{
	unsigned char a : 4, b : 4;
};
int main()
{
	char line[80];
	unsigned char ch, frame[256];
	int image[256][256], quantizedFrame[256];
	int i = 0, j = 0, xres = 256, yres = 256, maxintensity = 255;
	
	//Open and read the the input image
	ifstream fp("goldhill.pgm");
	fp.getline(line, 80);
	if ((line[0] == 'P') && (line[1] == '5'))
		cout << "Greyscale images" << endl;
	while (fp.peek() == '#') fp.getline(line, 80); // ignore comments
	fp >> xres >> yres >> maxintensity;
	cout << "Rows : " << xres << endl;
	cout << "Columns : " << yres << endl;
	cout << "Max Value : " << maxintensity << endl;

	//Files for saving intermediate values
	ofstream tFileOld("tFileOld.txt", std::ofstream::trunc);
	ofstream tFileNew("tFileNew.txt", std::ofstream::trunc);
	ofstream tFileBin("tFileNew.bin", std::ofstream::trunc | ios::binary);

	//Quantization starts from here
	unsigned char pack, parta, partb, mask = 15; // 0000 1111
	int min = 256, max = -1, frameSize = 256, pixel = 0, peak = -1;
	double delta = 0, M = 15.0, N = 256*256;
	struct packed p;

	for (i = 0; !fp.eof(); i++)
	{
		//For each frame find the min and max values
		min = 256, max = -1;
		for (j = 0; j < frameSize; j++)
		{
			frame[j] = fp.get();
			pixel = (int)frame[j];
			tFileOld << pixel << "  ";
			if (pixel < min)	min = pixel;
			if (pixel > max)	max = pixel;
			if (pixel > peak)	peak = pixel;
		}
		tFileOld << endl;

		delta = (max - min) / M; //compute delta using min and max
		tFileBin << (unsigned char)min << (unsigned char)max; //Write min and max in the bin file before the frame values
		tFileNew << "Min : " << min << "  " << "Max : " << max << "  " << "Delta : " << delta << endl;

		//Quantize the frame
		for (j = 0; j < frameSize; j++)
		{
			quantizedFrame[j] = round(((int)frame[j] - min) / delta);
			tFileNew << quantizedFrame[j] << "  ";
		}
		tFileNew << endl;

		//Pack two samples to one
		for (j = 0; j < frameSize; j+=2)
		{
			p.a = quantizedFrame[j];
			p.b = quantizedFrame[j + 1];
			pack = (p.a << 4) | p.b;
			tFileBin << pack; //write the packed byte to the bin file
		}
	}

	fp.close();
	tFileOld.close();
	tFileNew.close();
	tFileBin.close();

	system("Pause");
	return 0;
}

