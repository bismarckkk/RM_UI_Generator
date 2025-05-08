#include <iostream>
#include <fstream>

#include <deps/argparse.hpp>
#include <deps/json.hpp>

#include "lib.hpp"
#include "generators/generators.hpp"
#include "utils/utils.hpp"

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("RM UI Generator");
    program.add_argument("-t", "--test").flag();
    program.add_argument("-s", "--static").flag();
    program.add_argument("-i", "--input-file").default_value("");
    program.add_argument("-o", "--output-dir").default_value("generated");
    program.add_argument("frames").remaining();
    program.parse_args(argc, argv);

    if (program.get<bool>("--test")) {
        return 0;
    }
    if (program.get<std::string>("--input-file").empty()) {
        std::cerr << "[UI Gen] [Error] You must specify the input file." << std::endl;
        return 1;
    }

    fs::path input_file = program.get<std::string>("--input-file");
    fs::path output_dir = program.get<std::string>("--output-dir");
    if (!fs::exists(input_file)) {
        std::cerr << "[UI Gen] [Error] Input file " << input_file << " does not exist." << std::endl;
        return 1;
    }
    if (!fs::exists(output_dir)) {
        fs::create_directories(output_dir);
    }
    if (!fs::is_directory(output_dir)) {
        std::cerr << "[UI Gen] [Error] Output directory " << output_dir << " is not a directory." << std::endl;
        return 1;
    }

    std::ifstream ifs(input_file);
    if (!ifs.is_open()) {
        std::cerr << "[UI Gen] [Error] Failed to open input file " << input_file << std::endl;
        return 1;
    }
    nlohmann::json project = nlohmann::json::parse(ifs);
    ifs.close();

    if (project["version"] != 2) {
        std::cerr << "[UI Gen] [Error] Local generator only supports version 2 RMUI file." << std::endl;
        std::cerr << "                 Please resave it with newest RMUI Designer." << std::endl;
        return 1;
    }

    generatorData data;
    auto frames = program.get<std::vector<std::string>>("frames");
    try {
        for (const auto& frame : project["data"].items()) {
            if (std::ranges::find(frames, frame.key()) == frames.end()) {
                continue;
            }
            if (!isValidCIdentifier(frame.key())) {
                std::cerr << "[UI Gen] [Error] Invalid frame name: " << frame.key() << std::endl;
                return 1;
            }
            std::vector<std::shared_ptr<Object>> objs;
            try {
                std::set<std::pair<std::string, std::string>> existingPairs;
                for (const auto& obj : frame.value().items()) {
                    try {
                        auto ptr = from_json(obj.value());
                        if (!isValidCIdentifier(ptr->group)) {
                            std::cerr << "[UI Gen] [Error] Invalid frame group: " << ptr->group << std::endl;
                            return 1;
                        }
                        if (!isValidCIdentifier(ptr->name)) {
                            std::cerr << "[UI Gen] [Error] Invalid object name: " << ptr->name << std::endl;
                            return 1;
                        }
                        auto groupNamePair = std::make_pair(ptr->group, ptr->name);
                        if (existingPairs.contains(groupNamePair)) {
                            std::cerr << "[UI Gen] [Error] Duplicate object found in frame "
                                      << frame.key() << ": group=" << ptr->group
                                      << ", name=" << ptr->name << std::endl;
                            return 1;
                        }
                        existingPairs.insert(groupNamePair);
                        objs.push_back(ptr);
                    } catch (const std::exception& e) {
                        std::cerr << "[UI Gen] [Error] Failed to parse object " << frame.key() << "/" << obj.key() << ": " << e.what() << std::endl;
                        continue;
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "[UI Gen] [Error] Failed to parse frame " << frame.key() << ": " << e.what() << std::endl;
                continue;
            }
            if (frames.size() == 1 && frames[0] == "default") {
                data["g"] = objs;
            } else {
                data[frame.key()] = objs;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[UI Gen] [Error] Failed to parse JSON: " << e.what() << std::endl;
    }

    try {
        if (program.get<bool>("--static")) {
            generateStatic(output_dir, data);
            writeFile(output_dir / "ui_types.h", ui_types_h_static);
            writeFile(output_dir / "ui_interface.h", ui_interface_h_static);
            writeFile(output_dir / "ui_interface.c", ui_interface_c_static);
        } else {
            generateDynamic(output_dir, data);
            writeFile(output_dir / "ui_types.h", ui_types_h_dynamic);
            writeFile(output_dir / "ui_interface.h", ui_interface_h_dynamic);
            writeFile(output_dir / "ui_interface.c", ui_interface_c_dynamic);
        }
    } catch (const std::exception& e) {
        std::cerr << "[UI Gen] [Error] Failed to generate files: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "[UI Gen] [Info] Successfully generated files in " << output_dir << std::endl;

    return 0;
}