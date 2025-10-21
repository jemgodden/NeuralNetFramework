#include <iostream>
#include <fstream>
#include <string>

#include "../../include/nnffs/utils/dataset.h"
#include "../../include/nnffs/utils/exception.h"
#include "../../include/nnffs/matrix/matrix.h"


DataSet::DataSet(std::string filePath) {
    _filePath = filePath;
    _fileRows = 0;
    _fileCols = 0;
    
    _getParameters(); 
    _data = new Matrix(_fileRows, _fileCols);
    _getData(); 
};

DataSet::~DataSet() {
    delete _data;
};

std::string DataSet::filePath() {
    return _filePath;
};

int DataSet::fileRows() {
    return _fileRows;
};

int DataSet::fileCols() {
    return _fileCols;
};

Matrix* DataSet::data() {
    return _data;
};

void DataSet::_getParameters() {
    std::fstream inputFile;
    inputFile.open(this->_filePath);
    try {
        if (inputFile.is_open()) {
            std::string firstLine;

            if (std::getline(inputFile, firstLine)) {
                this->_fileCols = std::count(firstLine.begin(), firstLine.end(), ',') + 1;
            }
            else {
                throw EmptyFile("Input file is empty.");
            }

            int rowCount = 0;
            std::string nextLine;

            while (std::getline(inputFile, nextLine)) {
                ++rowCount;
            }

            this->_fileRows = rowCount;
        }
        else {
            throw FileOpenError("Input file was not opened properly.");
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }

    inputFile.close();
};

double DataSet::_convertStrToDouble(std::string strValue) {
    double value;
    try {
        value = std::stod(strValue);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }
    return value;
}

void DataSet::_getData() {
    std::fstream inputFile;
    inputFile.open(_filePath);

    try {
        if (inputFile.is_open()) {
            std::string firstLine;
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
    }
    
    std::string nextLine;
    int rowNum = 0;

    while (std::getline(inputFile, nextLine)) {
        int colNum = 0;
        std::string strValue;
        
        for (int i=0; i<nextLine.length(); i++) {
            if (nextLine[i] != ',') {
                strValue.append(1, nextLine[i]);
            }
            else {
                _data->set(rowNum, colNum, _convertStrToDouble(strValue));
                strValue.clear();
                ++colNum;
            }
        }
        _data->set(rowNum, colNum, _convertStrToDouble(strValue));
        strValue.clear();
        ++rowNum;
    }

    inputFile.close();
};

void DataSet::printData() {
    _data->print();
}

// void DataSet::_readFile(void (DataSet::*func)(std::fstream)) {
//     std::fstream* inputFile;
//     inputFile->open(_filePath);

//     try {
//         if (inputFile->is_open()) {
//             (this->*func)(*inputFile);
//         }
//         else {
//             throw "Input file was not opened properly.";
//         }
//     }
//     catch (std::string msg) {
//         std::cout << msg << std::endl; 
//     }

//     inputFile->close();
// };
