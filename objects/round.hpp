//
// Created by abc55 on 2025/5/7.
//

#ifndef ROUND_HPP
#define ROUND_HPP

#include "object.hpp"

class Round final : public Object {
public:
    double lineWidth{};
    double r{};

    Round() {
        type = ObjectType::UiRound;
    }

    std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const override {
        auto pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << Object::to_init_code(frame_name, is_dynamic);
        ss << std::format("    {}width = {};\n", pointer, std::round(lineWidth));
        ss << std::format("    {}r = {};\n", pointer, std::round(r));
        return ss.str();
    }
};

namespace ns {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Round, name, group, type, color, layer, x, y, lineWidth, r);
}

#endif //ROUND_HPP
