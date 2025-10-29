#pragma once

#include <filesystem>
#include <string>

#include "../matrix/matrix.h"


class MatrixFileWriter {
    private:
        std::string _filePath;

        std::filesystem::path _fullFilePath;

        const Matrix* _matrix;

        int _fileRows{};

        int _fileCols{};

        void _createDirectory() const;

    public:
        MatrixFileWriter();

        ~MatrixFileWriter();

        std::string filePath() const;

        std::filesystem::path fullFilePath() const;

        const Matrix *matrix() const;

        int fileRows() const;

        int fileCols() const;

        void writeMatrixToFile(const std::string& filePath, const Matrix* matrix);
};
