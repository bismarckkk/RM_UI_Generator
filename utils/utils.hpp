//
// Created by abc55 on 2025/5/8.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <filesystem>
#include <string>

void writeFile(const std::filesystem::path& path, const std::string& content);
bool isValidCIdentifier(const std::string& identifier);

#endif //UTILS_HPP
