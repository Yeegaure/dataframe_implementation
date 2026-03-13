//
// Created by Egor Smirnov on 9.03.2026.
//

#ifndef DATAFRAME_H
#define DATAFRAME_H
#include <vector>
#include <string>
#include <cstdint>
#include <ostream>

struct Slice {
    size_t start_, end_; // exclusive ~ in python
    constexpr Slice(size_t start, size_t end) : start_(start), end_(end) {}
    static constexpr Slice all() { return Slice(0, SIZE_MAX); }
};

// shape return
struct Shape {
    size_t rows_;
    size_t columns_;
};

inline std::ostream &operator<<(std::ostream &os, const Shape &shape) {
    os << "shape: (" << shape.rows_ << " x " << shape.columns_ << ')' << "\n";
    return os;
}

class DataFrame {
private:
    std::vector<std::string> columns_;
    std::vector<std::vector<std::string>> data_;

    size_t column_index(const std::string& col_name) const;

    static bool is_numeric_string(const std::string &s);
    static bool is_int_string(const std::string &s);

    static bool is_numeric_vector(const std::vector<std::string> &v);
    static bool is_int_vector(const std::vector<std::string> &v);

public:
    DataFrame() = default;
    explicit DataFrame(const std::vector<std::string> &cols);

    // columns
    const std::vector<std::string> &get_columns() const;
    void set_columns(const std::vector<std::string> &cols);
    // rows
    const std::vector<std::vector<std::string>> &get_rows() const;
    void add_row(const std::vector<std::string> &row);
    void add_row(std::vector<std::string> && row);
    // shapes
    size_t column_number() const;
    size_t row_number() const;
    // printing
    void print() const;
    void print(size_t lim) const;
    // load
    bool to_csv(const std::string &path) const;
    // []
    std::vector<std::string> operator[](size_t row) const;
    std::vector<std::string> operator[](const std::string &col_name) const;
    // loc()
    const std::string &loc(size_t row, size_t col) const;
    DataFrame loc(const Slice &row_slice, const Slice &col_slice) const;
    std::vector<std::string> loc(size_t row, const Slice &col_slice) const;
    std::vector<std::string> loc(const Slice &row_slice, size_t col) const;
    // loc() name-based overloads
    const std::string& loc(size_t row, const std::string &col_name) const;
    DataFrame loc(const Slice &row_slice, const std::vector<std::string> &col_names) const;
    std::vector<std::string> loc(const Slice &row_slice, const std::string &col_name) const;

    // conversion vector: string -> int/double
    static std::vector<int> to_int_vector(const std::vector<std::string> &v);
    static std::vector<double> to_double_vector(const std::vector<std::string> &v);

    Shape shape() const;
};

#endif //DATAFRAME_H
