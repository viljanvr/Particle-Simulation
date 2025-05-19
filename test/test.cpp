#include <stdio.h>
#include <stdlib.h>
#include <vector>


size_t m_rows = 2;
std::vector<int> m_i = {0, 0, 1};
std::vector<int> m_j = {0, 1, 1};
std::vector<float> m_v = {1.0, 1.0, 2.0};

double x[] = {0.0, 1.0, 1.0};

/* Sparse Matrix
1 1 0
0 2 0
0 0 0
*/

/* Result
2
2
0
*/

/* Sparse Matrix transposed
1 0 0
1 2 0
0 0 0
*/

/* Result
1
3
0
*/


void matVecMult(double x[], double r[]) {
    std::fill(r, r + m_rows, 0);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_i[index]] += m_v[index] * x[m_j[index]];
        // for (int i = 0; i < 3; i++) {
        //     printf("%f\n", r[i]);
        // }
        // printf("--\n");
    }
}

void matTransVecMult(double x[], double r[]) {
    std::fill(r, r + m_rows, 0);

    for (size_t index = 0; index < m_v.size(); index++) {
        r[m_j[index]] += m_v[index] * x[m_i[index]];
    }
}

void addCell(size_t i, size_t j, double val) {
    m_i.push_back(i);
    m_j.push_back(j);
    m_v.push_back(val);
}

int main(int argc, char **argv) {
    double r1[] = {0, 0, 0};

    matVecMult(x, r1);
    for (int i = 0; i < 3; i++) {
        printf("%f\n", r1[i]);
    }

    double r2[] = {0, 0, 0};
    matTransVecMult(x, r2);
    for (int i = 0; i < 3; i++) {
        printf("%f\n", r2[i]);
    }
}
