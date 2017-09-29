#pragma once
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

class TrainingData
{
public:
	TrainingData(const string filename);
	bool isEof() { return m_trainingDataFile.eof(); }
	~TrainingData();
	string wave;
	bool readHeader(int &ChunkSize, char &WeirdCharThing, int &SubChunk1Size, short &AudioFormat, short &NumChannels, int &SampleRate, int &ByteRate, short &BlockAlign, short &BitsPerSample, int &SubChunk2Size);

	unsigned getNextInputs(vector<double> &inputVals, double &prevOutputVal, int range, int inputNr);
	unsigned getTargetOutputs(vector<double> &targetOutputVals, int range);

private:
	ifstream m_trainingDataFile;
	unsigned short toReadShort;
	char toReadChar;
	int toReadInt;
};

bool TrainingData::readHeader(int &ChunkSize, char &WeirdCharThing, int &SubChunk1Size, short &AudioFormat, short &NumChannels, int &SampleRate, int &ByteRate, short &BlockAlign, short &BitsPerSample, int &SubChunk2Size)
{
	wave.clear();
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&ChunkSize), sizeof(toReadInt));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	wave.push_back(toReadChar);
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	wave.push_back(toReadChar);
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	wave.push_back(toReadChar);
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	wave.push_back(toReadChar);
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&WeirdCharThing), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&SubChunk1Size), sizeof(toReadInt));
	m_trainingDataFile.read((char*)(&AudioFormat), sizeof(toReadShort));
	m_trainingDataFile.read((char*)(&NumChannels), sizeof(toReadShort));
	m_trainingDataFile.read((char*)(&SampleRate), sizeof(toReadInt));
	m_trainingDataFile.read((char*)(&ByteRate), sizeof(toReadInt));
	m_trainingDataFile.read((char*)(&BlockAlign), sizeof(toReadShort));
	m_trainingDataFile.read((char*)(&BitsPerSample), sizeof(toReadShort));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&toReadChar), sizeof(toReadChar));
	m_trainingDataFile.read((char*)(&SubChunk2Size), sizeof(toReadInt));

	if (wave == "WAVE")
		return true;
	else
		return false;
}

TrainingData::TrainingData(const string filename)
{
	m_trainingDataFile.open(filename.c_str(), std::ios::binary);
	if (m_trainingDataFile.fail())
	{
		std::cout << "Couldn't load file\n";
	}
}

TrainingData::~TrainingData()
{
	m_trainingDataFile.close();
}

unsigned TrainingData::getNextInputs(vector<double> &inputVals, double &prevOutputVal, int range, int inputNr)
{
	if (inputVals.size() != 0)
	{
		inputVals.erase(inputVals.begin());
		inputVals.push_back(prevOutputVal);
	}
	else
	{
		while (inputVals.size() < inputNr)
		{
			m_trainingDataFile.read((char*)(&toReadShort), sizeof(toReadShort));
			inputVals.push_back(double(toReadShort) / double(range));
		}
	}

	return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals, int range)
{
	targetOutputVals.clear();

	m_trainingDataFile.read((char*)(&toReadShort), sizeof(toReadShort));
	targetOutputVals.push_back(double(toReadShort) / double(range));
	return targetOutputVals.size();
}