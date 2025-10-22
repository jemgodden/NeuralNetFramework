#include "../../include/nnf/network/network.h"
#include "../../include/nnf/matrix/matrix.h"


Network::Network(const int inputNodes, const int hiddenLayers, const int hiddenNodes, const int outputNodes, const double learningRate) {
    _inputNodes = inputNodes;
    _hiddenLayers = hiddenLayers;
    _hiddenNodes = hiddenNodes;
    _outputNodes = outputNodes;
    _learningRate = learningRate;
    _hiddenWeights = new Matrix(hiddenNodes, inputNodes);
    _outputWeights = new Matrix(outputNodes, hiddenNodes);
};

Network::~Network() {
    
};

int Network::inputNodes() const {
    return _inputNodes;
};

int Network::hiddenLayers() const {
    return _hiddenLayers;
};

int Network::hiddenNodes() const {
    return _hiddenNodes;
};

int Network::outputNodes() const {
    return _outputNodes;
};

double Network::learningRate() const {
    return _outputNodes;
};

void Network::info() {

};

void Network::save() {

};

void Network::load() {

};

void Network::train() {

};

void Network::predict() {

};
