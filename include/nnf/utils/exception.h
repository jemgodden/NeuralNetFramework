#pragma once


class IllegalMatrixOperation : public std::exception {
    private:
        std::string _message;

    public:
        IllegalMatrixOperation(const char* msg);
        const char* what() const noexcept override;
};


class FileOpenError : public std::exception {
    private:
        std::string _message;

    public:
        FileOpenError(const char* msg);
        const char* what() const noexcept override;
};


class EmptyFile : public std::exception {
    private:
        std::string _message;

    public:
        EmptyFile(const char* msg);
        const char* what() const noexcept override;
};
