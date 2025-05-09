//
// Created by abc55 on 2025/5/7.
//

#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <filesystem>
#include <vector>
#include <map>

#include <deps/json.hpp>
#include "objects/objects.hpp"

namespace fs = std::filesystem;

using generatorData = std::map<std::string, std::vector<std::shared_ptr<Object>>>;

void generateDynamic(const fs::path& path, const generatorData& framesData);
void generateStatic(const fs::path& path, const generatorData& framesData);

nlohmann::json generateDynamicW(const generatorData& framesData);
nlohmann::json generateStaticW(const generatorData& framesData);

#endif //GENERATORS_HPP
