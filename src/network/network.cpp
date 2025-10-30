#include <iostream>

#include "../../include/nnf/network/network.h"
#include "../../include/nnf/matrix/matrix.h"
#include "../../include/nnf/network/activations.h"
#include "../../include/nnf/network/costs.h"
#include "../../include/nnf/utils/file_writer.h"
#include "../../include/nnf/utils/file_reader.h"


Network::Network(const int inputNodes, const int outputNodes, const int hiddenNodes) {
    _inputNodes = inputNodes;
    _hiddenNodes = hiddenNodes;
    _outputNodes = outputNodes;

    _inputToHiddenWeights = nullptr;
    _inputToHiddenBiases = nullptr;
    _hiddenToOutputWeights = nullptr;
    _hiddenToOutputBiases = nullptr;

    _hiddenLayerOutput = nullptr;
} ;

Network::~Network() {
    free(_inputToHiddenWeights);
    free(_inputToHiddenBiases);
    free(_hiddenToOutputWeights);
    free(_hiddenToOutputBiases);

    if (_hiddenLayerOutput != nullptr) {free(_hiddenLayerOutput);};
};

int Network::inputNodes() const {
    return _inputNodes;
};

int Network::outputNodes() const {
    return _outputNodes;
};

int Network::hiddenNodes() const {
    return _hiddenNodes;
};

void Network::_initialiseMatrices() {
    _inputToHiddenWeights = new Matrix(_hiddenNodes, _inputNodes); // (h, i)
    _inputToHiddenWeights->randomise();
    _inputToHiddenBiases = new Matrix(_hiddenNodes, 1); // (h, 1)
    _inputToHiddenBiases->randomise();
    _hiddenToOutputWeights = new Matrix(_outputNodes, _hiddenNodes); // (o, h)
    _hiddenToOutputWeights->randomise();
    _hiddenToOutputBiases = new Matrix(_outputNodes, 1); // (o, 1)
    _hiddenToOutputBiases->randomise();
};

std::tuple<Matrix*, Matrix*> Network::_splitLabelsMatrix(const Matrix* inputMatrix, const int labelColumnIndex) {
    if (labelColumnIndex == -1) {
        return inputMatrix->colSlice(inputMatrix->cols()-1);
    }
    if (labelColumnIndex < 0 || labelColumnIndex >= inputMatrix->cols()) {
        throw std::invalid_argument("Specified label column index is out of bounds for input matrix.");
    }
    return inputMatrix->colSlice(labelColumnIndex);
};

void Network::_setHiddenLayerOutput(Matrix* hiddenLayerOutput) {
    if (_hiddenLayerOutput != nullptr) {free(_hiddenLayerOutput);};
    _hiddenLayerOutput = hiddenLayerOutput; // (h, n)
};

Matrix* Network::_feedForward(const Matrix* trainMatrix) { // (i, n)
    const Matrix* inputHiddenLayerWeights = _inputToHiddenWeights->dot(trainMatrix); // (h, n)
    Matrix* inputHiddenLayerBiases = inputHiddenLayerWeights->columnwiseAdd(_inputToHiddenBiases); // (h, n)
    inputHiddenLayerBiases->apply(&sigmoid); // (h, n)
    _setHiddenLayerOutput(inputHiddenLayerBiases);
    delete inputHiddenLayerWeights;

    const Matrix* outputHiddenLayerWeights = _hiddenToOutputWeights->dot(inputHiddenLayerBiases); // (o, n)
    Matrix* outputHiddenLayerBiases = outputHiddenLayerWeights->columnwiseAdd(_hiddenToOutputBiases); // (o, n)
    outputHiddenLayerBiases->apply(&sigmoid); // (o, n)
    delete outputHiddenLayerWeights;

    return outputHiddenLayerBiases; // (o, n)
};

void Network::_backPropagate(const Matrix* trainMatrix, const Matrix* actualLabelMatrix, const Matrix* predictedLabelMatrix, const double learningRate) { // (i, n), (n, 1), (n, o)
    // Calculate Errors
    // Calculating output layer errors.
    const Matrix* outputError = categoricalCrossEntropyLossDerivative(actualLabelMatrix, predictedLabelMatrix); // (n, 1)
    const Matrix* predictedLabelMatrixCopy = predictedLabelMatrix->deepCopy(); // (n, o)
    predictedLabelMatrixCopy->apply(&sigmoidDerivative); // (n, o)
    const Matrix* outputDelta = predictedLabelMatrixCopy->columnwiseMultiply(outputError); // (n, o)
    delete predictedLabelMatrixCopy;
    delete outputError;

    // Calculating hidden layer errors.
    const Matrix* hiddenToOutputWeightsCopy = _hiddenToOutputWeights->deepCopy(); // (o, h)
    const Matrix* hiddenError = outputDelta->dot(hiddenToOutputWeightsCopy); // (n, h)
    const Matrix* hiddenLayerOutputCopy = _hiddenLayerOutput->deepCopy(); // (h, n)
    hiddenLayerOutputCopy->apply(&sigmoidDerivative); // (h, n)
    hiddenLayerOutputCopy->transpose(); // (n, h)
    const Matrix* hiddenDelta = hiddenError->multiply(hiddenLayerOutputCopy); // (n, h)
    delete hiddenToOutputWeightsCopy;
    delete hiddenError;
    delete hiddenLayerOutputCopy;

    // Update Weights and Biases
    // Updating hidden-output weights.
    const Matrix* hiddenToOutputWeightsDelta = _hiddenLayerOutput->dot(outputDelta); // (h, o)
    hiddenToOutputWeightsDelta->scale(learningRate); // (h, o)
    hiddenToOutputWeightsDelta->transpose(); // (o, h)
    _hiddenToOutputWeights = _hiddenToOutputWeights->add(hiddenToOutputWeightsDelta); // (o, h)
    delete hiddenToOutputWeightsDelta;

    // Updating hidden-output biases.
    const Matrix* hiddenToOutputBiasesDelta = outputDelta->rowwiseSum(); // (1, o)
    hiddenToOutputBiasesDelta->scale(learningRate); // (1, o)
    hiddenToOutputBiasesDelta->transpose(); // (o, 1)
    _hiddenToOutputBiases = _hiddenToOutputBiases->add(hiddenToOutputBiasesDelta); // (o, 1)
    delete outputDelta;
    delete hiddenToOutputBiasesDelta;

    // Updating input-hidden weights.
    const Matrix* trainMatrixCopy = trainMatrix->deepCopy(); // (i, n)
    const Matrix* inputToHiddenWeightsDelta = trainMatrixCopy->dot(hiddenDelta); // (i, h)
    inputToHiddenWeightsDelta->scale(learningRate); // (i, h)
    inputToHiddenWeightsDelta->transpose(); // (h, i)
    _inputToHiddenWeights = _inputToHiddenWeights->add(inputToHiddenWeightsDelta); // (h, i)
    delete trainMatrixCopy;
    delete inputToHiddenWeightsDelta;

    // Updating input-hidden biases.
    const Matrix* inputToHiddenBiasesDelta = hiddenDelta->columnwiseSum(); // (1, h)
    inputToHiddenBiasesDelta->scale(learningRate); // (1, h)
    inputToHiddenBiasesDelta->transpose(); // (h, 1)
    _inputToHiddenBiases = _inputToHiddenBiases->add(inputToHiddenBiasesDelta); // (h, 1)
    delete hiddenDelta;
    delete inputToHiddenBiasesDelta;
};

