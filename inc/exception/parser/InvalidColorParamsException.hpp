#pragma once

#include <exception>
#include <string>

class InvalidColorParamsException : public std::exception {
private:
    std::string message;

public:
    InvalidColorParamsException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
