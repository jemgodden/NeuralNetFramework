#include <iostream>

#include "../../include/nnf/matrix/matrix.hpp"
#include "../../include/nnf/network/network.hpp"
#include "../../include/nnf/network/activations.hpp"
#include "../../include/nnf/network/costs.hpp"
#include "costs.cpp"
#include "../../include/nnf/utils/file_writer.hpp"
#include "../../include/nnf/utils/file_reader.hpp"


Network::Network(const int inputNodes, const int hiddenNodes) {
    _inputNodes = inputNodes;
    _hiddenNodes = hiddenNodes;

    _inputToHiddenWeights = nullptr;
    _inputToHiddenBiases = nullptr;
    _hiddenToOutputWeights = nullptr;
    _hiddenToOutputBiases = nullptr;

    _hiddenLayerOutput = nullptr;
};

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

int Network::hiddenNodes() const {
    return _hiddenNodes;
};

void Network::_setInputToHiddenWeights(Matrix* newMatrix) {
    free(_inputToHiddenWeights);
    _inputToHiddenWeights = newMatrix;
};

void Network::_setInputToHiddenBiases(Matrix* newMatrix) {
    free(_inputToHiddenBiases);
    _inputToHiddenBiases = newMatrix;
};

void Network::_setHiddenToOutputWeights(Matrix* newMatrix) {
    free(_hiddenToOutputWeights);
    _hiddenToOutputWeights = newMatrix;
};

void Network::_setHiddenToOutputBiases(Matrix* newMatrix) {
    free(_hiddenToOutputBiases);
    _hiddenToOutputBiases = newMatrix;
};

void Network::_initialiseMatrices() {
    _inputToHiddenWeights = new Matrix(_hiddenNodes, _inputNodes); // (h, i)
    _inputToHiddenWeights->randomise();
    _inputToHiddenBiases = new Matrix(_hiddenNodes, 1); // (h, 1)
    _inputToHiddenBiases->randomise();
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

    MatrixFileWriter* fileWriter = new MatrixFileWriter();
    fileWriter->writeMatrixToFile("models/" + modelDir + "/input_to_hidden_weights.csv", _inputToHiddenWeights);
    fileWriter->writeMatrixToFile("models/" + modelDir + "/input_to_hidden_biases.csv", _inputToHiddenBiases);
    fileWriter->writeMatrixToFile("models/" + modelDir + "/hidden_to_output_weights.csv", _hiddenToOutputWeights);
    fileWriter->writeMatrixToFile("models/" + modelDir + "/hidden_to_output_biases.csv", _hiddenToOutputBiases);
};

void Network::load(const std::string& modelName) {
    MatrixFileReader* fileReader = new MatrixFileReader();
    _inputToHiddenWeights = fileReader->readMatrixFromFile("models/" + modelName + "/input_to_hidden_weights.csv");
    _inputToHiddenBiases = fileReader->readMatrixFromFile("models/" + modelName + "/input_to_hidden_biases.csv");
    _hiddenToOutputWeights = fileReader->readMatrixFromFile("models/" + modelName + "/hidden_to_output_weights.csv");
    _hiddenToOutputBiases = fileReader->readMatrixFromFile("models/" + modelName + "/hidden_to_output_biases.csv");

    _inputNodes = _inputToHiddenWeights->cols();
    _hiddenNodes = _inputToHiddenWeights->rows();
};



ClassificationNetwork::ClassificationNetwork(const int inputNodes, const int outputNodes, const int hiddenNodes): Network(inputNodes, hiddenNodes) {
    _outputNodes = outputNodes;
};

ClassificationNetwork::~ClassificationNetwork() = default;

int ClassificationNetwork::outputNodes() const {
    return _outputNodes;
};

void ClassificationNetwork::_initialiseMatrices() {
    Network::_initialiseMatrices();
    _hiddenToOutputWeights = new Matrix(_outputNodes, _hiddenNodes); // (o, h)
    _hiddenToOutputWeights->randomise();
    _hiddenToOutputBiases = new Matrix(_outputNodes, 1); // (o, 1)
    _hiddenToOutputBiases->randomise();
};

