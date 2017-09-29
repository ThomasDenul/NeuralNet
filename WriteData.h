#pragma once
#include <vector>
#include <fstream>
#include <iostream>

using namespace  std;

class WriteData
{
public:
	WriteData(const string filename);
	~WriteData();
	void writeHeader(int ChunkSize, char WeirdCharThing, int SubChunk1Size, short AudioFormat, short NumChannels, int SampleRate, int ByteRate, short BlockAlign, short BitsPerSample, int SubChunk2Size);

	void WriteOutputs(double output1, double output2);

private:
	ofstream m_outputDataFile;
	char toReadChar;
};

void WriteData::writeHeader(int ChunkSize, char WeirdCharThing, int SubChunk1Size, short AudioFormat, short NumChannels, int SampleRate, int ByteRate, short BlockAlign, short BitsPerSample, int SubChunk2Size)
{
	toReadChar = 'R';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'I';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'F';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'F';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	m_outputDataFile.write((char*)(&ChunkSize), sizeof(int));
	toReadChar = 'W';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'A';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'V';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'E';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'f';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'm';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 't';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = WeirdCharThing;
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	m_outputDataFile.write((char*)(&SubChunk1Size), sizeof(int));
	m_outputDataFile.write((char*)(&AudioFormat), sizeof(short));
	m_outputDataFile.write((char*)(&NumChannels), sizeof(short));
	m_outputDataFile.write((char*)(&SampleRate), sizeof(int));
	m_outputDataFile.write((char*)(&ByteRate), sizeof(int));
	m_outputDataFile.write((char*)(&BlockAlign), sizeof(short));
	m_outputDataFile.write((char*)(&BitsPerSample), sizeof(short));
	toReadChar = 'd';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'a';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 't';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	toReadChar = 'a';
	m_outputDataFile.write((char*)(&toReadChar), sizeof(toReadChar));
	m_outputDataFile.write((char*)(&SubChunk2Size), sizeof(int));
}

WriteData::WriteData(const string filename)
{
	m_outputDataFile.open(filename.c_str() + string(".wav"), ios::out | ios::binary);
	if (m_outputDataFile.fail())
	{
		std::cout << "Couldn't create write file\n";
	}
}

WriteData::~WriteData()
{
	m_outputDataFile.close();
}

void WriteData::WriteOutputs(double output1, double output2)
{
	unsigned short outputValue1 = output1;
	unsigned short outputValue2 = output2;

	if (output2 != 65536)
	{
		int p = 1;
	}

	m_outputDataFile.write((char*)&outputValue1, sizeof(unsigned short));
	m_outputDataFile.write((char*)&outputValue2, sizeof(unsigned short));
}