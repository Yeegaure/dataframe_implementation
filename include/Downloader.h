//
// Created by Egor Smirnov on 10.03.2026.
//

#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include <string>
#include <filesystem>
#include <stdexcept>

class Downloader {
protected:
    std::string output_path_; // output/images or output/dataframes
    static void ensure_directory_structure_(const std::filesystem::path& root) {
        const auto output = root / "output", dir_df = output / "dataframes", dir_img = output / "images";
        try {
            if (!std::filesystem::exists(output)) std::filesystem::create_directory(output);
            if (!std::filesystem::exists(dir_df)) std::filesystem::create_directory(dir_df);
            if (!std::filesystem::exists(dir_img)) std::filesystem::create_directory(dir_img);
        } catch (...) {
            throw std::runtime_error(std::string("failed to create output directory structure"));
        }
    }

public:
    explicit Downloader(const std::string &sub_folder_path) {
        std::filesystem::path root = std::filesystem::current_path().parent_path();
        ensure_directory_structure_(root);
        std::string root_str = root.string();
        output_path_ = root_str + "/output/" + sub_folder_path + "/";
        try {
            std::filesystem::create_directories(output_path_);
        } catch (const std::exception &e) {
            throw std::runtime_error(std::string("failed to create output directory ") + e.what());
        }
    }
    virtual void save(const std::string &name) const = 0;
    virtual ~Downloader() = default;
};
#endif //DOWNLOADER_H
