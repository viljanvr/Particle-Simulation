#pragma once
#include <vector>
#include "linearSolver.h"

class SparseMatrix {
public:
    SparseMatrix(size_t rows, size_t cols);
    void matVecMult(double x[], double r[]);
    void matTransVecMult(double x[], double r[]);
    void addCell(size_t i, size_t j, double val);
    std::pair<size_t, size_t> getDim() { return {m_rows, m_cols}; };


private:
    size_t m_rows;
    size_t m_cols;
    std::vector<size_t> m_i; // rows
    std::vector<size_t> m_j; // columns
    std::vector<double> m_v;
};
