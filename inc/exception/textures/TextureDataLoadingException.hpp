#pragma once

#include <exception>
#include <string>

class TextureDataLoadingException : public std::exception {
private:
    std::string message;

public:
    TextureDataLoadingException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
