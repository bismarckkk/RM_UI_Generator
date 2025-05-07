//
// Created by abc55 on 2025/5/7.
//

#ifndef TEXT_HPP
#define TEXT_HPP

#include "object.hpp"

class Text : public Object {
public:
    double fontSize{};
    std::string text{};

    Text() {
        type = ObjectType::UiText;
    }

    std::string to_pointer(const std::string& frame_name, bool is_dynamic = true) const override {
        if (is_dynamic) {
            return std::format("{}->", to_name(frame_name));
        } else {
            return std::format("{}_f.option.", to_name(frame_name));
        }
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {};\n", pointer, std::round(fontSize / 10.));
        ss << std::format("    {}font_size = {};\n", pointer, std::round(fontSize));
        ss << std::format("    {}str_length = {};\n", pointer, text.length());
        ss << std::format("    strcpy({}string, \"{}\");\n", pointer, text);
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Text, name, group, type, color, layer, x, y, fontSize, text);
}

#endif //TEXT_HPP
