//
// Created by abc55 on 2025/5/7.
//

#include "arc.hpp"
#include "line.hpp"
#include "rect.hpp"
#include "round.hpp"
#include "ellipse.hpp"
#include "number.hpp"
#include "float.hpp"
#include "text.hpp"

#include "objects.hpp"

nlohmann::json to_data(const std::shared_ptr<Object>& o, const std::string& frame_name, bool is_dynamic) {
    nlohmann::json j;
    j["name"] = o->to_name(frame_name);
    j["type"] = to_type(o->type);
    j["init_code"] = o->to_init_code(frame_name, is_dynamic);
    return j;
}

void to_json(nlohmann::json& j, const std::shared_ptr<Object>& o) {
    switch (o->type) {
        case ObjectType::UiArc:
            ns::to_json(j, *std::dynamic_pointer_cast<Arc>(o));
        break;
        case ObjectType::UiLine:
            ns::to_json(j, *std::dynamic_pointer_cast<Line>(o));
        break;
        case ObjectType::UiRect:
            ns::to_json(j, *std::dynamic_pointer_cast<Rect>(o));
        break;
        case ObjectType::UiRound:
            ns::to_json(j, *std::dynamic_pointer_cast<Round>(o));
        break;
        case ObjectType::UiEllipse:
            ns::to_json(j, *std::dynamic_pointer_cast<Ellipse>(o));
        break;
        case ObjectType::UiNumber:
            ns::to_json(j, *std::dynamic_pointer_cast<Number>(o));
        break;
        case ObjectType::UiFloat:
            ns::to_json(j, *std::dynamic_pointer_cast<Float>(o));
        break;
        case ObjectType::UiText:
            ns::to_json(j, *std::dynamic_pointer_cast<Text>(o));
    }
}

std::shared_ptr<Object> from_json(const nlohmann::json& j) {
    std::shared_ptr<Object> o;
    auto type = j.at("type").get<std::string>();
    if (type == "UiArc") {
        o = std::make_shared<Arc>();
        ns::from_json(j, *std::dynamic_pointer_cast<Arc>(o));
    } else if (type == "UiLine") {
        o = std::make_shared<Line>();
        ns::from_json(j, *std::dynamic_pointer_cast<Line>(o));
    } else if (type == "UiRect") {
        o = std::make_shared<Rect>();
        ns::from_json(j, *std::dynamic_pointer_cast<Rect>(o));
    } else if (type == "UiRound") {
        o = std::make_shared<Round>();
        ns::from_json(j, *std::dynamic_pointer_cast<Round>(o));
    } else if (type == "UiEllipse") {
        o = std::make_shared<Ellipse>();
        ns::from_json(j, *std::dynamic_pointer_cast<Ellipse>(o));
    } else if (type == "UiNumber") {
        o = std::make_shared<Number>();
        ns::from_json(j, *std::dynamic_pointer_cast<Number>(o));
    } else if (type == "UiFloat") {
        o = std::make_shared<Float>();
        ns::from_json(j, *std::dynamic_pointer_cast<Float>(o));
    } else if (type == "UiText") {
        o = std::make_shared<Text>();
        ns::from_json(j, *std::dynamic_pointer_cast<Text>(o));
    }
    return o;
}
