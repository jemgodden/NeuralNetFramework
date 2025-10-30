#pragma once

#include <string>

#include "../matrix/matrix.h"


class Network {
    private:
        int _inputNodes;

        int _hiddenNodes;

        int _outputNodes;

        Matrix* _inputToHiddenWeights;

        Matrix* _inputToHiddenBiases;

        Matrix* _hiddenToOutputWeights;

        Matrix* _hiddenToOutputBiases;

        Matrix* _hiddenLayerOutput;

        void _initialiseMatrices();

        static std::tuple<Matrix*, Matrix*> _splitLabelsMatrix(const Matrix *inputMatrix, int labelColumnIndex);

        void _setHiddenLayerOutput(Matrix *hiddenLayerOutput);

        Matrix* _feedForward(const Matrix *trainMatrix);

        void _backPropagate(const Matrix* trainMatrix, const Matrix* actualLabelMatrix, const Matrix* predictedLabelMatrix, double learningRate);

    public:
        Network(int inputNodes, int outputNodes, int hiddenNodes=10);
        
        ~Network();

        int inputNodes() const;

        int outputNodes() const;

        int hiddenNodes() const;

        void train(const Matrix* trainMatrix, int labelColumnIndex=-1, int epochs=4000, double learningRate=0.1);

        double predict(const Matrix* testMatrix, int labelColumnIndex=-1);

        void save(const std::string &modelName="") const;

        void load(const std::string &modelName);
};
