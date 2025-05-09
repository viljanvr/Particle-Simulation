#include "SparseMatrix.h"

#include <iomanip>
#include <iostream>

SparseMatrix::SparseMatrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {}

void SparseMatrix::matVecMult(double x[], double r[]) {
    std::fill(r, r + m_rows, 0);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_i[index]] += m_v[index] * x[m_j[index]];
    }
}

void SparseMatrix::matTransVecMult(double x[], double r[]) {
    std::fill(r, r + m_rows, 0);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_j[index]] += m_v[index] * x[m_i[index]];
    }
}

void SparseMatrix::addCell(size_t i, size_t j, double val) {
    m_i.push_back(i);
    m_j.push_back(j);
    m_v.push_back(val);
}

void SparseMatrix::debugPrint() {
    std::cout << m_rows << ", " << m_cols << std::endl;
    std::vector<std::vector<double>> materialized_matrix(m_rows, std::vector<double>(m_cols, 0));
    for (size_t index = 0; index < m_v.size(); index++) {
        materialized_matrix[m_i[index]][m_j[index]] = m_v[index];
    }
    for (auto row: materialized_matrix) {
        for (auto col: row) {
            std::cout << std::setw(10) << col << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

