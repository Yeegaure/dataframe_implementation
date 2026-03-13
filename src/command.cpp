//
// Created by Egor Smirnov on 13.03.2026.
//

#include "command.h"
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

void StatsCommand::execute(DataFrame& df, const CommandArgs& args) {
    auto& a = std::get<StatsArgs>(args);

    auto vec = DataFrame::to_double_vector(df.loc(Slice(0, df.row_number()), a.col));

    if (vec.empty()) {
        std::cout << "Column is empty or non-numeric\n";
        return;
    }

    double sum = 0;
    for (double v : vec) sum += v;
    double mean = sum / vec.size();

    double var = 0;
    for (double v : vec) var += (v - mean) * (v - mean);
    var /= vec.size();

    double mn = *std::min_element(vec.begin(), vec.end());
    double mx = *std::max_element(vec.begin(), vec.end());

    std::cout << "Stats for column: " << a.col << "\n";
    std::cout << "  mean: " << mean << "\n";
    std::cout << "  variance: " << var << "\n";
    std::cout << "  min: " << mn << "\n";
    std::cout << "  max: " << mx << "\n";
}


void HelpCommand::execute(DataFrame& df, const CommandArgs& args) {
    std::cout << "Available commands:\n";
    std::cout << "  load <path> - load a CSV file\n";
    std::cout << "  head <n> - print first n rows\n";
    std::cout << "  slice <a> <b> <col> - print column <col> for rows [a, b)\n";
    std::cout << "  plot <x> <y> - plot x vs y (first 50 rows)\n";
    std::cout << "  stats <col> - show mean, variance, min, max for column <col>\n";
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
    commands_["stats"] = std::make_shared<StatsCommand>();
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
    else if (cmd == "stats") {
        std::string col;
        iss >> col;
        return {cmd, StatsArgs{col}};
    }
    else if (cmd == "help") {
        return {cmd, LoadArgs{""}};
    }

    return {"", LoadArgs{""}};
}

CommandHandler::CommandHandler(std::unique_ptr<DataFrame>& df, CommandRegistry& registry)
    : df_(df), registry_(registry) {}

void CommandHandler::dispatch(const std::string& cmdName, const CommandArgs& args) {
    auto cmd = registry_.get(cmdName);
    if (!cmd) {
        std::cout << "Unknown command\n";
        return;
    }
    cmd->execute(*df_, args);
}
