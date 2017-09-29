#include "Net.h"
#include "TrainingData.h"
#include "WriteData.h"

int main()
{
	int Range = 65536;
	int nrOfInputs = 23;

	vector<unsigned> topology;
	for (int i = 0; i < nrOfInputs; i++)
	{
		topology.push_back(nrOfInputs - i);
	}

	bool istextfile = false;
	string trainingData;
	string fileType;
	bool quit = false;
	int stringSize;
	int val;
		while (!istextfile)
		{
			fileType.clear();
			cout << "Training file or quit?" << '\n';
			getline(cin, trainingData);

			stringSize = trainingData.size();

			for (int i = 1; i <= 4; ++i)
			{
				if (stringSize >= 4)
				{
					val = stringSize - i;
					fileType.push_back(trainingData[val]);
				}
			}

			if (trainingData == "quit")
			{
				quit = true;
				break;
			}
			else if (fileType == "vaw." || fileType == "\"vaw")
				istextfile = true;
			else
				cout << "Wrong file type! .wav file required!" << '\n';
		}

		if (!quit)
		{
			TrainingData trainData(trainingData);
			WriteData writeData("WriteTest");

			int chunkSize;
			int subChunk1Size;
			short audioFormat;
			short numChannels;
			int sampleRate;
			int byteRate;
			short blockAlign;
			short bitsPerSample;
			int subChunk2Size;
			char weirdCharThing;

			Net myNet(topology);

			vector<double> inputVals1, inputVals2, targetVals1, targetVals2, resultVals1, resultVals2;
			targetVals1.push_back(0.0);
			targetVals2.push_back(0.0);
			int trainingPass = 0;

			trainData.readHeader(chunkSize, weirdCharThing, subChunk1Size, audioFormat, numChannels, sampleRate, byteRate, blockAlign, bitsPerSample, subChunk2Size);
			writeData.writeHeader(chunkSize, weirdCharThing, subChunk1Size, audioFormat, numChannels, sampleRate, byteRate, blockAlign, bitsPerSample, subChunk2Size);

			while (!trainData.isEof())
			{
				++trainingPass;

				//Get new input data and feed it forward:
				if (trainData.getNextInputs(inputVals1, targetVals1[0], Range, nrOfInputs) != topology[0])
				{
					break;
				}
				if (trainData.getNextInputs(inputVals2, targetVals2[0], Range, nrOfInputs) != topology[0])
				{
					break;
				}
				myNet.feedForward(inputVals1, inputVals2);

				//Collect the net's actual results:
				myNet.getResults(resultVals1, resultVals2);
				writeData.WriteOutputs(
					((resultVals1.back() +1)/2)* Range, ((resultVals2.back()+1)/2) * Range);

				//Train the net what the outputs should have been:
				trainData.getTargetOutputs(targetVals1, Range);
				trainData.getTargetOutputs(targetVals2, Range);
				assert(targetVals1.size() == topology.back());

				myNet.backProp(targetVals1, targetVals2);
			}

			cout << endl << "Done" << endl;
		}
		getchar();
		getchar();
}