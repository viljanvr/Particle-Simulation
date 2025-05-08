#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {}

void SparseMatrix::matVecMult(double x[], double r[]) {
    std::fill(r, r + m_rows, 0);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_i[index]] = m_v[index] * x[m_j[index]];
    }
}

void SparseMatrix::matTransVecMult(double x[], double r[]) {
    std::fill(r, r + m_rows, 0);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_j[index]] = m_v[index] * x[m_i[index]];
    }
}

void SparseMatrix::addCell(size_t i, size_t j, double val) {
    m_i.push_back(i);
    m_j.push_back(j);
    m_v.push_back(val);
}
