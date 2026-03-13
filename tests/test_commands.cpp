#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "command.h"
#include "CSVLoader.h"
#include "DataFrame.h"

// helper: capture stdout
static std::string capture_output(std::function<void()> fn) {
    testing::internal::CaptureStdout();
    fn();
    return testing::internal::GetCapturedStdout();
}

// helper: write a temporary CSV
static std::string write_temp_csv(const std::string& name, const std::string& content) {
    std::filesystem::create_directories("tests");
    std::string path = "tests/" + name;
    std::ofstream out(path);
    out << content;
    return path;
}

// CommandParser tests
TEST(CommandParserTest, ParseLoad) {
    CommandParser parser;
    auto [cmd, args] = parser.parse("load data.csv");

    EXPECT_EQ(cmd, "load");
    EXPECT_EQ(std::get<LoadArgs>(args).path_, "data.csv");
}

TEST(CommandParserTest, ParseHead) {
    CommandParser parser;
    auto [cmd, args] = parser.parse("head 5");

    EXPECT_EQ(cmd, "head");
    EXPECT_EQ(std::get<HeadArgs>(args).n_, 5);
}

TEST(CommandParserTest, ParseSlice) {
    CommandParser parser;
    auto [cmd, args] = parser.parse("slice 2 7 Column");

    EXPECT_EQ(cmd, "slice");
    auto s = std::get<SliceArgs>(args);
    EXPECT_EQ(s.rows_.start_, 2);
    EXPECT_EQ(s.rows_.end_, 7);
    EXPECT_EQ(s.col_, "Column");
}

TEST(CommandParserTest, ParseStats) {
    CommandParser parser;
    auto [cmd, args] = parser.parse("stats Profit");

    EXPECT_EQ(cmd, "stats");
    EXPECT_EQ(std::get<StatsArgs>(args).col, "Profit");
}

// CommandRegistry tests
TEST(CommandRegistryTest, RegistryReturnsCommands) {
    CommandRegistry reg;

    EXPECT_TRUE(reg.get("load") != nullptr);
    EXPECT_TRUE(reg.get("head") != nullptr);
    EXPECT_TRUE(reg.get("slice") != nullptr);
    EXPECT_TRUE(reg.get("plot") != nullptr);
    EXPECT_TRUE(reg.get("stats") != nullptr);
    EXPECT_TRUE(reg.get("help") != nullptr);

    EXPECT_EQ(reg.get("unknown"), nullptr);
}

// LoadCommand
TEST(CommandTest, LoadCommandLoadsCSV) {
    std::string path = write_temp_csv(
        "load_test.csv",
        "A,B\n"
        "1,2\n"
        "3,4\n"
    );

    DataFrame df({"X"}); // dummy
    LoadCommand cmd;

    auto out = capture_output([&]() {
        cmd.execute(df, LoadArgs{path});
    });

    EXPECT_NE(out.find("loaded"), std::string::npos);
    EXPECT_EQ(df.row_number(), 2);
    EXPECT_EQ(df.loc(0, 0), "1");
}

// HeadCommand
TEST(CommandTest, HeadCommandPrintsRows) {
    DataFrame df({"A", "B"});
    df.add_row({"1", "2"});
    df.add_row({"3", "4"});

    HeadCommand cmd;

    auto out = capture_output([&]() {
        cmd.execute(df, HeadArgs{1});
    });

    EXPECT_NE(out.find("1\t2"), std::string::npos);
    EXPECT_NE(out.find("printed 1 rows"), std::string::npos);
}

// SliceCommand
TEST(CommandTest, SliceCommandPrintsColumnSlice) {
    DataFrame df({"A", "B"});
    df.add_row({"10", "20"});
    df.add_row({"30", "40"});
    df.add_row({"50", "60"});

    SliceCommand cmd;

    auto out = capture_output([&]() {
        cmd.execute(df, SliceArgs{Slice(1, 3), "A"});
    });

    EXPECT_NE(out.find("30"), std::string::npos);
    EXPECT_NE(out.find("50"), std::string::npos);
}

// StatsCommand
TEST(CommandTest, StatsCommandComputesCorrectStats) {
    DataFrame df({"X"});
    df.add_row({"1"});
    df.add_row({"2"});
    df.add_row({"3"});
    df.add_row({"4"});

    StatsCommand cmd;

    auto out = capture_output([&]() {
        cmd.execute(df, StatsArgs{"X"});
    });

    EXPECT_NE(out.find("mean: 2.5"), std::string::npos);
    EXPECT_NE(out.find("variance: 1.25"), std::string::npos);
    EXPECT_NE(out.find("min: 1"), std::string::npos);
    EXPECT_NE(out.find("max: 4"), std::string::npos);
}

// CommandHandler
TEST(CommandHandlerTest, DispatchCallsCorrectCommand) {
    auto df = std::make_unique<DataFrame>(std::vector<std::string>{"A"});
    df->add_row({"10"});

    CommandRegistry reg;
    CommandHandler handler(df, reg);

    auto out = capture_output([&]() {
        handler.dispatch("stats", StatsArgs{"A"});
    });

    EXPECT_NE(out.find("mean"), std::string::npos);
}

TEST(CommandTest, PlotCommandCreatesFile) {
    DataFrame df({"X", "Y"});
    df.add_row({"1", "2"});
    df.add_row({"3", "4"});

    PlotCommand cmd;

    // remove old file if exists
    if (std::filesystem::exists("plot.png"))
        std::filesystem::remove("plot.png");

    auto out = capture_output([&]() {
        cmd.execute(df, PlotArgs{"X", "Y"});
    });

    EXPECT_TRUE(std::filesystem::exists("plot.png"));
    EXPECT_NE(out.find("plot saved"), std::string::npos);
}
