#include <iostream>
#include <fstream>
#include <format>
#include <functional>
#include <set>

#include <deps/json.hpp>
#include <emscripten/bind.h>

#include "generators/generators.hpp"
#include "utils/utils.hpp"

std::string generate(const std::string& _project, bool static_mode) {
    auto project = nlohmann::json::parse(_project);
    nlohmann::json info = nlohmann::json::array();

    generatorData data;
    try {
        for (const auto& frame : project.items()) {
            if (!isValidCIdentifier(frame.key())) {
                info.push_back({
                    {"type", "error"},
                    {"message", std::format("Invalid frame name: {}", frame.key())}
                });
                return nlohmann::json({{"code", {}}, {"info", info}}).dump();
            }
            std::vector<std::shared_ptr<Object>> objs;
            try {
                std::set<std::pair<std::string, std::string>> existingPairs;
                for (const auto& obj : frame.value().items()) {
                    try {
                        auto ptr = from_json(obj.value());
                        if (!isValidCIdentifier(ptr->group)) {
                            info.push_back({
                                {"type", "error"},
                                {"message", std::format("Invalid frame group: {}", ptr->group)}
                            });
                            return nlohmann::json({{"code", {}}, {"info", info}}).dump();
                        }
                        if (!isValidCIdentifier(ptr->name)) {
                            info.push_back({
                                {"type", "error"},
                                {"message", std::format("Invalid object name: {}", ptr->name)}
                            });
                            return nlohmann::json({{"code", {}}, {"info", info}}).dump();
                        }
                        auto groupNamePair = std::make_pair(ptr->group, ptr->name);
                        if (existingPairs.contains(groupNamePair)) {
                            info.push_back({
                                {"type", "error"},
                                {"message", std::format("Duplicate object found in frame {}: group={}, name={}", frame.key(), ptr->group, ptr->name)}
                            });
                            return nlohmann::json({{"code", {}}, {"info", info}}).dump();
                        }
                        existingPairs.insert(groupNamePair);
                        objs.push_back(ptr);
                    } catch (const std::exception& e) {
                        info.push_back({
                            {"type", "error"},
                            {"message", std::format("Failed to parse object {}/{}: {}", frame.key(), obj.key(), e.what())}
                        });
                        return nlohmann::json({{"code", {}}, {"info", info}}).dump();
                    }
                }
            } catch (const std::exception& e) {
                info.push_back({
                    {"type", "error"},
                    {"message", std::format("Failed to parse frame {}: {}", frame.key(), e.what())}
                });
                return nlohmann::json({{"code", {}}, {"info", info}}).dump();
            }
            if (project.size() == 1 && frame.key() == "default") {
                data["g"] = objs;
            } else {
                data[frame.key()] = objs;
            }
        }
    } catch (const std::exception& e) {
        info.push_back({
            {"type", "error"},
            {"message", std::format("Failed to parse JSON: {}", e.what())}
        });
        return nlohmann::json({{"code", {}}, {"info", info}}).dump();
    }

    nlohmann::json res({{"code", {}}, {"info", info}});
    try {
        if (static_mode) {
            res = generateStaticW(data);
        } else {
            res = generateDynamicW(data);
        }
        res["info"].insert(res["info"].begin(), info.begin(), info.end());
        return res.dump();
    } catch (const std::exception& e) {
        res["info"].push_back({
            {"type", "error"},
            {"message", std::format("Failed to generate files: {}", e.what())}
        });
        return res.dump();
    }
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(generator) {
    function("generate", &generate);
}
