#pragma once

#include <exception>
#include <string>

class InvalidNormalParamsException : public std::exception {
private:
    std::string message;

public:
    InvalidNormalParamsException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
