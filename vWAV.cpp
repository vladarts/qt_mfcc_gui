#include "vWAV.h"

vWAV::vWAV(void)
{

}

vWAV::~vWAV(void)
{
}


bool vWAV::Open( char* Path )
{
	if (fopen_s(&file, Path, "rb") != 0)
		return false;
	header = (WAVHEADER *)malloc(sizeof(WAVHEADER));
	fread_s(header, sizeof(WAVHEADER), sizeof(WAVHEADER), 1, file);
	if (header->format[1] != FORMAT_WAV[1] && header->format[2] != FORMAT_WAV[2] && header->format[3] != FORMAT_WAV[3] && header->format[4] != FORMAT_WAV[4])
	{
		this->CloseFile();
		return false;
	}
	return true;
}

bool vWAV::GetSamples( int count, long int* buffer )
{
	if (feof(file) || file == NULL)
		return false;
	memset(buffer,0,sizeof(long int) * count);

	long int buf = 0;
	for (int i = 0; i < count && (!feof(file)); i++)
	{
		fread(&buf, header->bitsPerSample/8, 1, file);
		buffer[i] = buf;
		buf = 0;
	}

	return true;
}

int vWAV::GetSampleRate()
{
	return header->sampleRate;
}

int vWAV::GetNumberOfChannels()
{
	return header->numChannels;
}

int vWAV::GetCountOfSamples()
{
	return header->subchunk2Size/(header->bitsPerSample/8);
}

int vWAV::GetBitPerSample()
{
	return header->bitsPerSample;
}

bool vWAV::CloseFile()
{
	if (file == 0)
		return false;
	fclose(file);
	return true;
}

void vWAV::Terminate()
{
	this->CloseFile();
	free(header);
}

bool vWAV::EndOfWAV()
{
	return feof(file);
}


#ifdef _IOSTREAM_

void vWAV::PrintInfo()
{
	std::cout << "File info: \n";
	std::cout << "Sample rate: \t\t" << this->GetSampleRate() << ";\n";
	std::cout << "Bit per sample: \t" << this->GetBitPerSample() << ";\n";
	std::cout << "Number of channels: \t" << this->GetNumberOfChannels() << ";\n";
	std::cout << "Number of samples: \t" << this->GetCountOfSamples() << ";\n";
	std::cout << "Samples on channel: \t" << this->GetCountOfSamples() / this->GetNumberOfChannels() << ";\n";
}

#endif