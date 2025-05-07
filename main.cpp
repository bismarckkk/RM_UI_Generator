#include <iostream>
#include <fstream>

#include <deps/argparse.hpp>
#include <deps/json.hpp>

#include "generators/generators.hpp"

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
        std::cerr << "You must specify the input file." << std::endl;
        return 1;
    }

    fs::path input_file = program.get<std::string>("--input-file");
    fs::path output_dir = program.get<std::string>("--output-dir");
    if (!fs::exists(input_file)) {
        std::cerr << "Input file " << input_file << " does not exist." << std::endl;
        return 1;
    }
    if (!fs::exists(output_dir)) {
        fs::create_directories(output_dir);
    }
    if (!fs::is_directory(output_dir)) {
        std::cerr << "Output directory " << output_dir << " is not a directory." << std::endl;
        return 1;
    }

    std::ifstream ifs(input_file);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open input file " << input_file << std::endl;
        return 1;
    }
    nlohmann::json project = nlohmann::json::parse(ifs);
    ifs.close();

    if (project["version"] != 2) {
        std::cerr << "Local generator only supports version 2 RMUI file." << std::endl;
        std::cerr << "Please resave it with newest RMUI Designer." << std::endl;
        return 1;
    }

    generatorData data;
    auto frames = program.get<std::vector<std::string>>("frames");
    for (const auto& frame : project["data"].items()) {
        if (std::ranges::find(frames, frame.key()) == frames.end()) {
            continue;
        }
        std::vector<std::shared_ptr<Object>> objs;
        for (const auto& obj : frame.value().items()) {
            objs.push_back(from_json(obj.value()));
        }
        data[frame.key()] = objs;
    }

    if (program.get<bool>("--static")) {
        generateStatic(output_dir, data);
    } else {
        generateDynamic(output_dir, data);
    }

    return 0;
}