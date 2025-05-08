//
// Created by abc55 on 2025/5/8.
//

#include <cctype>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include "utils.hpp"

namespace fs = std::filesystem;

void writeFile(const fs::path& path, const std::string& content) {
    std::ofstream ofs(path);
    if (!ofs.is_open()) {
        std::cerr << "[UI Gen] [Error] Failed to open file " << path << std::endl;
        return;
    }
    ofs << content;
    ofs.close();
}

bool isValidCIdentifier(const std::string& identifier) {
    if (identifier.empty()) {
        return false;
    }
    for (size_t i = 1; i < identifier.size(); ++i) {
        if (!std::isalnum(identifier[i]) && identifier[i] != '_') {
            return false;
        }
    }

    return true;
}
