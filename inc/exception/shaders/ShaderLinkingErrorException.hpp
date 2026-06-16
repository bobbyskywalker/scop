#pragma once

#include <exception>
#include <string>

class ShaderLinkingErrorException : public std::exception {
private:
    std::string message;

public:
    ShaderLinkingErrorException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
