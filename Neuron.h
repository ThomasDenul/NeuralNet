#pragma once
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

struct Connection
{
	double weight1;
	double deltaWeight1;
	double weight2;
	double deltaWeight2;
};

class Neuron;
typedef vector<Neuron> Layer;

class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	void setOutputVal1(double val) { m_outputVal1 = val; }
	void setOutputVal2(double val) { m_outputVal2 = val; }
	double getOutputVal1() const { return m_outputVal1; }
	double getOutputVal2() const { return m_outputVal2; }
	void feedForward(const Layer &prevLayer);
	void calcOutputGradients(double targetVal1, double targetVal2);
	void calcHiddenGradients(const Layer &nextLayer);
	void updateInputWeights(Layer &prevLayer);

private:
	static double eta;  // [0.0 to 1.0] overall net training rate
	static double alpha;  // [0.0 to n] multiplier to the last weight change (momentum)
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	static double randomWeight() { return rand() / double(RAND_MAX); }
	double sumDOW1(const Layer &nextLayer) const;
	double sumDOW2(const Layer &nextLayer) const;
	double m_outputVal1;
	double m_outputVal2;
	vector<Connection> m_outputWeights;
	unsigned m_myIndex;
	double m_gradient1;
	double m_gradient2;
};

double Neuron::eta = 0.15; // overall net learning rate, [0.0 to 1.0]
double Neuron::alpha = 0.5;  // momentum, multiplier of last deltaWeight, [0.0 to n]


void Neuron::updateInputWeights(Layer &prevLayer)
{

	for (unsigned n = 0; n < prevLayer.size(); ++n)
	{
		Neuron &neuron = prevLayer[n];
		double oldDeltaweight1 = neuron.m_outputWeights[m_myIndex].deltaWeight1;
		double oldDeltaweight2 = neuron.m_outputWeights[m_myIndex].deltaWeight2;

		double newDeltaWeight1 =
			eta
			*neuron.getOutputVal1()
			*m_gradient1
			// adds momentum = a fraction of the previous delta weight
			+ alpha
			* oldDeltaweight1;

		double newDeltaWeight2 = eta * neuron.getOutputVal2() * m_gradient2 + alpha * oldDeltaweight2;

		neuron.m_outputWeights[m_myIndex].deltaWeight1 = newDeltaWeight1;

		neuron.m_outputWeights[m_myIndex].weight1 += newDeltaWeight1;
		neuron.m_outputWeights[m_myIndex].deltaWeight2 = newDeltaWeight2;

		neuron.m_outputWeights[m_myIndex].weight2 += newDeltaWeight2;
	}
}

double Neuron::sumDOW1(const Layer &nextLayer) const
{
	double sum = 0.0;

	for (unsigned n = 0; n < nextLayer.size() - 1; ++n)
	{
		sum += m_outputWeights[n].weight1 * nextLayer[n].m_gradient1;
		sum = double(int(sum) % 1000000);

	}

	return sum;
}

double Neuron::sumDOW2(const Layer &nextLayer) const
{
	double sum = 0.0;

	for (unsigned n = 0; n < nextLayer.size() - 1; ++n)
	{
		sum += m_outputWeights[n].weight2 * nextLayer[n].m_gradient2;
		sum = double(int(sum) % 1000000);
	}

	return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
	double dow1 = sumDOW1(nextLayer);
	double dow2 = sumDOW2(nextLayer);
	m_gradient1 = dow1 * Neuron::transferFunctionDerivative(m_outputVal1);
	m_gradient2 = dow2 * Neuron::transferFunctionDerivative(m_outputVal2);
}

void Neuron::calcOutputGradients(double targetVal1, double targetVal2)
{
	double delta1 = targetVal1 - m_outputVal1;
	m_gradient1 = delta1 * Neuron::transferFunctionDerivative(m_outputVal1);

	double delta2 = targetVal2 - m_outputVal2;
	m_gradient2 = delta2 * Neuron::transferFunctionDerivative(m_outputVal2);
}

double Neuron::transferFunction(double x)
{
	// tanh - output range [-1.0 to 1.0]
	return (tanh(x));
}

double Neuron::transferFunctionDerivative(double x)
{
	// tanh derivative
	return 1 - (tanh(x) * tanh(x));
}

void Neuron::feedForward(const Layer &prevLayer)
{
	double sum1 = 0.0;
	double sum2 = 0.0;

	for (unsigned n = 0; n < prevLayer.size(); ++n)
	{
		sum1 += prevLayer[n].getOutputVal1() *
			prevLayer[n].m_outputWeights[m_myIndex].weight1;

		sum2 += prevLayer[n].getOutputVal2() *
			prevLayer[n].m_outputWeights[m_myIndex].weight2;
	}

	m_outputVal1 = Neuron::transferFunction(sum1);

	m_outputVal2 = Neuron::transferFunction(sum2);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned c = 0; c < numOutputs; ++c)
	{
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight1 = randomWeight();
		m_outputWeights.back().weight2 = randomWeight();
	}

	m_myIndex = myIndex;
}