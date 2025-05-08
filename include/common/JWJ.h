#pragma once
#include <vector>
#include "linearSolver.h"
#include "SparseMatrix.h"

class JWJ : public implicitMatrix {
public:
    JWJ(SparseMatrix J, std::vector<double> W);

    void matVecMult(double x[], double r[]) override;

private:
    std::vector<double> m_W;
    SparseMatrix m_J;
};


