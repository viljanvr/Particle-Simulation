#include "JWJ.h"

#include <iostream>

#include <ostream>
#include <utility>
#include <vector>


JWJ::JWJ(SparseMatrix J, std::vector<double> W) : m_J(std::move(J)), m_W(std::move(W)) {}

void JWJ::matVecMult(double x[], double r[]) {
    auto [rows, cols] = m_J.getDim();

    double temp[cols];
    m_J.matTransVecMult(x, temp);


    for (size_t i = 0; i < rows; i++) {
        temp[i] *= m_W[i];
    }

    m_J.matVecMult(temp, r);
}



