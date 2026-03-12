//
// Created by Egor Smirnov on 9.03.2026.
//

#include "../include/DataFrame.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <ostream>

DataFrame::DataFrame(const std::vector<std::string> &cols) : columns_(cols) {}
// columns
const std::vector<std::string> &DataFrame::get_columns() const { return columns_; }
void DataFrame::set_columns(const std::vector<std::string> &cols) { columns_ = cols;}
// rows
const std::vector<std::vector<std::string> > &DataFrame::get_rows() const { return data_; }
void DataFrame::add_row(const std::vector<std::string> &row) {
    if (row.size() != columns_.size()) {
        throw std::runtime_error("mismatch between rows and columns dimensions");
    }
    data_.push_back(row);
}
void DataFrame::add_row(std::vector<std::string> && row) {
    if (row.size() != columns_.size()) {
        throw std::runtime_error("mismatch between rows and columns dimensions");
    }
    data_.push_back(std::move(row));
}

size_t DataFrame::column_number() const { return columns_.size(); }
size_t DataFrame::row_number() const { return data_.size(); }

void DataFrame::print() const {
    for (const auto &col : columns_) std::cout << col << "\t";
    std::cout << "\n";
    for (const auto &item : data_) {
        for (const auto &cell : item) std::cout << cell << "\t";
        std::cout << "\n";
    }
}

void DataFrame::print(size_t lim = 10) const {
    for (const auto &col : columns_) std::cout << col << "\t";
    std::cout << "\n";
    size_t counter = 0;
    for (const auto &item : data_) {
        for (const auto &cell : item) std::cout << cell << "\t";
        std::cout << "\n";
        counter++;
        if (counter >= lim) break;
    }
    std::cout << "----------------------\nprinted " << counter << " rows\n";
}

bool DataFrame::to_csv(const std::string &path) const {
    // ex: 2013-11-26,26,November,2013,19,Youth (<25)
    //     2015-11-26,26,November,2015,19,Youth (<25)
    try {
        std::ofstream out(path); // closes file automatically => ok
        if (!out.is_open()) {
            std::cerr << "can't open the file by the path: " << path << "\n";
            return false;
        }

        // col names
        for (size_t i = 0; i < columns_.size(); ++i) {
            out << columns_[i];
            if (i + 1 < columns_.size()) out << ","; // .csv
        }
        out << "\n";
        // write rows by cells
        for (const auto &row : data_) {
            for (size_t i = 0; i < row.size(); ++i) {
                out << row[i];
                if (i + 1 < row.size()) out << ",";
            }
            out << "\n";
        }
        // out.close();
        return true;
    }
    catch (const std::exception &e) {
        std::cerr << "exception while saving .csv file: " << e.what() << std::endl;
        return false;
    }
}
// loc for rows
std::vector<std::string> DataFrame::operator[](size_t row) const {
    if (row >= data_.size()) throw std::out_of_range("row index out of range");
    return data_[row];
}

// loc for columns
std::vector<std::string> DataFrame::operator[](const std::string &col_name) const {
    auto it = std::find(columns_.begin(), columns_.end(), col_name);
    if (it == columns_.end()) throw std::out_of_range("column not found" + col_name);

    size_t col = it - columns_.begin();
    std::vector<std::string> result;
    result.reserve(data_.size());

    for (const auto &row : data_) result.push_back(row[col]);
    return result;
}

const std::string &DataFrame::loc(size_t row, size_t col) const {
    if (row > data_.size() || col >= columns_.size()) throw std::out_of_range("index out of range");
    return data_[row][col];
}
// .loc operations
DataFrame DataFrame::loc(const Slice &row_slice, const Slice &col_slice) const {
    size_t row_start = row_slice.start_;
    size_t row_end = std::min(row_slice.end_, data_.size());
    size_t col_start = col_slice.start_;
    size_t col_end = std::min(col_slice.start_, columns_.size());

    if (row_start > row_end || col_start > col_end) throw std::out_of_range("invalid indexing");

    std::vector<std::string> new_cols(columns_.begin() + col_start, columns_.begin() + col_end);
    DataFrame result(new_cols);

    for (size_t r = row_start; r < row_end; ++r) {
        std::vector<std::string> row;
        row.reserve(col_end - col_start);
        for (size_t c = col_start; c < col_end; ++c) {
            row.push_back(data_[r][c]);
        }
        result.add_row(row);
    }
    return result;
}
// matrix loc
std::vector<std::string> DataFrame::loc(size_t row, Slice &col_slice) const {
    if (row >= data_.size()) throw std::out_of_range("row index out of range");

    size_t col_start = col_slice.start_;
    size_t col_end = std::min(col_slice.end_, columns_.size());
    if (col_start > col_end) throw std::out_of_range("invalid indexing");

    std::vector<std::string> result;
    result.reserve(col_end - col_start);
    for (size_t c = col_start; c < col_end; ++c) result.push_back(data_[row][c]);
    return result;
}

// vector loc
std::vector<std::string> DataFrame::loc(Slice &row_slice, size_t col) const {
    if (col >= columns_.size()) throw std::out_of_range("col index out of range");

    size_t row_start = row_slice.start_;
    size_t row_end = std::min(row_slice.end_, columns_.size());
    if (row_start > row_end) throw std::out_of_range("invalid indexing");

    std::vector<std::string> result;
    result.reserve(row_end - row_start);
    for (size_t r = row_start; r < row_end; ++r) result.push_back(data_[r][col]);
    return result;
}


bool DataFrame::is_numeric_string(const std::string &s) {
    try {
        std::size_t pos;
        std::stod(s, &pos);
        return pos == s.size();
    } catch (...) { return false; }
}

bool DataFrame::is_int_string(const std::string &s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '-') ++i;
    for (; i < s.size(); ++i) {
        if (!std::isdigit(s[i])) return false;
    }
    return true;
}

bool DataFrame::is_numeric_vector(const std::vector<std::string> &v) {
    return std::all_of(v.begin(), v.end(), is_numeric_string);
}

bool DataFrame::is_int_vector(const std::vector<std::string> &v) {
    return std::all_of(v.begin(), v.end(), is_int_string);
}

std::vector<int> DataFrame::to_int_vector(const std::vector<std::string> &v) {
    if  (!is_numeric_vector(v)) throw std::runtime_error("vector has non numeric values");
    std::vector<int> result;
    result.reserve(v.size());
    for (const auto &item : v) {
        int temp = std::stoi(item);
        result.push_back(static_cast<int>(temp));
    }
    return result;
}

std::vector<double> DataFrame::to_double_vector(const std::vector<std::string> &v) {
    if  (!is_numeric_vector(v)) throw std::runtime_error("vector has non numeric values");
    std::vector<double> result;
    result.reserve(v.size());
    for (const auto &item : v) {
        double temp = std::stod(item);
        result.push_back(static_cast<double>(temp));
    }
    return result;
}

Shape DataFrame::shape() const {
    return { data_.size(), columns_.size()};
}







