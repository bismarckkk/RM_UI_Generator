//
// Created by abc55 on 2025/5/7.
//

#ifndef LINE_HPP
#define LINE_HPP

#include "object.hpp"

class Line final : public Object {
public:
    double lineWidth{};
    double x2{}, y2{};

    Line() {
        type = ObjectType::UiLine;
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {}\n", pointer, std::round(lineWidth));
        ss << std::format("    {}end_x = {}\n", pointer, x2);
        ss << std::format("    {}end_y = {}\n", pointer, y2);
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Line, name, group, type, color, layer, x, y, lineWidth, x2, y2);
}

#endif //LINE_HPP
