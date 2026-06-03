#pragma once

#include <exception>
#include <string>

class FileUnprocessableException : public std::exception {
private:
    std::string message;

public:
    FileUnprocessableException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