Matrix* ClassificationNetwork::_feedForward(const Matrix* trainMatrix) { // (i, n)
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

void ClassificationNetwork::_backPropagate(const Matrix* trainMatrix, const Matrix* actualLabelMatrix, const Matrix* predictedLabelMatrix, const double learningRate) { // (i, n), (n, 1), (n, o)
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
    const Matrix* transposedHiddenLayerOutputCopy = hiddenLayerOutputCopy->transpose(); // (n, h)
    const Matrix* hiddenDelta = hiddenError->multiply(transposedHiddenLayerOutputCopy); // (n, h)
    delete hiddenToOutputWeightsCopy;
    delete hiddenError;
    delete hiddenLayerOutputCopy;
    delete transposedHiddenLayerOutputCopy;

    // Update Weights and Biases
    // Updating hidden-output weights.
    const Matrix* hiddenToOutputWeightsDelta = _hiddenLayerOutput->dot(outputDelta); // (h, o)
    hiddenToOutputWeightsDelta->scale(learningRate); // (h, o)
    const Matrix* transposedHiddenToOutputWeightsDelta = hiddenToOutputWeightsDelta->transpose(); // (o, h)
    Matrix* transformedHiddenToOutputWeights = _hiddenToOutputWeights->add(transposedHiddenToOutputWeightsDelta); // (o, h)
    _setHiddenToOutputWeights(transformedHiddenToOutputWeights);
    delete hiddenToOutputWeightsDelta;
    delete transposedHiddenToOutputWeightsDelta;

    // Updating hidden-output biases.
    const Matrix* hiddenToOutputBiasesDelta = outputDelta->columnwiseSum(); // (1, o)
    hiddenToOutputBiasesDelta->scale(learningRate); // (1, o)
    const Matrix* transposedHiddenToOutputBiasesDelta = hiddenToOutputBiasesDelta->transpose(); // (o, 1)
    Matrix* transformedHiddenToOutputBiases = _hiddenToOutputBiases->add(transposedHiddenToOutputBiasesDelta); // (o, 1)
    _setHiddenToOutputBiases(transformedHiddenToOutputBiases);
    delete outputDelta;
    delete hiddenToOutputBiasesDelta;
    delete transposedHiddenToOutputBiasesDelta;

    // Updating input-hidden weights.
    const Matrix* trainMatrixCopy = trainMatrix->deepCopy(); // (i, n)
    const Matrix* inputToHiddenWeightsDelta = trainMatrixCopy->dot(hiddenDelta); // (i, h)
    inputToHiddenWeightsDelta->scale(learningRate); // (i, h)
    const Matrix* transposedInputToHiddenWeightsDelta = inputToHiddenWeightsDelta->transpose(); // (h, i)
    Matrix* transformedInputToHiddenWeights = _inputToHiddenWeights->add(transposedInputToHiddenWeightsDelta); // (h, i)
    _setInputToHiddenWeights(transformedInputToHiddenWeights);
    delete trainMatrixCopy;
    delete inputToHiddenWeightsDelta;
    delete transposedInputToHiddenWeightsDelta;

    // Updating input-hidden biases.
    const Matrix* inputToHiddenBiasesDelta = hiddenDelta->columnwiseSum(); // (1, h)
    inputToHiddenBiasesDelta->scale(learningRate); // (1, h)
    const Matrix* transposedInputToHiddenBiasesDelta = inputToHiddenBiasesDelta->transpose(); // (h, 1)
    Matrix* transformedInputToHiddenBiases = _inputToHiddenBiases->add(transposedInputToHiddenBiasesDelta); // (h, 1)
    _setInputToHiddenBiases(transformedInputToHiddenBiases);
    delete hiddenDelta;
    delete inputToHiddenBiasesDelta;
    delete transposedInputToHiddenBiasesDelta;
};

void ClassificationNetwork::_train(const Matrix* trainMatrix, const int labelColumnIndex, const int epochs, const double learningRate) { // (n, i+1)
    auto [trainLabels, trainData] = _splitLabelsMatrix(trainMatrix, labelColumnIndex); // (n, i), (n, 1)
    if (trainData->cols() != inputNodes()) {
        throw std::invalid_argument("Input matrix must have the same number of columns as stated with inputNodes at network creation.");
    }

    // std::cout << "trainData: (" << trainData->rows() << ", " << trainData->cols() << ")" << std::endl;
    // std::cout << "hello" << std::endl;

    const Matrix* transposedTrainData = trainData->transpose(); // (i, n)
    delete trainData;
    _initialiseMatrices();

    for (int epoch=1; epoch<epochs+1; epoch++) {
        const Matrix* predictedLabels = _feedForward(transposedTrainData); // (o, n)
        const Matrix* transposedPredictedLabels = predictedLabels->transpose(); // (n, o)
        delete predictedLabels;

        _backPropagate(transposedTrainData, trainLabels, transposedPredictedLabels, learningRate);

        if (epoch % 1000 == 0 || epoch == 1) {
            const double loss = categoricalCrossEntropyLoss(trainLabels, transposedPredictedLabels);
            std::cout << "Epoch: " << epoch << ", Loss: " << loss << std::endl;
        }
        delete transposedPredictedLabels;
    }
    delete trainLabels;
    delete transposedTrainData;
};

void ClassificationNetwork::train(const Matrix* trainMatrix, const int labelColumnIndex, const int epochs, const double learningRate) {
    _train(trainMatrix, labelColumnIndex, epochs, learningRate);
};

double ClassificationNetwork::_predict(const Matrix* testMatrix, const int labelColumnIndex) { // (n, i+1)
    auto [testLabels, testData] = _splitLabelsMatrix(testMatrix, labelColumnIndex); // (n, i), (n, 1)
    const Matrix* transposedTestData = testData->transpose(); // (i, n)
    delete testData;

    const Matrix* unprocessedPredictedLabels = _feedForward(transposedTestData); // (o, n)
    const Matrix* predictedLabels = unprocessedPredictedLabels->columnwiseArgmax(); // (1, n)
    predictedLabels->addScalar(1.0); // (1, n)
    const Matrix* transposedPredictedLabels = predictedLabels->transpose(); // (n, 1)
    delete unprocessedPredictedLabels;
    delete predictedLabels;

    const int numRows = testMatrix->rows();
    double numCorrect = 0.0;

    for (int i=0; i<numRows; i++) {
        if (transposedPredictedLabels->get(i, 0) == testLabels->get(i, 0)) {
            numCorrect += 1;
        }
    }

    delete testLabels;
    delete transposedPredictedLabels;
    return 1.0 * (numCorrect / numRows);
};

double ClassificationNetwork::predict(const Matrix* testMatrix, const int labelColumnIndex) {
    return _predict(testMatrix, labelColumnIndex);
};

void ClassificationNetwork::load(const std::string& modelName) {
    Network::load(modelName);
    _outputNodes = _hiddenToOutputWeights->rows();
};