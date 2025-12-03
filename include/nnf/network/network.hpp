#pragma once

#include <string>

#include "../matrix/matrix.hpp"


class Network {
    private:
        virtual Matrix* _feedForward(const Matrix *trainMatrix) = 0;

        virtual void _backPropagate(const Matrix* trainMatrix, const Matrix* actualLabelMatrix, const Matrix* predictedLabelMatrix, double learningRate) = 0;

    protected:
        int _inputNodes;

        int _hiddenNodes;

        Matrix* _inputToHiddenWeights;

        Matrix* _inputToHiddenBiases;

        Matrix* _hiddenToOutputWeights;

        Matrix* _hiddenToOutputBiases;

        Matrix* _hiddenLayerOutput;

        virtual void _initialiseMatrices();

        void _setInputToHiddenWeights(Matrix* newMatrix);

        void _setInputToHiddenBiases(Matrix* newMatrix);

        void _setHiddenToOutputWeights(Matrix* newMatrix);

        void _setHiddenToOutputBiases(Matrix* newMatrix);

        void _setHiddenLayerOutput(Matrix *hiddenLayerOutput);

        static std::tuple<Matrix*, Matrix*> _splitLabelsMatrix(const Matrix *inputMatrix, int labelColumnIndex);

    public:
        explicit Network(int inputNodes, int hiddenNodes=10);

        virtual ~Network();

        int inputNodes() const;

        int hiddenNodes() const;

        virtual void _train(const Matrix* trainMatrix, int labelColumnIndex, int epochs, double learningRate) = 0;

        virtual double _predict(const Matrix* testMatrix, int labelColumnIndex) = 0;

        void save(const std::string &modelName="") const;

        virtual void load(const std::string &modelName);
};

class ClassificationNetwork final : Network {
    private:
        int _outputNodes;

        void _initialiseMatrices() override;

        Matrix* _feedForward(const Matrix *trainMatrix) override;

        void _backPropagate(const Matrix* trainMatrix, const Matrix* actualLabelMatrix, const Matrix* predictedLabelMatrix, double learningRate) override;

        void _train(const Matrix* trainMatrix, int labelColumnIndex, int epochs, double learningRate) override;

        double _predict(const Matrix* testMatrix, int labelColumnIndex) override;

    public:
        ClassificationNetwork(int inputNodes, int outputNodes, int hiddenNodes=10);

        ~ClassificationNetwork() override;

        int outputNodes() const;

        void train(const Matrix* trainMatrix, int labelColumnIndex=-1, int epochs=4000, double learningRate=0.1);

        double predict(const Matrix* testMatrix, int labelColumnIndex=-1);

        void load(const std::string &modelName) override;
};
