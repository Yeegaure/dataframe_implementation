//
// Created by Egor Smirnov on 13.03.2026.
//

#ifndef COMMAND_H
#define COMMAND_H
#include <memory>
#include <string>
#include <variant>
#include <unordered_map>

#include <DataFrame.h>
#include <CSVLoader.h>

struct LoadArgs { std::string path_; };
struct HeadArgs { size_t n_; };
struct SliceArgs { Slice rows_; std::string col_;};
struct PlotArgs { std::string x, y; };
struct StatsArgs { std::string col; };

using CommandArgs = std::variant<LoadArgs, HeadArgs, SliceArgs, PlotArgs, StatsArgs>;

// base command class
class InterfaceCommand {
public:
    virtual ~InterfaceCommand() = default;
    virtual void execute(DataFrame &df, const CommandArgs &args) = 0;
};
// other classes
class LoadCommand : public InterfaceCommand {
public: void execute(DataFrame &df, const CommandArgs &args) override;
};

class HeadCommand : public InterfaceCommand {
public: void execute(DataFrame &df, const CommandArgs &args) override;
};

class SliceCommand : public InterfaceCommand {
public: void execute(DataFrame &df, const CommandArgs &args) override;
};

class PlotCommand : public InterfaceCommand {
public: void execute(DataFrame &df, const CommandArgs &args) override;
};

class StatsCommand : public InterfaceCommand {
public: void execute(DataFrame& df, const CommandArgs& args) override;
};

class HelpCommand : public InterfaceCommand {
public:
    void execute(DataFrame& df, const CommandArgs& args) override;
};


// command registry
class CommandRegistry {
private:
    std::unordered_map<std::string, std::shared_ptr<InterfaceCommand>> commands_;
public:
    CommandRegistry();
    std::shared_ptr<InterfaceCommand> get(const std::string &name);
};

// command parser
class CommandParser {
public: std::pair<std::string, CommandArgs> parse(const std::string &input);
};

// command handler
class CommandHandler {
private:
    std::unique_ptr<DataFrame> &df_;
    CommandRegistry &registry_;
public:
    CommandHandler(std::unique_ptr<DataFrame> &df, CommandRegistry &registry);
    void dispatch(const std::string &cmdName, const CommandArgs &args);
};

#endif //COMMAND_H
