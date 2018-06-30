#ifndef __UTILS_H__
#define __UTILS_H__
const double inf = 1e100;
double* inv(const double* c, int n, int m) {
    double* T = new double[n*m];
    for (int i=0;i<n;i++) 
        for (int j=0;j<m;j++) 
            T[j*n+i] = c[i*m+j];
    return T;
}
#endif
