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

        void _getData() const;

    public:
        explicit DataSet(const std::string &filePath);
        
        ~DataSet();

        std::string filePath();

        int fileRows() const;

        int fileCols() const;

        Matrix* data() const;

        static int convertStrToInt(const std::string& strValue);

        static double convertStrToDouble(const std::string& strValue);

        void printData() const;
        
};
