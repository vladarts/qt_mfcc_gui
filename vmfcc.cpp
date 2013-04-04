#include "vmfcc.h"


VMFCC::VMFCC()
{
}

int* VMFCC::GetWindow(int size)
{
    float *tmp = new float [size];
    for(int i = 0; i < size; ++i)
    {
        tmp[i] = (float)i/size;
    }

    float  a0 = 0.35875f, a1 = 0.48829f, a2 = 0.14128f, a3 = 0.01168f;
    for(int i = 0; i < size; ++i)
    {
        tmp[i] = (float)(a0 - a1 * cos(2*PI*tmp[i])+a2 * cos(2*PI*2*tmp[i])-a3 * cos(2*PI*3*tmp[i]));
    }

    //    int * res = new int [size];

    //	for(int i = 0; i < size; ++i)
    //	{
    //		res[i] = FLT2FX16(tmp[i]);
    //	}
    //	delete [] tmp;

    return (int *)tmp;
}

void VMFCC::fft_execute(int CountIn, int *In, fftw_complex *Out)
{
    fftw_plan fftPlan = fftw_plan_dft_r2c_1d(CountIn,(double *)In,Out,FFTW_ESTIMATE);
    fftw_execute(fftPlan);
    fftw_free(fftPlan);
}

void VMFCC::execute()
{

}
