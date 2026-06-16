#pragma once

#include <exception>
#include <string>

class InvalidVertexParamsException : public std::exception {
private:
    std::string message;

public:
    InvalidVertexParamsException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
