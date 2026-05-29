#include "../inc/util.hpp"
#include <sstream>

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> res;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) res.push_back(item);
    }
    return res;
}
