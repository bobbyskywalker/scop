#pragma once

#include <string>
#include <vector>

# define PARSER_ERROR_LOG "Exception occurred when parsing line "

std::vector<std::string> split(const std::string& s, char delim);
