//
// Created by abc55 on 2025/5/7.
//

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <format>
#include <sstream>

#include "enum.hpp"

class Object {
public:
    virtual ~Object() = default;

    std::string name;
    std::string group;
    ObjectType type{};
    ObjectColor color{};
    int layer{};
    double x{}, y{};

    std::string to_name(const std::string& frame_name) const {
        return std::format("ui_{}_{}_{}", frame_name, group, name);
    }

    virtual std::string to_pointer(const std::string& frame_name, bool is_dynamic = true) const {
        return std::format("{}->", to_name(frame_name));
    }

    virtual std::string to_init_code(const std::string& frame_name, bool is_dynamic = true) const {
        std::string pointer = to_pointer(frame_name, is_dynamic);
        std::stringstream ss;
        ss << std::format("    {}figure_type = {};\n", pointer, to_int(type));
        ss << std::format("    {}operate_type = 1;\n", pointer);
        ss << std::format("    {}layer = {};\n", pointer, layer);
        ss << std::format("    {}color = {};\n", pointer, to_int(color));
        ss << std::format("    {}start_x = {};\n", pointer, std::round(x));
        ss << std::format("    {}start_y = {};\n", pointer, std::round(y));
        return ss.str();
    };
};

#endif //OBJECT_HPP