void Network::train(const Matrix* trainMatrix, const int labelColumnIndex, const int epochs, const double learningRate) { // (n, i+1)
    auto [trainData, trainLabels] = _splitLabelsMatrix(trainMatrix, labelColumnIndex); // (n, i), (n, 1)
    if (trainData->rows() != inputNodes()) {
        throw std::invalid_argument("Input matrix must have the same number of nodes as stated with inputNodes at network creation.");
    }

    trainData->transpose(); // (i, n)
    _initialiseMatrices();

    for (int epoch=0; epoch<epochs; epoch++) {
        const Matrix* predictedLabels = _feedForward(trainData); // (o, n)
        predictedLabels->transpose(); // (n, o)
        _backPropagate(trainData, trainLabels, predictedLabels, learningRate);

        if (epoch % 2000 == 0) {
            const double loss = categoricalCrossEntropyLoss(trainLabels, predictedLabels);
            std::cout << "Epoch: " << epoch << ", Loss: " << loss << std::endl;
        }
        delete predictedLabels;
    }
};

double Network::predict(const Matrix* testMatrix, const int labelColumnIndex) {
    auto [testData, testLabels] = _splitLabelsMatrix(testMatrix, labelColumnIndex);

    const Matrix* unprocessedPredictedLabels = _feedForward(testData);
    const Matrix* predictedLabels = unprocessedPredictedLabels->columnwiseArgmax();
    predictedLabels->addScalar(1.0);
    predictedLabels->transpose();

    delete unprocessedPredictedLabels;

    const int numRows = testMatrix->rows();
    double numCorrect = 0.0;

    for (int i=0; i<numRows; i++) {
        if (predictedLabels->get(i, 0) == testLabels->get(i, 0)) {
            numCorrect += 1;
        }
    }

    delete predictedLabels;
    return 1.0 * (numCorrect / numRows);
};

void Network::save(const std::string& modelName) const {
    std::string modelDir;
    if (modelName.empty()) {
        time_t timestamp;
        time(&timestamp);
        const tm* datetime = localtime(&timestamp);
        char datetime_buffer[20];
        std::strftime(datetime_buffer, sizeof(datetime_buffer), "%Y%m%d-%H%M%S", datetime);
        modelDir = datetime_buffer;
        std::cout << "Saving model to directory: " << modelDir << std::endl;
    }
    else {
        modelDir = modelName;
    }

    MatrixFileWriter* file_writer = new MatrixFileWriter();
    file_writer->writeMatrixToFile("models/" + modelDir + "/input_to_hidden_weights.csv", _inputToHiddenWeights);
    file_writer->writeMatrixToFile("models/" + modelDir + "/input_to_hidden_biases.csv", _inputToHiddenBiases);
    file_writer->writeMatrixToFile("models/" + modelDir + "/hidden_to_output_weights.csv", _hiddenToOutputWeights);
    file_writer->writeMatrixToFile("models/" + modelDir + "/hidden_to_output_biases.csv", _hiddenToOutputBiases);
};

void Network::load(const std::string& modelName) {
    MatrixFileReader* file_reader = new MatrixFileReader();
    _inputToHiddenWeights = file_reader->readMatrixFromFile("models/" + modelName + "/input_to_hidden_weights.csv");
    _inputToHiddenBiases = file_reader->readMatrixFromFile("models/" + modelName + "/input_to_hidden_biases.csv");
    _hiddenToOutputWeights = file_reader->readMatrixFromFile("models/" + modelName + "/hidden_to_output_weights.csv");
    _hiddenToOutputBiases = file_reader->readMatrixFromFile("models/" + modelName + "/hidden_to_output_biases.csv");

    _inputNodes = _inputToHiddenWeights->cols();
    _hiddenNodes = _inputToHiddenWeights->rows();
    _outputNodes = _hiddenToOutputWeights->rows();
};
