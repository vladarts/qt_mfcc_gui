#ifndef VMFCC_H
#define VMFCC_H

#include <fftw3.h>
#include <math.h>

#define PI 3.1415926536

class VMFCC
{
private:
    int* GetWindow(int size);
public:
    int* in;
    int* out;
    int* outFFT;

    void execute();

    void fft_execute(int CountIn, int *In, fftw_complex *Out);

    void vmfcc_terminate()
    {
        delete(this->in);
        delete(this->out);
        delete(this->outFFT);
    }

    VMFCC init(int CountIn, int* In, int CountOut);
    VMFCC();
};

#endif // VMFCC_H
