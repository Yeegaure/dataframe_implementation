//
// Created by Egor Smirnov on 10.03.2026.
//

#include "IMG_Downloader.h"
#include <filesystem>

IMG_Downloader::IMG_Downloader(const std::string &curr_img_path)
    : Downloader("images"), curr_img_path_(curr_img_path) {}

void IMG_Downloader::save(const std::string &name) const {
    const std::string full_path = output_path_ + name + ".png";
    std::filesystem::copy(curr_img_path_, full_path, std::filesystem::copy_options::overwrite_existing);
}
