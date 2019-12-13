#define _CRT_SECURE_NO_WARNINGS
#define MAKE_FILE		1

#include <stdio.h>
#include "ProcBuffers.h"


ILRMA *iip_AUX;
#if MAKE_FILE == 1
FILE **IVA;
double **out_buff;
short **IVA_out;
#endif
double **input_temp;
double **output;

ProcBuffers::ProcBuffers()
{
	int i, ch;

	iip_AUX = new ILRMA();

	input_temp = new double*[Nch];
	output = new double*[Nch];
#if MAKE_FILE == 1
	out_buff = new double*[Nch];
	IVA_out = new short*[Nch];
#endif
	for ( ch = 0; ch < Nch; ch++)
	{
		input_temp[ch] = new double[nWin];
		output[ch] = new double[BufferSize];
#if MAKE_FILE == 1
		out_buff[ch] = new double[BufferSize];
		IVA_out[ch] = new short[BufferSize];
#endif
		for ( i = 0; i < nWin; i++)
		{
			input_temp[ch][i] = 0.0;
		}
	}

#if MAKE_FILE == 1
	char file_name[2][500];
	IVA = new FILE*[Nch];
	for (ch = 0; ch < Nch; ch++)
	{
		sprintf(file_name[0], ".\\output\\IVA_ch%d.pcm", ch + 1);
		IVA[ch] = fopen(file_name[0], "wb");
	}
#endif


}

ProcBuffers::~ProcBuffers()
{
	int ch;

	for ( ch = 0; ch < Nch; ch++)
	{
		delete[] input_temp[ch];
		delete[] output[ch];
#if MAKE_FILE == 1
		delete[] out_buff[ch];
		delete[] IVA_out[ch];
#endif
	}
	delete[] input_temp;
	delete[] output;
#if MAKE_FILE == 1
	delete[] out_buff;
	delete[] IVA_out;
#endif

#if MAKE_FILE == 1
	char file_name[2][500];
	for (ch = 0; ch < Nch; ch++)
	{
		fclose(IVA[ch]);
		sprintf(file_name[0], ".\\output\\IVA_ch%d.pcm", ch + 1);
		sprintf(file_name[1], ".\\output\\IVA_ch%d.wav", ch + 1);
		pcm2wav(file_name[0], file_name[1], (long)(SamplingFreq));
		remove(file_name[0]);
	}
	delete[] IVA;
	
#endif

	delete iip_AUX;
}

void ProcBuffers::Process(double **input, int Nframe)
{
	int i, j, ch;
	for ( ch = 0; ch < Nch; ch++)
	{
		for ( i = 0; i < 3 * BufferSize; i++)
		{
			input_temp[ch][i] = input_temp[ch][BufferSize + i];
		}
		for (i = 0; i < BufferSize; i++)
		{
			input_temp[ch][3 * BufferSize + i] = input[ch][i];
		}
	}
	if (Nframe >= 3)
	{
		iip_AUX->ILRMA_lemma(input_temp, Nframe, output);

#if MAKE_FILE == 1
		for (i = 0; i < Nch; i++)
		{
			for (j = 0; j < BufferSize; j++)
			{
				out_buff[i][j] = output[i][j] * 32768.0;
				IVA_out[i][j] = (short)(out_buff[i][j]);
			}
			fwrite(IVA_out[i], sizeof(short), BufferSize, IVA[i]);
		}
#endif
	}

}