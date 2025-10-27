#pragma once

#include <filesystem>
#include <string>

#include "../matrix/matrix.h"


class MatrixFileReader {
    private:
        std::string _filePath;

        std::filesystem::path _fullFilePath;

        bool _header;

        int _fileRows;

        int _fileCols;

        Matrix* _matrix;

        void _getParameters();

    public:
        MatrixFileReader();

        ~MatrixFileReader();

        std::string filePath();

        std::filesystem::path fullFilePath();

        bool headerRow() const;

        int fileRows() const;

        int fileCols() const;

        Matrix* matrix() const;

        static double convertStrToDouble(const std::string &strValue);

        Matrix* readMatrixFromFile(const std::string &filePath, bool header=false, int rows=0, int cols=0);
};
