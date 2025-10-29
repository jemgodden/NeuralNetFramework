#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "../../include/nnf/utils/file_reader.h"
#include "../../include/nnf/matrix/matrix.h"
#include "../../include/nnf/utils/exception.h"


MatrixFileReader::MatrixFileReader() {
    _filePath = "";
    _fullFilePath = std::filesystem::path();
    _headerRow = false;
    _fileRows = 0;
    _fileCols = 0;
    _matrix = nullptr;
};

MatrixFileReader::~MatrixFileReader() {
    delete _matrix;
};

std::string MatrixFileReader::filePath() const {
    return _filePath;
};

std::filesystem::path MatrixFileReader::fullFilePath() const {
    return _fullFilePath;
};

bool MatrixFileReader::headerRow() const {
    return _headerRow;
};

int MatrixFileReader::fileRows() const {
    return _fileRows;
};

int MatrixFileReader::fileCols() const {
    return _fileCols;
};

Matrix* MatrixFileReader::matrix() const {
    return _matrix;
};

void MatrixFileReader::_getParameters() {
    std::ifstream inputFile;
    inputFile.open(fullFilePath());

    try {
        if (inputFile.is_open()) {
            std::string firstLine;

            if (std::getline(inputFile, firstLine)) {
                _fileCols = std::count(firstLine.begin(), firstLine.end(), ',') + 1;
            }
            else {
                throw EmptyFile("Input file is empty.");
            }

            int rowCount = 0;
            std::string nextLine;

            while (std::getline(inputFile, nextLine)) {
                ++rowCount;
            }

            _fileRows = rowCount - headerRow();
        }
        else {
            throw FileOpenError("Input file was not opened properly.");
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }

    inputFile.close();
};

double MatrixFileReader::convertStrToDouble(const std::string& strValue) {
    double value;
    try {
        value = std::stod(strValue);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
    return value;
};

Matrix* MatrixFileReader::readMatrixFromFile(const std::string &filePath, const bool header, const int rows, const int cols) {
    _filePath = filePath;
    _fullFilePath = std::filesystem::current_path().parent_path() / std::filesystem::path(filePath);
    _headerRow = header;
    if (rows == 0 || cols == 0) {
        _getParameters();
    }
    else {
        _fileRows = rows;
        _fileCols = cols;
    }

    std::ifstream inputFile;
    inputFile.open(fullFilePath());

    std::string firstLine;
    try {
        if (inputFile.is_open()) {
            if (std::getline(inputFile, firstLine)) {}
            else {
                throw EmptyFile("Input file is empty.");
            }
        }
        else {
            throw FileOpenError("Input file was not opened properly.");
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }

    _matrix = new Matrix(rows, cols);

    int rowNum = 0;
    if (!headerRow()) {
        int colNum = 0;
        std::string strValue;
        for (int i=0; i<firstLine.length(); i++) {
            if (firstLine[i] != ',') {
                strValue.append(1, firstLine[i]);
            }
            else {
                _matrix->set(rowNum, colNum, convertStrToDouble(strValue));
                strValue.clear();
                ++colNum;
            }
        }
        strValue.clear();
        rowNum++;
    }

    std::string nextLine;
    while (std::getline(inputFile, nextLine)) {
        int colNum = 0;
        std::string strValue;

        for (int i=0; i<nextLine.length(); i++) {
            if (nextLine[i] != ',') {
                strValue.append(1, nextLine[i]);
            }
            else {
                _matrix->set(rowNum, colNum, convertStrToDouble(strValue));
                strValue.clear();
                ++colNum;
            }
        }
        _matrix->set(rowNum, colNum, convertStrToDouble(strValue));
        strValue.clear();
        ++rowNum;
    }

    inputFile.close();
    return _matrix;
};
