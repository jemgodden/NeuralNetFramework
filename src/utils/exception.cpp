#include <exception>
#include <string>

#include "../../include/nnf/utils/exception.h"


IllegalMatrixOperation::IllegalMatrixOperation(const char* msg) {
    _message = "Illegal Matrix Operation: ";
    _message.append(msg);
};

const char* IllegalMatrixOperation::what() const noexcept {
    return _message.c_str();
};


FileOpenError::FileOpenError(const char* msg) {
    _message = "File Open Error: ";
    _message.append(msg);
};

const char* FileOpenError::what() const noexcept {
    return _message.c_str();
};


EmptyFile::EmptyFile(const char* msg) {
    _message = "Empty File: ";
    _message.append(msg);
};

const char* EmptyFile::what() const noexcept {
    return _message.c_str();
};
