#include "MatrixForImplicitEuler.h"

MatrixForImplicitEuler::MatrixForImplicitEuler(const SparseMatrix &Jx, const SparseMatrix &Jv, const double h) :
    m_Jx(Jx), m_Jv(Jv), m_h(h) {};

void MatrixForImplicitEuler::matVecMult(double x[], double r[]) {
    const size_t n = m_Jx.getDim().first;
    std::vector xVec(x, x + n);

    SparseMatrix matrix(n, n);
    for (size_t i = 0; i < n; i++) {
        matrix.addToCell(i, i, 1);
    }
    matrix += -(m_h * m_h) * m_Jx;
    matrix += -m_h * m_Jv;

    std::vector result = matrix.matVecMult(xVec);
    std::copy(result.begin(), result.end(), r);
}

