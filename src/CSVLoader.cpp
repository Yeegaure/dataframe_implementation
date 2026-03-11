//
// Created by Egor Smirnov on 9.03.2026.
//

#include "../include/CSVLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>

// function splits a string by "," ~ str.split()
std::vector<std::string> CSVLoader::split(const std::string& line) {
    std::vector<std::string> result;
    std::string cell;
    bool in_quotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        // "," in quotes => keep reading
        if (c == '"') in_quotes = !in_quotes;
        else if (c == ',' && in_quotes == false) {
            result.push_back(cell);
            cell.clear();
        }
        else cell.push_back(c);
    }
    result.push_back(cell);
    return result;
}

std::optional<DataFrame> CSVLoader::load(const std::string &path) const {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "can't open .csv file by the path: " << path << "\n";
        return std::nullopt;
    }

    std::string line;

    if (!getline(file, line)) {
        std::cerr << "empty .csv file by the path: " << path << "\n";
        return std::nullopt;
    }

    std::vector<std::string> columns = split(line);
    DataFrame df(columns);

    while (std::getline(file, line)) {
        // remove \r ~ instead of \n
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::vector<std::string> row = split(line);
        try {
            df.add_row(row);
        } catch (const std::exception &e) {
            std::cerr << "skipped bad line: " << e.what() << "\n";
        }
    }
    return df;
}
