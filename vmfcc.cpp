#include "vmfcc.h"


VMFCC::VMFCC()
{
}

double* VMFCC::GetWindow(int size)
{
    double *tmp = new double [size];
    for(int i = 0; i < size; ++i)
    {
        tmp[i] = (float)i/size;
    }

    double  a0 = 0.35875, a1 = 0.48829, a2 = 0.14128, a3 = 0.01168;
    for(int i = 0; i < size; ++i)
    {
        tmp[i] = (double)(a0 - a1 * cos(2*PI*tmp[i])+a2 * cos(2*PI*2*tmp[i])-a3 * cos(2*PI*3*tmp[i]));
    }
    return tmp;
}

void VMFCC::vmfcc_fft_execute(int CountIn, double *In, fftw_complex *Out)
{
    fftw_plan fftPlan = fftw_plan_dft_r2c_1d(CountIn,In,Out,FFTW_ESTIMATE);
    fftw_execute(fftPlan);
    fftw_free(fftPlan);
}

void VMFCC::mfcc_SetCountOfMFCC( int count )
{
	_countOut = count;
	delete[] out;
	out = new double[count];
}

void VMFCC::vmfcc_execute()
{
#ifdef CONSOLE_OUT
	int time = clock();
#endif
	for (int i = 0; i < _countIn; i++)
		in[i] *= window[i];
	fftw_plan fftPlan = fftw_plan_dft_r2c_1d(_countIn,in,outFFT,FFTW_ESTIMATE);
	fftw_execute(fftPlan);
	fftw_destroy_plan(fftPlan);
	int N = _countIn / 2 + 1; // Number of FFT results
	int* f = new int[_countOut + 2]; // freq graduation
	int f1 = 0;
	int fh = freq * (_countOut - 1) / _countOut;
	int Fs = fh - f1;
	for (int i = 0; i < _countOut + 2; i++)
	{
		f[i] = (int)(Bm(B(f1)+i*(B(fh)-B(f1))/(_countOut + 1)) * N / Fs);
	}
	double* energys = new double[_countOut];
	memset(energys, 0, sizeof(double) * _countOut);
	for (int i = 0; i < _countOut; i++)
	{
		for (int k = f[i]; k < f[i+1] && k < N; k++)
			energys[i] += (((double)outFFT[k][0] * outFFT[k][0] + (double)outFFT[k][1] * outFFT[k][1]) * (k - f[i]) / (f[i+1] - f[i]));
		for (int k = f[i+1]; k <= f[i+2] && k < _countIn; k++)
			energys[i] += (((double)outFFT[k][0] * outFFT[k][0] + (double)outFFT[k][1] * outFFT[k][1]) * (f[i+2] - k) / (f[i+2] - f[i+1]));
	}
	for (int i = 0; i < _countOut; i++)
	{
		energys[i] = log(energys[i]);
	}
	fftw_plan planDCT = fftw_plan_r2r_1d(_countOut, energys, out, FFTW_REDFT10, FFTW_ESTIMATE);
	fftw_execute(planDCT);
	fftw_destroy_plan(planDCT);

	delete[] energys;
	delete[] f;

#ifdef CONSOLE_OUT
	cout << "MFCC result:" << endl;
	for (int i = 0; i < _countOut; i++)
		cout << i << ": \t" << out[i] << endl;
	cout << "Sample Freq: \t \t" << freq << endl;
	cout << "time of counting: \t" << ((float)(clock() - time))/CLOCKS_PER_SEC << endl;
	cout << "time of playing: \t" << (float)_countIn/freq << endl;
#endif
}

void VMFCC::vmfcc_terminate()
{
	delete[] this->in;
	delete[] this->out;
	delete[] this->window;
	fftw_free(outFFT);
}

void VMFCC::vmfcc_init( int CountIn, long int* In, int CountOut, int Freq )
{
	in = new double[CountIn];
	out = new double[CountOut];
	outFFT = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (CountIn / 2 + 1));
	_countIn = CountIn;
	for (int i = 0; i < CountIn; i++)
		in[i] = (double)In[i];
	_countOut = CountOut;
	freq = Freq;
	window = GetWindow(CountIn);
}

void VMFCC::vmfcc_updateBuffer( long int* In )
{
	for (int i = 0; i < _countIn; i++)
		in[i] = (double)In[i];
}

double VMFCC::B( double b )
{
	return 1125*log(1+b/700);
}

double VMFCC::Bm( double b )
{
	return 700*(exp(b/1125)-1);
}
