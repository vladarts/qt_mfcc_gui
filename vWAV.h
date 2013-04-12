#pragma once
#include <QtGui/QApplication>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

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
    qint32 chunkSize;

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
    qint32 subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    qint16 audioFormat;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    qint16 numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    qint32 sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    qint32 byteRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    qint16 blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    qint16 bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    qint32 subchunk2Size;

    // Далее следуют непосредственно Wav данные.
};

const char FORMAT_WAV[] = {87, 65, 86, 69};

class vWAV
{
public:
	FILE *file;
	WAVHEADER *header;

	bool Open(char* Path);

	bool GetSamples(int count, long int* buffer);

	int GetSampleRate();

	int GetNumberOfChannels();

	int GetCountOfSamples();

	int GetBitPerSample();

	bool CloseFile();

	void Terminate();

	bool EndOfWAV();

#ifdef _IOSTREAM_
	void PrintInfo();
#endif

	vWAV(void);
	~vWAV(void);
};

