#pragma once

#include <exception>
#include <string>

class MissingShaderFileException : public std::exception {
private:
    std::string message;

public:
    MissingShaderFileException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
