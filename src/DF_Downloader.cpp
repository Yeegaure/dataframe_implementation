//
// Created by Egor Smirnov on 10.03.2026.
//

#include "DF_Downloader.h"
#include <string>

DF_Downloader::DF_Downloader(const DataFrame &df) : Downloader("dataframes"), df_(df) {}

void DF_Downloader::save(const std::string &name) const {
    const std::string full_path = output_path_ + name + ".csv";
    df_.to_csv(full_path);
}
