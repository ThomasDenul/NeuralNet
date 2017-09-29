#pragma once
#include "Neuron.h"
#include <sstream>
#include <iostream>

class Net
{
public:
	Net(const vector<unsigned> &topology);
	void feedForward(const vector<double> &inputVals1, const vector<double> &inputVals2);
	void backProp(const vector<double> &targetVals1, const vector<double> &targetVals2);
	void getResults(vector<double> &resultVals1, vector<double> &resultVals2) const;
	double getRecentAverageError1() const { return m_recentAverageError1; }
	double getRecentAverageError2() const { return m_recentAverageError2; }

private:
	vector<Layer> m_layers;
	double m_error1;
	double m_error2;
	double m_recentAverageError1;
	double m_recentAverageError2;
	double m_recentAverageSmoothingFactor;
};

void Net::getResults(vector<double> &resultVals1, vector<double> &resultVals2) const
{
	resultVals1.clear();
	resultVals2.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n)
	{
		resultVals1.push_back(m_layers.back()[n].getOutputVal1());
		resultVals2.push_back(m_layers.back()[n].getOutputVal2());
	}
}

void Net::backProp(const vector<double> &targetVals1, const vector<double> &targetVals2)
{
	// Calculate overall net error (RMS (Root Mean Square Error) of output neuron errors)

	Layer &outputLayer = m_layers.back();
	m_error1 = 0.0;
	m_error2 = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		double delta = targetVals1[n] - outputLayer[n].getOutputVal1();
		m_error1 += delta * delta;
		delta = targetVals2[n] - outputLayer[n].getOutputVal2();
		m_error2 += delta * delta;
	}
	m_error1 /= outputLayer.size() - 1; // get average error squared
	m_error1 = sqrt(m_error1); // RMS
	m_error2 /= outputLayer.size() - 1;
	m_error2 = sqrt(m_error2);

	m_recentAverageError1 =
		(m_recentAverageError1 * m_recentAverageSmoothingFactor + m_error1)
		/ (m_recentAverageSmoothingFactor + 1.0); // Nothing to do with the neural net. Just to check how well the net has been doing over the last several dozen training samples.
	m_recentAverageError2 =
		(m_recentAverageError2 * m_recentAverageSmoothingFactor + m_error2)
		/ (m_recentAverageSmoothingFactor + 1.0);


	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		outputLayer[n].calcOutputGradients(targetVals1[n], targetVals2[n]);
	}


	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
	{
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); ++n)
		{
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}


	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
	{
		Layer &layer = m_layers[layerNum];
		Layer &prevLayer = m_layers[layerNum - 1];

		for (unsigned n = 0; n < layer.size() - 1; ++n)
		{
			layer[n].updateInputWeights(prevLayer);
		}
	}
}

void Net::feedForward(const vector<double> &inputVals1, const vector<double> &inputVals2)
{
	assert(inputVals1.size() == m_layers[0].size() - 1); // To check whether there's no different amout of input values than neurons (-1 for bias neuron)
	assert(inputVals2.size() == m_layers[0].size() - 1);

	for (unsigned i = 0; i < inputVals1.size(); ++i)
	{
		m_layers[0][i].setOutputVal1(inputVals1[i]);
	}
	for (unsigned i = 0; i < inputVals2.size(); ++i)
	{
		m_layers[0][i].setOutputVal2(inputVals2[i]);
	}

	//Forward propagate
	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum)
	{
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n)
		{
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	{
		m_layers.push_back(Layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		//We have new layer. Now fill it with neurons and add bias neuron:
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)
		{
			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
			cout << "Made a Neuron!" << endl;
		}

		// Force the bias node's output value to 1.0. It's the last neuron created above
		m_layers.back().back().setOutputVal1(1.0);
		m_layers.back().back().setOutputVal2(1.0);
	}
}


void showVectorVals(string lable, vector<double> &v, int range)
{
	cout << lable << " ";
	for (unsigned i = 0; i < v.size(); ++i)
	{
		cout << abs(v[i] * range) << " ";
	}

	cout << '\n';
}