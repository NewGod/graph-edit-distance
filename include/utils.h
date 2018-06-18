#ifdef __UTILS_H__
#define __UTILS_H__
template<typename DT> 
DT* inv(DT* c, int n, int m) {
    DT* T = new DT[n*m];
    for (int i=0;i<n;i++) 
        for (int j=0;j<m;j++) 
            T[j*n+i] = c[i*m+j];
    return T;
}
#endif
