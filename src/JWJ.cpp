#include "JWJ.h"

#include <iostream>

#include <ostream>
#include <utility>
#include <vector>


JWJ::JWJ(SparseMatrix J, std::vector<double> W) : m_W(std::move(W)), m_J(std::move(J))  {}

void JWJ::matVecMult(double x[], double r[]) {
    auto dim = m_J.getDim();
    auto rows = dim.first;
    auto cols = dim.second;

    std::vector<double> xVec(x, x + rows);

    std::vector<double> temp = m_J.matTransVecMult(xVec);

    for (size_t i = 0; i < cols; i++) {
        temp[i] *= m_W[i];
    }

    std::vector<double> result = m_J.matVecMult(temp);
    std::copy(result.begin(), result.end(), r);
}



