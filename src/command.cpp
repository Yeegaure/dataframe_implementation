//
// Created by Egor Smirnov on 13.03.2026.
//

#include "../include/command.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// command
void LoadCommand::execute(DataFrame &df, const CommandArgs &args) {
    auto& a = std::get<LoadArgs>(args);
    CSVLoader loader;
    auto loaded = loader.load(a.path_);
    if (!loaded) {
        std::cout << "failed to load the file\n";
        return;
    }
    df = *loaded;
    std::cout << "loaded " << a.path_ << "\n";
}

void HeadCommand::execute(DataFrame &df, const CommandArgs &args) {
    auto& a = std::get<HeadArgs>(args);
    df.print(a.n_);
}

void SliceCommand::execute(DataFrame &df, const CommandArgs &args) {
    auto& a = std::get<SliceArgs>(args);
    auto col = df.loc(a.rows_, a.col_);
    for (auto& c : col) std::cout << c << "\n";
}

void PlotCommand::execute(DataFrame &df, const CommandArgs &args) {
    auto& a = std::get<PlotArgs>(args);

    auto x = DataFrame::to_double_vector(df.loc(Slice(0, 50), a.x));
    auto y = DataFrame::to_double_vector(df.loc(Slice(0, 50), a.y));

    plt::plot(x, y);
    plt::savefig("plot.png");
    std::cout << "plot saved\n";
}

void HelpCommand::execute(DataFrame& df, const CommandArgs& args) {
    std::cout << "Available commands:\n";
    std::cout << "  load <path> - load a CSV file\n";
    std::cout << "  head <n> - print first n rows\n";
    std::cout << "  slice <a> <b> <col> - print column <col> for rows [a, b)\n";
    std::cout << "  plot <x> <y> - plot x vs y (first 50 rows)\n";
    std::cout << "  help - show this help message\n";
    std::cout << "  exit - quit the program\n";
}

//// registry
CommandRegistry::CommandRegistry() {
    commands_["load"]  = std::make_shared<LoadCommand>();
    commands_["head"]  = std::make_shared<HeadCommand>();
    commands_["slice"] = std::make_shared<SliceCommand>();
    commands_["plot"]  = std::make_shared<PlotCommand>();
    commands_["help"]  = std::make_shared<HelpCommand>();
}

std::shared_ptr<InterfaceCommand> CommandRegistry::get(const std::string &name) {
    if (commands_.count(name)) return commands_[name];
    return nullptr;
}

std::pair<std::string, CommandArgs> CommandParser::parse(const std::string &input) {
    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd;

    if (cmd == "load") {
        std::string path;
        iss >> path;
        return {cmd, LoadArgs{path}};
    }

    else if (cmd == "head") {
        size_t n;
        iss >> n;
        return {cmd, HeadArgs{n}};
    }
    else if (cmd == "slice") {
        size_t a, b;
        std::string col;
        iss >> a >> b >> col;
        return {cmd, SliceArgs{Slice(a, b), col}};
    }
    else if (cmd == "plot") {
        std::string x, y;
        iss >> x >> y;
        return {cmd, PlotArgs{x, y}};
    }
    else if (cmd == "help") {
        return {cmd, LoadArgs{""}};
    }


    return {"", LoadArgs{""}};
}
