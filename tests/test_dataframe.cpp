#include <gtest/gtest.h>
#include "DataFrame.h"

TEST(DataFrameTest, ConstructWithColumns) {
    DataFrame df({"A", "B", "C"});

    EXPECT_EQ(df.column_number(), 3);
    EXPECT_EQ(df.row_number(), 0);

    auto cols = df.get_columns();
    EXPECT_EQ(cols[0], "A");
    EXPECT_EQ(cols[1], "B");
    EXPECT_EQ(cols[2], "C");
}

// adding rows
TEST(DataFrameTest, AddRowCorrectSize) {
    DataFrame df({"A", "B"});

    df.add_row({"1", "2"});
    df.add_row({"3", "4"});

    EXPECT_EQ(df.row_number(), 2);
    EXPECT_EQ(df[0][0], "1");
    EXPECT_EQ(df[1][1], "4");
}

TEST(DataFrameTest, AddRowWrongSizeThrows) {
    DataFrame df({"A", "B"});

    EXPECT_THROW(df.add_row({"1"}), std::runtime_error);
    EXPECT_THROW(df.add_row({"1", "2", "3"}), std::runtime_error);
}

// column indexing
TEST(DataFrameTest, ColumnIndexing) {
    DataFrame df({"A", "B"});
    df.add_row({"10", "20"});
    df.add_row({"30", "40"});

    auto colA = df["A"];
    auto colB = df["B"];

    ASSERT_EQ(colA.size(), 2);
    EXPECT_EQ(colA[0], "10");
    EXPECT_EQ(colA[1], "30");

    ASSERT_EQ(colB.size(), 2);
    EXPECT_EQ(colB[0], "20");
    EXPECT_EQ(colB[1], "40");
}

TEST(DataFrameTest, ColumnIndexingThrowsOnMissingColumn) {
    DataFrame df({"A", "B"});
    df.add_row({"1", "2"});

    EXPECT_THROW(df["C"], std::out_of_range);
}

// row indexing
TEST(DataFrameTest, RowIndexing) {
    DataFrame df({"A", "B"});
    df.add_row({"x", "y"});
    df.add_row({"p", "q"});

    auto row0 = df[0];
    auto row1 = df[1];

    EXPECT_EQ(row0[0], "x");
    EXPECT_EQ(row0[1], "y");
    EXPECT_EQ(row1[0], "p");
    EXPECT_EQ(row1[1], "q");
}

TEST(DataFrameTest, RowIndexingThrows) {
    DataFrame df({"A"});
    df.add_row({"1"});

    EXPECT_THROW(df[5], std::out_of_range);
}

// loc(row, col)
TEST(DataFrameTest, LocSingleCell) {
    DataFrame df({"A", "B"});
    df.add_row({"10", "20"});
    df.add_row({"30", "40"});

    EXPECT_EQ(df.loc(0, 0), "10");
    EXPECT_EQ(df.loc(1, 1), "40");
}

TEST(DataFrameTest, LocSingleCellThrows) {
    DataFrame df({"A"});
    df.add_row({"1"});

    EXPECT_THROW(df.loc(5, 0), std::out_of_range);
    EXPECT_THROW(df.loc(0, 5), std::out_of_range);
}

// loc(row_slice, col_slice)
TEST(DataFrameTest, LocRowColSlice) {
    DataFrame df({"A", "B", "C"});
    df.add_row({"1", "2", "3"});
    df.add_row({"4", "5", "6"});
    df.add_row({"7", "8", "9"});

    DataFrame sliced = df.loc(Slice(1, 3), Slice(0, 2));

    EXPECT_EQ(sliced.row_number(), 2);
    EXPECT_EQ(sliced.column_number(), 2);

    EXPECT_EQ(sliced.loc(0, 0), "4");
    EXPECT_EQ(sliced.loc(0, 1), "5");
    EXPECT_EQ(sliced.loc(1, 0), "7");
    EXPECT_EQ(sliced.loc(1, 1), "8");
}

// loc(row_slice, col_name)
TEST(DataFrameTest, LocRowSliceColName) {
    DataFrame df({"A", "B"});
    df.add_row({"10", "20"});
    df.add_row({"30", "40"});
    df.add_row({"50", "60"});

    auto col = df.loc(Slice(0, 3), "B");

    ASSERT_EQ(col.size(), 3);
    EXPECT_EQ(col[0], "20");
    EXPECT_EQ(col[1], "40");
    EXPECT_EQ(col[2], "60");
}

// conversion
TEST(DataFrameTest, ToDoubleVector) {
    std::vector<std::string> v = {"1.5", "2.5", "3.5"};
    auto result = DataFrame::to_double_vector(v);

    ASSERT_EQ(result.size(), 3);
    EXPECT_DOUBLE_EQ(result[0], 1.5);
    EXPECT_DOUBLE_EQ(result[1], 2.5);
    EXPECT_DOUBLE_EQ(result[2], 3.5);
}

TEST(DataFrameTest, ToDoubleVectorThrowsOnNonNumeric) {
    std::vector<std::string> v = {"1.0", "abc", "3.0"};
    EXPECT_THROW(DataFrame::to_double_vector(v), std::runtime_error);
}

// shape
TEST(DataFrameTest, Shape) {
    DataFrame df({"A", "B"});
    df.add_row({"1", "2"});
    df.add_row({"3", "4"});

    Shape s = df.shape();
    EXPECT_EQ(s.rows_, 2);
    EXPECT_EQ(s.columns_, 2);
}
