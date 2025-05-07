//
// Created by abc55 on 2025/5/7.
//

#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP

#include "object.hpp"

class Ellipse final : public Object {
public:
    double lineWidth{};
    double rx{}, ry{};

    Ellipse() {
        type = ObjectType::UiEllipse;
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {};\n", pointer, std::round(lineWidth));
        ss << std::format("    {}rx = {};\n", pointer, std::round(rx));
        ss << std::format("    {}ry = {};\n", pointer, std::round(ry));
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Ellipse, name, group, type, color, layer, x, y, lineWidth, rx, ry);
}

#endif //ELLIPSE_HPP
