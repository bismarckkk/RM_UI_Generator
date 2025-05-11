//
// Created by abc55 on 2025/5/7.
//

#ifndef FLOAT_HPP
#define FLOAT_HPP

#include "number.hpp"

class Float : public Number {
public:
    Float() {
        type = ObjectType::UiFloat;
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {};\n", pointer, std::round(fontSize / 10.));
        ss << std::format("    {}font_size = {};\n", pointer, std::round(fontSize));
        ss << std::format("    {}number = {};\n", pointer, std::round(number * 1000.));
        return ss.str();
    }
};

namespace ns {

template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int>  = 0>
void to_json(BasicJsonType &nlohmann_json_j, const Float &nlohmann_json_t) {
    nlohmann_json_j["name"] = nlohmann_json_t.name;
    nlohmann_json_j["group"] = nlohmann_json_t.group;
    nlohmann_json_j["type"] = nlohmann_json_t.type;
    nlohmann_json_j["color"] = nlohmann_json_t.color;
    nlohmann_json_j["layer"] = nlohmann_json_t.layer;
    nlohmann_json_j["x"] = nlohmann_json_t.x;
    nlohmann_json_j["y"] = nlohmann_json_t.y;
    nlohmann_json_j["fontSize"] = nlohmann_json_t.fontSize;
    nlohmann_json_j["float"] = nlohmann_json_t.number;
}

template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int>  = 0>
void from_json(const BasicJsonType &nlohmann_json_j, Float &nlohmann_json_t) {
    nlohmann_json_j.at("name").get_to(nlohmann_json_t.name);
    nlohmann_json_j.at("group").get_to(nlohmann_json_t.group);
    nlohmann_json_j.at("type").get_to(nlohmann_json_t.type);
    nlohmann_json_j.at("color").get_to(nlohmann_json_t.color);
    nlohmann_json_j.at("layer").get_to(nlohmann_json_t.layer);
    nlohmann_json_j.at("x").get_to(nlohmann_json_t.x);
    nlohmann_json_j.at("y").get_to(nlohmann_json_t.y);
    nlohmann_json_j.at("fontSize").get_to(nlohmann_json_t.fontSize);
    nlohmann_json_j.at("float").get_to(nlohmann_json_t.number);
}

}

#endif //FLOAT_HPP
