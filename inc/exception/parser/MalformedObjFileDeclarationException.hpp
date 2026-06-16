#pragma once

#include <exception>
#include <string>

class MalformedObjFileDeclarationException : public std::exception {
private:
    std::string message;

public:
    MalformedObjFileDeclarationException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
