#pragma once

#include <string>
#include "../matrix/matrix.h"


class DataSet {
    private:
        std::string _filePath;
        int _fileRows;
        int _fileCols;
        Matrix* _data;

        void _getParameters();

        double _convertStrToDouble(std::string strValue);

        void _getData();

        // void _readFile(void (DataSet::*func)(std::fstream));

    public:
        DataSet(std::string filepath);
        
        ~DataSet();

        std::string filePath();

        int fileRows();

        int fileCols();

        Matrix* data();

        void printData();
        
};
