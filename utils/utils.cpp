//
// Created by abc55 on 2025/5/8.
//

#include <array>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>

#include "utils.hpp"

namespace fs = std::filesystem;

std::array<std::string, 3> splitUserCodeSections(const std::string& input) {
    std::string before, userCode, after;
    std::istringstream stream(input);
    std::string line;

    bool inUserCode = false;
    while (std::getline(stream, line)) {
        if (line.find("// User Code Begin") != std::string::npos) {
            inUserCode = true;
            userCode += line + "\n";
        } else if (line.find("// User Code End") != std::string::npos) {
            inUserCode = false;
            userCode += line + "\n";
        } else if (inUserCode) {
            userCode += line + "\n";
        } else if (userCode.empty()) {
            before += line + "\n";
        } else {
            after += line + "\n";
        }
    }

    return {before, userCode, after};
}

void writeFile(const fs::path& path, const std::string& content) {
    std::array<std::string, 3> contents = splitUserCodeSections(content);
    if (fs::exists(path)) {
        std::ifstream file(path);
        std::string old((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        auto old_contents = splitUserCodeSections(old);
        if (!old_contents[1].empty()) {
            contents[1] = old_contents[1];
        }
    }
    std::ofstream ofs(path);
    if (!ofs.is_open()) {
        std::cerr << "[UI Gen] [Error] Failed to open file " << path << std::endl;
        return;
    }
    ofs << contents[0] << contents[1] << contents[2] << std::endl;
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
