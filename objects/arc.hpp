//
// Created by abc55 on 2025/5/7.
//

#ifndef ARC_HPP
#define ARC_HPP

#include "object.hpp"

class Arc final : public Object {
public:
    double lineWidth{};
    double startAngle{}, endAngle{};
    double rx{}, ry{};

    Arc() {
        type = ObjectType::UiArc;
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {}\n", pointer, std::round(lineWidth));
        ss << std::format("    {}start_angle = {}\n", pointer, startAngle);
        ss << std::format("    {}end_angle = {}\n", pointer, endAngle);
        ss << std::format("    {}rx = {}\n", pointer, rx);
        ss << std::format("    {}ry = {}\n", pointer, ry);
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Arc, name, group, type, color, layer, x, y, lineWidth, startAngle, endAngle, rx, ry);
}

#endif //ARC_HPP
