#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

struct WAVHEADER
{
    // WAV-������ ���������� � RIFF-���������:

    // �������� ������� "RIFF" � ASCII ���������
    // (0x52494646 � big-endian �������������)
    char chunkId[4];

    // 36 + subchunk2Size, ��� ����� �����:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // ��� ���������� ������ �������, ������� � ���� �������.
    // ����� ������, ��� ������ ����� - 8, �� ����,
    // ��������� ���� chunkId � chunkSize.
    unsigned long chunkSize;

    // �������� ������� "WAVE"
    // (0x57415645 � big-endian �������������)
    char format[4];

    // ������ "WAVE" ������� �� ���� ����������: "fmt " � "data":
    // ���������� "fmt " ��������� ������ �������� ������:

    // �������� ������� "fmt "
    // (0x666d7420 � big-endian �������������)
    char subchunk1Id[4];

    // 16 ��� ������� PCM.
    // ��� ���������� ������ ����������, ������� � ���� �������.
    unsigned long subchunk1Size;

    // ����� ������, ������ ������ ����� �������� ����� http://audiocoding.ru/wav_formats.txt
    // ��� PCM = 1 (�� ����, �������� �����������).
    // ��������, ������������ �� 1, ���������� ��������� ������ ������.
    unsigned short audioFormat;

    // ���������� �������. ���� = 1, ������ = 2 � �.�.
    unsigned short numChannels;

    // ������� �������������. 8000 ��, 44100 �� � �.�.
    unsigned long sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    unsigned long byteRate;

    // numChannels * bitsPerSample/8
    // ���������� ���� ��� ������ ������, ������� ��� ������.
    unsigned short blockAlign;

    // ��� ���������� "��������" ��� �������� ��������. 8 ���, 16 ��� � �.�.
    unsigned short bitsPerSample;

    // ���������� "data" �������� �����-������ � �� ������.

    // �������� ������� "data"
    // (0x64617461 � big-endian �������������)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // ���������� ���� � ������� ������.
    unsigned long subchunk2Size;

    // ����� ������� ��������������� Wav ������.
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

