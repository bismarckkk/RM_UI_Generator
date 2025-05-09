//
// Created by abc55 on 2025/5/7.
//

#include <fstream>
#include <format>

#include <deps/inja.hpp>

#include "template/dynamic/ui_h.hpp"
#include "template/dynamic/ui_frame_h.hpp"
#include "template/dynamic/ui_frame_c.hpp"
#include "generators.hpp"

void generateDynamic(const fs::path& path, const generatorData& framesData) {
    std::vector<std::string> frames;
    for (const auto& [frame, objects] : framesData) {
        frames.push_back(frame);
        nlohmann::json data;
        data["frame"] = frame;
        std::vector<nlohmann::json> objs;
        std::vector<nlohmann::json> textObjs;
        for (const auto& obj : objects) {
            if (obj->type == ObjectType::UiText) {
                textObjs.push_back(to_data(obj, frame));
            } else {
                objs.push_back(to_data(obj, frame));
            }
        }
        data["objs"] = objs;
        data["textObjs"] = textObjs;
        std::ofstream ui_frame_h(path / std::format("ui_{}.h", frame));
        std::ofstream ui_frame_c(path / std::format("ui_{}.c", frame));
        inja::render_to(ui_frame_h, ui_frame_h_template, data);
        inja::render_to(ui_frame_c, ui_frame_c_template, data);
        ui_frame_h.close();
        ui_frame_c.close();
    }
    nlohmann::json data;
    data["frames"] = frames;
    std::ofstream ui_h(path / "ui.h");
    inja::render_to(ui_h, ui_h_template, data);
    ui_h.close();
}

nlohmann::json generateDynamicW(const generatorData& framesData) {
    std::vector<std::string> frames;
    nlohmann::json res;
    for (const auto& [frame, objects] : framesData) {
        frames.push_back(frame);
        nlohmann::json data;
        data["frame"] = frame;
        std::vector<nlohmann::json> objs;
        std::vector<nlohmann::json> textObjs;
        for (const auto& obj : objects) {
            if (obj->type == ObjectType::UiText) {
                textObjs.push_back(to_data(obj, frame));
            } else {
                objs.push_back(to_data(obj, frame));
            }
        }
        data["objs"] = objs;
        data["textObjs"] = textObjs;
        auto hs = inja::render(ui_frame_h_template, data);
        auto cs = inja::render(ui_frame_c_template, data);
        res[std::format("ui_{}", frame)] = {
            {"h", hs},
            {"c", cs}
        };
    }
    const nlohmann::json data({{"frames", frames}});
    res["ui"] = {
        {"h", inja::render(ui_h_template, data)}
    };
    return nlohmann::json{{"code", res}, {"info", nlohmann::json::array()}};
}
