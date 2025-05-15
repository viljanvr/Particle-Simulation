#include "SparseMatrix.h"

#include <iomanip>
#include <iostream>

SparseMatrix::SparseMatrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {}

std::vector<double> SparseMatrix::matVecMult(const std::vector<double> &x) const {
    std::vector<double> r(m_rows);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_i[index]] += m_v[index] * x[m_j[index]];
    }

    return r;
}

std::vector<double> SparseMatrix::matTransVecMult(const std::vector<double> &x) const {
    std::vector<double> r(m_cols, 0);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_j[index]] += m_v[index] * x[m_i[index]];
    }

    return r;
}

void SparseMatrix::addCell(size_t i, size_t j, double val) {
    m_i.push_back(i);
    m_j.push_back(j);
    m_v.push_back(val);
}

void SparseMatrix::debugPrint() const {
    std::cout << m_rows << ", " << m_cols << std::endl;
    std::vector<std::vector<double>> materialized_matrix(m_rows, std::vector<double>(m_cols, 0));
    for (size_t index = 0; index < m_v.size(); index++) {
        materialized_matrix[m_i[index]][m_j[index]] = m_v[index];
    }
    for (const auto& row: materialized_matrix) {
        for (const auto col: row) {
            std::cout << std::setw(10) << col << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

