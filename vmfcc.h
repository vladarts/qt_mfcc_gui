#ifndef VMFCC_H
#define VMFCC_H

#define CONSOLE_OUT1

#include "fftw3.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#ifdef CONSOLE_OUT
#include <ctime>
#endif

using namespace std;

#define PI 3.1415926536

#define FRAME_TIME 0.5



class VMFCC
{
private:
    double* GetWindow(int size);
	int _countIn;
	int _countOut;
	double B(double b);
	double Bm(double b);
	static double round (double value)
	{
		return (double) floor(value + 0.5);
	}
public:
    double* in;
    double* out;
	double* window;
	int freq;
    fftw_complex* outFFT;

	void mfcc_SetCountOfMFCC(int count);

    void vmfcc_execute();

    void vmfcc_fft_execute(int CountIn, double *In, fftw_complex *Out);

    void vmfcc_terminate();

    void vmfcc_init(int CountIn, long int* In, int CountOut, int Freq);

	void vmfcc_updateBuffer(long int* In);

	static int mfcc_GetOptimalCountIn(int sampleRate)
	{
		return (int)pow(2,round(log(FRAME_TIME * sampleRate) / log((double)2)));
	}

    VMFCC();
};

#endif // VMFCC_H
