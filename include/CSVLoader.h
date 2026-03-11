//
// Created by Egor Smirnov on 9.03.2026.
//

#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <vector>
#include "DataFrame.h"

class CSVLoader {
private:
    // splits line by tokens, used as a helper function => static
    static std::vector<std::string> split(const std::string &line);
public:
    CSVLoader() = default;
    std::optional<DataFrame> load(const std::string &path) const;
};

#endif //CSVLOADER_H
