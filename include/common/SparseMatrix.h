#pragma once
#include <vector>
#include "linearSolver.h"

class SparseMatrix {
public:
    SparseMatrix(size_t rows, size_t cols);
    [[nodiscard]] std::vector<double> matVecMult(const std::vector<double> &x) const;
    [[nodiscard]] std::vector<double> matTransVecMult(const std::vector<double> &x) const;
    void addCell(size_t i, size_t j, double val);
    [[nodiscard]] std::pair<size_t, size_t> getDim() const { return {m_rows, m_cols}; };

     void debugPrint() const;


private:
    size_t m_rows;
    size_t m_cols;
    std::vector<size_t> m_i; // rows
    std::vector<size_t> m_j; // columns
    std::vector<double> m_v;
};
