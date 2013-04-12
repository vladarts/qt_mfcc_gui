#pragma once
#include <QtGui/QApplication>
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
    qint32 chunkSize;

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
    qint32 subchunk1Size;

    // ����� ������, ������ ������ ����� �������� ����� http://audiocoding.ru/wav_formats.txt
    // ��� PCM = 1 (�� ����, �������� �����������).
    // ��������, ������������ �� 1, ���������� ��������� ������ ������.
    qint16 audioFormat;

    // ���������� �������. ���� = 1, ������ = 2 � �.�.
    qint16 numChannels;

    // ������� �������������. 8000 ��, 44100 �� � �.�.
    qint32 sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    qint32 byteRate;

    // numChannels * bitsPerSample/8
    // ���������� ���� ��� ������ ������, ������� ��� ������.
    qint16 blockAlign;

    // ��� ���������� "��������" ��� �������� ��������. 8 ���, 16 ��� � �.�.
    qint16 bitsPerSample;

    // ���������� "data" �������� �����-������ � �� ������.

    // �������� ������� "data"
    // (0x64617461 � big-endian �������������)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // ���������� ���� � ������� ������.
    qint32 subchunk2Size;

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

