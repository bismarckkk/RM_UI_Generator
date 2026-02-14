//
// Created by abc55 on 2025/5/7.
//

#include <map>
#include <cmath>
#include <fstream>
#include <format>

#include <deps/inja.hpp>

#include "template/static/ui_h.hpp"
#include "template/static/ui_frame_h.hpp"
#include "template/static/ui_frame_c.hpp"
#include "generators.hpp"

struct Objects {
    std::vector<std::shared_ptr<Object>> figure{};
    std::vector<std::shared_ptr<Object>> text{};
};

void generateStatic(const fs::path& path, const generatorData& framesData) {
    std::vector<std::string> frames;
    int frame_id = 0;
    nlohmann::json ui_h_data = {{"frames", nlohmann::json::array()}};
    for (const auto& [frame, objects] : framesData) {
        frames.push_back(frame);
        nlohmann::json frame_data({
            {"name", frame},
            {"groups", nlohmann::json::array()}
        });
        std::map<std::string, Objects> groups;
        for (const auto& obj : objects) {
            auto group = obj->group;
            if (!groups.contains(group)) {
                groups[group] = Objects{};
            }
            if (obj->type == ObjectType::UiText) {
                groups[group].text.push_back(obj);
            } else {
                groups[group].figure.push_back(obj);
            }
        }
        int group_id = 0;
        nlohmann::json data = {
            {"frame", frame},
            {"frame_id", frame_id},
            {"groups", nlohmann::json::array()}
        };
        for (const auto& [group, objects] : groups) {
            frame_data["groups"].emplace_back(group);
            nlohmann::json group_data({
                {"name", group},
                {"id", group_id},
                {"splits", nlohmann::json::array()},
                {"textSplits", nlohmann::json::array()}
            });
            const int figure_split_count = (int)std::ceil(objects.figure.size() / 7.0);
            for (int split_id = 0; split_id < figure_split_count; split_id++) {
                int offset = split_id * 7;
                int num = std::min(7, (int)objects.figure.size() - offset);
                int message_len = 7;
                if (num == 1) {
                    message_len = 1;
                } else if (num <= 2) {
                    message_len = 2;
                } else if (num <= 5) {
                    message_len = 5;
                }
                if (num != message_len) {
                    std::cerr << "[UI Gen] [Warn] Length of Frame " << frame <<" Group " << group << " Split "
                              << split_id <<" is not 1 or 2 or 5 or 7." << std::endl;
                }
                nlohmann::json _data = {
                    {"frame", frame},
                    {"frame_id", frame_id},
                    {"split_id", split_id},
                    {"start_id", offset},
                    {"message_len", message_len},
                    {"objs", nlohmann::json::array()}
                };
                for (int i = 0; i < num; i++) {
                    _data["objs"].push_back(to_data(objects.figure[offset + i], frame));
                }
                group_data["splits"].push_back(std::move(_data));
            }
            for (int i = 0; i < objects.text.size(); i++) {
                int offset = (int)objects.figure.size() + i;
                int split_id = figure_split_count + i;
                nlohmann::json _data = {
                    {"frame", frame},
                    {"frame_id", frame_id},
                    {"split_id", split_id},
                    {"start_id", offset},
                    {"obj", to_data(objects.text[i], frame)}
                };
                group_data["textSplits"].push_back(std::move(_data));
            }
            group_id++;
            data["groups"].push_back(group_data);
        }
        frame_id++;
        std::ofstream ui_frame_h(path / std::format("ui_{}.h", frame));
        std::ofstream ui_frame_c(path / std::format("ui_{}.c", frame));
        inja::render_to(ui_frame_h, ui_frame_h_template_s, data);
        inja::render_to(ui_frame_c, ui_frame_c_template_s, data);
        ui_frame_h.close();
        ui_frame_c.close();
        ui_h_data["frames"].push_back(frame_data);
    }
    std::ofstream ui_h(path / "ui.h");
    inja::render_to(ui_h, ui_h_template_s, ui_h_data);
    ui_h.close();
}

nlohmann::json generateStaticW(const generatorData& framesData) {
    nlohmann::json res;
    nlohmann::json info = nlohmann::json::array();
    std::vector<std::string> frames;
    int frame_id = 0;
    nlohmann::json ui_h_data = {{"frames", nlohmann::json::array()}};
    for (const auto& [frame, objects] : framesData) {
        frames.push_back(frame);
        nlohmann::json frame_data({
            {"name", frame},
            {"groups", nlohmann::json::array()}
        });
        std::map<std::string, Objects> groups;
        for (const auto& obj : objects) {
            auto group = obj->group;
            if (!groups.contains(group)) {
                groups[group] = Objects{};
            }
            if (obj->type == ObjectType::UiText) {
                groups[group].text.push_back(obj);
            } else {
                groups[group].figure.push_back(obj);
            }
        }
        int group_id = 0;
        nlohmann::json data = {
            {"frame", frame},
            {"frame_id", frame_id},
            {"groups", nlohmann::json::array()}
        };
        for (const auto& [group, objects] : groups) {
            frame_data["groups"].emplace_back(group);
            nlohmann::json group_data({
                {"name", group},
                {"id", group_id},
                {"splits", nlohmann::json::array()},
                {"textSplits", nlohmann::json::array()}
            });
            const int figure_split_count = (int)std::ceil(objects.figure.size() / 7.0);
            for (int split_id = 0; split_id < figure_split_count; split_id++) {
                int offset = split_id * 7;
                int num = std::min(7, (int)objects.figure.size() - offset);
                int message_len = 7;
                if (num == 1) {
                    message_len = 1;
                } else if (num <= 2) {
                    message_len = 2;
                } else if (num <= 5) {
                    message_len = 5;
                }
                if (num != message_len) {
                    info.push_back({
                        {"level", "Warn"},
                        {"message", std::format("Length of Frame {} Group {} Split {} is not 1 or 2 or 5 or 7.", frame, group, split_id)}
                    });
                }
                nlohmann::json _data = {
                    {"frame", frame},
                    {"frame_id", frame_id},
                    {"split_id", split_id},
                    {"start_id", offset},
                    {"message_len", message_len},
                    {"objs", nlohmann::json::array()}
                };
                for (int i = 0; i < num; i++) {
                    _data["objs"].push_back(to_data(objects.figure[offset + i], frame));
                }
                group_data["splits"].push_back(std::move(_data));
            }
            for (int i = 0; i < objects.text.size(); i++) {
                int offset = (int)objects.figure.size() + i;
                int split_id = figure_split_count + i;
                nlohmann::json _data = {
                    {"frame", frame},
                    {"frame_id", frame_id},
                    {"split_id", split_id},
                    {"start_id", offset},
                    {"obj", to_data(objects.text[i], frame)}
                };
                group_data["textSplits"].push_back(std::move(_data));
            }
            group_id++;
            data["groups"].push_back(group_data);
        }
        frame_id++;
        res[std::format("ui_{}", frame)] = {
            {"h", inja::render(ui_frame_h_template_s, data)},
            {"c", inja::render(ui_frame_c_template_s, data)}
        };
        ui_h_data["frames"].push_back(frame_data);
    }
    res["ui"] = {
        {"h", inja::render(ui_h_template_s, ui_h_data)}
    };
    return nlohmann::json{{"code", res}, {"info", info}};
}
