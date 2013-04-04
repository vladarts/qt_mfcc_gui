#include <QApplication>
#include <iostream>
#include <fftw3.h>

#define PI 3.1415926536

using namespace std;
struct WAVHEADER
{
    // WAV-формат начинается с RIFF-заголовка:

    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunkId[4];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    unsigned long chunkSize;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    char format[4];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:

    // Содержит символы "fmt "
    // (0x666d7420 в big-endian представлении)
    char subchunk1Id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    unsigned long subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    unsigned short audioFormat;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    unsigned short numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    unsigned long sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    unsigned long byteRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    unsigned short blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    unsigned short bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    unsigned long subchunk2Size;

    // Далее следуют непосредственно Wav данные.
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FILE *file;
    int err;
    err = fopen_s(&file, "D:\\16_8.wav", "rb");

    WAVHEADER *header = (WAVHEADER *)malloc(sizeof(WAVHEADER));

    fread_s(header, sizeof(WAVHEADER), sizeof(WAVHEADER), 1, file);

    quint16* Data=new quint16 [header->subchunk2Size/(header->bitsPerSample/8)];
    fread(Data,header->bitsPerSample/8,header->subchunk2Size/(header->bitsPerSample/8),file);


    quint16 data[20];
    for (int i = 0; i < 20; i++)
    {
        data[i] = Data[i*20000];
    }

    double *in = (double *)malloc(sizeof(double)*1024);
    fftw_complex* out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex)*1024);


    fftw_plan fftPlan = fftw_plan_dft_r2c_1d(1024,in,out,FFTW_ESTIMATE);
    fftw_execute(fftPlan);

    fftw_destroy_plan(fftPlan);


    return a.exec();
}
