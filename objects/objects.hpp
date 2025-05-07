//
// Created by abc55 on 2025/5/7.
//

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <memory>

#include "object.hpp"

nlohmann::json to_data(const std::shared_ptr<Object>& o, const std::string& frame_name, bool is_dynamic = true);
void to_json(nlohmann::json& j, const std::shared_ptr<Object>& o);
std::shared_ptr<Object> from_json(const nlohmann::json& j);

#endif //OBJECTS_HPP
