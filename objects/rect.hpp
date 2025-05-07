//
// Created by abc55 on 2025/5/7.
//

#ifndef RECT_HPP
#define RECT_HPP

#include "object.hpp"

class Rect final : public Object {
public:
    double lineWidth{};
    double width{}, height{};

    Rect() {
        type = ObjectType::UiRect;
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {}\n", pointer, std::round(lineWidth));
        ss << std::format("    {}end_x = {}\n", pointer, x + width);
        ss << std::format("    {}end_y = {}\n", pointer, y + height);
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Rect, name, group, type, color, layer, x, y, lineWidth, width, height);
}

#endif //RECT_HPP
