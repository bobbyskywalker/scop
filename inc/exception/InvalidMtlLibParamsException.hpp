#pragma once

#include <exception>
#include <string>

class InvalidMtlLibParamsException : public std::exception {
private:
    std::string message;

public:
    InvalidMtlLibParamsException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
