#include <iostream>
#include <mach/boolean.h>

#include "include/nnf/matrix/matrix.hpp"
#include "include/nnf/network/network.hpp"
#include "include/nnf/utils/file_reader.hpp"
#include "include/nnf/utils/file_writer.hpp"


int main(int argc, char *argv[]) {
    MatrixFileReader* inputDataFileReader = new MatrixFileReader();
    const Matrix* trainingData = inputDataFileReader->readMatrixFromFile("example_data/iris_train.csv", TRUE);
    const Matrix* testData = inputDataFileReader->readMatrixFromFile("example_data/iris_test.csv", TRUE);

    ClassificationNetwork* network = new ClassificationNetwork(4, 3);
    network->train(trainingData);
    const double accuracy = network->predict(testData);
    std::cout << "The model accuracy is: " << accuracy << std::endl;

    delete network;
    delete trainingData;
    delete testData;

    return 0;
};
