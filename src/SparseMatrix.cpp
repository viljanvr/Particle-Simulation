#include "SparseMatrix.h"

#include <iomanip>
#include <iostream>

SparseMatrix::SparseMatrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {}

size_t SparseMatrix::index(size_t i, size_t j) const {
    return i + j * m_rows;
}

std::pair<size_t, size_t> SparseMatrix::coordinates(size_t idx) const {
    return {idx % m_rows, idx / m_rows };
}

std::vector<double> SparseMatrix::matVecMult(const std::vector<double> &x) const {
    std::vector<double> r(m_rows);

    for (auto cell : cells) {
        auto coords = coordinates(cell.first);
        double value = cell.second;
        size_t i = coords.first;
        size_t j = coords.second;
        r[i] += value * x[j];
    }

    return r;
}

std::vector<double> SparseMatrix::matTransVecMult(const std::vector<double> &x) const {
    std::vector<double> r(m_cols, 0);

    for (auto cell : cells) {
        auto coords = coordinates(cell.first);
        double value = cell.second;
        size_t i = coords.first;
        size_t j = coords.second;
        r[j] += value * x[i];
    }

    return r;
}

void SparseMatrix::addToCell(size_t i, size_t j, double val) {
    size_t idx = index(i, j);
    addToCell(idx, val);
}

void SparseMatrix::addToCell(size_t idx, double val) {
    cells.try_emplace(idx, 0);
    cells[idx] += val;
}


void SparseMatrix::operator+=(const SparseMatrix &rhs) {
    if (rhs.getDim() != getDim()) {
        throw std::runtime_error("SparseMatrix::operator+=: Dimension mismatch.");
    }
    for (auto cell : rhs.cells) {
        size_t idx = cell.first;
        size_t value = cell.second;
        addToCell(idx, value);
    }
}

void SparseMatrix::debugPrint() const {
    std::cout << m_rows << ", " << m_cols << std::endl;
    std::vector<std::vector<double>> materialized_matrix(m_rows, std::vector<double>(m_cols, 0));

    for (auto cell : cells) {
        auto coords = coordinates(cell.first);
        double value = cell.second;
        size_t i = coords.first;
        size_t j = coords.second;
        materialized_matrix[i][j] = value;
    }

    for (const auto& row: materialized_matrix) {
        for (const auto col: row) {
            std::cout << std::setw(10) << col << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


