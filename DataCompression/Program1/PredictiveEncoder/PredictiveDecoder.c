
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("Usage: The program takes only two arguments. Input and output file names\n");

	long i, previous = 0, current = 0, diff = 0, max = 0, min = 0, maxDiff = 0, minDiff = 0;
	float sampleCount = 0, total = 0, totalDiff = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	// argv[1] is the input file name, so open it in read mode
	FILE *file1 = fopen(argv[1], "r");
	FILE *file2 = fopen(argv[2], "w");

	/* fopen returns 0, the NULL pointer, on failure */
	if (file1 == 0)
	{
		printf("Could not open file %s\n", argv[1]);
	}
	else if (file2 == 0)
	{
		printf("Could not open file %s\n", argv[2]);
	}
	else
	{
		printf("The Input file name: %s\n", argv[1]);
		printf("The Output file name: %s\n", argv[2]);

		for (i = 1; i <= 5 && ((read = getline(&line, &len, file1)) != -1); i++)
		{
			fprintf(file2, "%s", line);
		}
		while ((read = getline(&line, &len, file1)) != -1)
		{
			if (sscanf(line, "%ld\n", &current) == EOF)
			{
				printf("WARNING: Incorrect value for device\n");
			}
			diff = current + previous;
			fprintf(file2, "%ld\n", diff);
			previous = current;
			sampleCount++;
			if (min > current) min = current;
			if (max < current) max = current;
			if (minDiff > diff) minDiff = diff;
			if (maxDiff < diff) maxDiff = diff;
			total += current;
			totalDiff += diff;
		}

		printf("The number of samples: %f\n", sampleCount);
		printf("The max value: %ld\n", max);
		printf("The min value: %ld\n", min);
		printf("The average value: %f\n", total / sampleCount);
		printf("The max difference value: %ld\n", maxDiff);
		printf("The min difference value: %ld\n", minDiff);
		printf("The average difference value: %f\n", totalDiff / sampleCount);
	}

	fclose(file1);
	fclose(file2);

	return 0;
}

