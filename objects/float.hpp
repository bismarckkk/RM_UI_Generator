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
        ss << std::format("    {}width = {}\n", pointer, std::round(fontSize / 10.));
        ss << std::format("    {}font_size = {}\n", pointer, std::round(fontSize));
        ss << std::format("    {}number = {}\n", pointer, std::round(number * 1000.));
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Float, name, group, type, color, layer, x, y, fontSize, number);
}

#endif //FLOAT_HPP
