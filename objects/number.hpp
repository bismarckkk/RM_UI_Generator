//
// Created by abc55 on 2025/5/7.
//

#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "object.hpp"

class Number : public Object {
public:
    double fontSize{};
    double number{};

    Number() {
        type = ObjectType::UiNumber;
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {}\n", pointer, std::round(fontSize / 10.));
        ss << std::format("    {}font_size = {}\n", pointer, std::round(fontSize));
        ss << std::format("    {}number = {}\n", pointer, std::round(number));
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Number, name, group, type, color, layer, x, y, fontSize, number);
}

#endif //NUMBER_HPP
