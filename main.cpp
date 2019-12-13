#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include "ProcBuffers.h"

using namespace std;
void usage(void)
{
	// Error function in case of incorrect command-line
	// argument specifications
	cout << "\nuseage: AUXIVA_Online_APIver.exe x_3x3_S(N)_ch1.wav x_3x3_S(N)_ch2.wav x_3x3_S(N)_ch3.wav \n";
	cout << "N is # of mixing signal.\n";
	exit(0);
}


int main(int argc, char *argv[])
{
	if (argc<3)
	{
		usage();
	}


	ProcBuffers *proc;
	proc = new ProcBuffers();

	int i, j, ch, Nframe;
	char file_name[Nch][50];
	double *signal[Nch];

	// Data reading
	for (ch = 0; ch < Nch; ch++)
	{
		sprintf(file_name[ch], argv[ch + 1]);
		cout << "reading " << file_name[ch] << endl;
		signal[ch] = wavread(file_name[ch]);
	}

	int input_length = length(signal[1]);
	double **Input;
	Input = new double *[Nch];
	for (int i = 0; i < Nch; i++)
	{
		Input[i] = new double[BufferSize];
		for (int j = 0; j < BufferSize; j++)
		{
			Input[i][j] = 0;
		}
	}

	int iteration = input_length / BufferSize;
	for (Nframe = 0; Nframe <= iteration; Nframe++)
	{
		for (i = 0; i < Nch; i++)
		{
			for (j = 0; j < BufferSize; j++)
			{
				if (Nframe*BufferSize + j > input_length)
				{
					Input[i][j] = 0;
				}
				else
				{
					Input[i][j] = signal[i][Nframe*BufferSize + j + 1];
				}
			}

		}
		proc->Process(Input, Nframe);
	}

	for (ch = 0; ch < Nch; ch++)
	{
		delete[] Input[ch];
	}
	delete[] Input;

	for (int i = 0; i < Nch; i++)
	{
		free(signal[i]);
	}
	delete proc;

}
