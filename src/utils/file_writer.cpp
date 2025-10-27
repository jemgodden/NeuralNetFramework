#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "../../include/nnf/utils/file_writer.h"
#include "../../include/nnf/matrix/matrix.h"


MatrixFileWriter::MatrixFileWriter() {
    _filePath = "";
    _fullFilePath = std::filesystem::path();
    _matrix = nullptr;
    _fileRows = 0;
    _fileCols = 0;
};

MatrixFileWriter::~MatrixFileWriter() = default;

std::string MatrixFileWriter::filePath() {
    return _filePath;
};

std::filesystem::path MatrixFileWriter::fullFilePath() {
    return _fullFilePath;
};

const Matrix *MatrixFileWriter::matrix() const {
    return _matrix;
};

int MatrixFileWriter::fileRows() const {
    return _fileRows;
};

int MatrixFileWriter::fileCols() const {
    return _fileCols;
};

void MatrixFileWriter::_createDirectory() const {
    const std::filesystem::path dir = _fullFilePath.parent_path();
    if (!std::filesystem::exists(dir)) {
        try {
            std::filesystem::create_directories(dir);
            std::cout << "Directory created: " << dir << std::endl;
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            throw;
        }
    }
};

void MatrixFileWriter::writeMatrixToFile(const std::string& filePath, const Matrix* matrix) {
    _filePath = filePath;
    _fullFilePath = std::filesystem::current_path().parent_path() / std::filesystem::path(filePath);
    _createDirectory();
    _matrix = matrix;
    _fileRows = matrix->rows();
    _fileCols = matrix->cols();

    std::ofstream outputFile;
    outputFile.open(_fullFilePath);

    outputFile << std::fixed << std::setprecision(6);
    for (int row = 0; row < _fileRows; row++) {
        for (int col = 0; col < _fileCols; col++) {
            outputFile << _matrix->get(row, col);
            if (col < _fileCols - 1) {
                outputFile << ",";
            }
        }
        if (row < _fileRows - 1) {
            outputFile << std::endl;
        }
    }

    outputFile.close();
};
