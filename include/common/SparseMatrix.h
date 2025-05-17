#pragma once
#include <vector>
#include "linearSolver.h"

#include <unordered_map>

class SparseMatrix {
public:
    SparseMatrix(size_t rows, size_t cols);
    [[nodiscard]] std::vector<double> matVecMult(const std::vector<double> &x) const;
    [[nodiscard]] std::vector<double> matTransVecMult(const std::vector<double> &x) const;
    void addToCell(size_t i, size_t j, double val);
    void addToCell(size_t index, double val);
    [[nodiscard]] std::pair<size_t, size_t> getDim() const { return {m_rows, m_cols}; };

    void debugPrint() const;

    void operator+=(const SparseMatrix &rhs);

    template<typename T>
    SparseMatrix operator*(T c) const {
        SparseMatrix res(m_rows, m_cols);
        for (auto &cell: cells) {
            res.addToCell(cell.first, cell.second * c);
        }
        return res;
    };

    template<typename T>
    friend SparseMatrix operator*(T c, const SparseMatrix &rhs) {
        return rhs * c;
    };


private:
    [[nodiscard]] size_t index(size_t i, size_t j) const;
    [[nodiscard]] std::pair<size_t, size_t> coordinates(size_t index) const;
    const size_t m_rows;
    const size_t m_cols;
    // indexed by i + j * m_rows
    std::unordered_map<size_t, double> cells;
};
