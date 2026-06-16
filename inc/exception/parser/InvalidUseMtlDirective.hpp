#pragma once

#include <exception>
#include <string>

class InvalidUseMtlDirective : public std::exception {
private:
    std::string message;

public:
    InvalidUseMtlDirective(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
