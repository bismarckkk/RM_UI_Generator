//
// Created by abc55 on 2025/5/7.
//

#ifndef ENUM_HPP
#define ENUM_HPP

#include <deps/json.hpp>

enum class ObjectType {
    UiLine = 0,
    UiRect,
    UiRound,
    UiEllipse,
    UiArc,
    UiFloat,
    UiNumber,
    UiText,
};

NLOHMANN_JSON_SERIALIZE_ENUM(ObjectType, {
    {ObjectType::UiArc, "arc"},
    {ObjectType::UiRound, "round"},
    {ObjectType::UiRect, "rect"},
    {ObjectType::UiLine, "line"},
    {ObjectType::UiEllipse, "ellipse"},
    {ObjectType::UiText, "text"},
    {ObjectType::UiNumber, "number"},
    {ObjectType::UiFloat, "float"}
})

inline std::string to_string(ObjectType type) {
    switch (type) {
        case ObjectType::UiArc:
            return "arc";
        case ObjectType::UiRound:
            return "round";
        case ObjectType::UiRect:
            return "rect";
        case ObjectType::UiLine:
            return "line";
        case ObjectType::UiEllipse:
            return "ellipse";
        case ObjectType::UiText:
            return "text";
        case ObjectType::UiNumber:
        case ObjectType::UiFloat:
            return "number";
    }
    return "unknown";
}

inline int to_int(ObjectType type) {
    return static_cast<int>(type);
}

inline std::string to_type(ObjectType type) {
    switch (type) {
        case ObjectType::UiArc:
            return "ui_interface_arc_t";
        case ObjectType::UiRound:
            return "ui_interface_round_t";
        case ObjectType::UiRect:
            return "ui_interface_rect_t";
        case ObjectType::UiLine:
            return "ui_interface_line_t";
        case ObjectType::UiEllipse:
            return "ui_interface_ellipse_t";
        case ObjectType::UiText:
            return "ui_interface_string_t";
        case ObjectType::UiNumber:
        case ObjectType::UiFloat:
            return "ui_interface_number_t";
    }
    return "ui_interface_figure_t";
}

enum class ObjectColor {
    main = 0,
    yellow,
    green,
    orange,
    purple,
    pink,
    cyan,
    black,
    white,
};

NLOHMANN_JSON_SERIALIZE_ENUM(ObjectColor, {
    {ObjectColor::main, "main"},
    {ObjectColor::yellow, "yellow"},
    {ObjectColor::green, "green"},
    {ObjectColor::orange, "orange"},
    {ObjectColor::purple, "purple"},
    {ObjectColor::pink, "pink"},
    {ObjectColor::cyan, "cyan"},
    {ObjectColor::black, "black"},
    {ObjectColor::white, "white"}
})

inline std::string to_string(ObjectColor color) {
    switch (color) {
        case ObjectColor::main:
            return "main";
        case ObjectColor::yellow:
            return "yellow";
        case ObjectColor::green:
            return "green";
        case ObjectColor::orange:
            return "orange";
        case ObjectColor::purple:
            return "purple";
        case ObjectColor::pink:
            return "pink";
        case ObjectColor::cyan:
            return "cyan";
        case ObjectColor::black:
            return "black";
        case ObjectColor::white:
            return "white";
    }
    return "unknown";
}

inline int to_int(ObjectColor color) {
    return static_cast<int>(color);
}

#endif //ENUM_HPP
