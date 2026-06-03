#pragma once

#include <exception>
#include <string>

class MalformedMaterialFileDeclarationException : public std::exception {
private:
    std::string message;

public:
    MalformedMaterialFileDeclarationException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
