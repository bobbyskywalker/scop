#pragma once

#include <exception>
#include <string>

class UnknownKeyInObjectFileException : public std::exception {
private:
    std::string message;

public:
    UnknownKeyInObjectFileException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
