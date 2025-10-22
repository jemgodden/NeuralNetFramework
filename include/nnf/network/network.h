#pragma once


#include "../matrix/matrix.h"


class Network {
    private:
        int _inputNodes;
        int _hiddenLayers;
        int _hiddenNodes;
        int _outputNodes;
        double _learningRate;
        Matrix* _hiddenWeights;
        Matrix* _outputWeights;

    public:
        Network(int inputNodes, int hiddenLayers, int hiddenNodes, int outputNodes, double learningRate);
        
        ~Network();
        
        int inputNodes() const;
        
        int hiddenLayers() const;

        int hiddenNodes() const;

        int outputNodes() const;

        double learningRate() const;

        void info();

        void save();

        void load();

        void train();

        void predict();
};
