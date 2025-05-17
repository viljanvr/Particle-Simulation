#pragma once
#include <vector>
#include "linearSolver.h"
#include "SparseMatrix.h"

class MatrixForImplicitEuler : public implicitMatrix {
public:
    MatrixForImplicitEuler(const SparseMatrix& Jx, const SparseMatrix& Jv, const double h);

    void matVecMult(double x[], double r[]) override;

private:
    const SparseMatrix& m_Jx;
    const SparseMatrix& m_Jv;
    const double m_h;
};