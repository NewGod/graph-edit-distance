#ifndef __LSAPE_H_
#define __LSAPE_H_
// x[cols][rows]
#include "utils.h"
#include<cstdio>
#include<algorithm>
class Assign{
    int n, m;
    int *rho, *varrho;
    double *u, *v;
    const double *C;// a array with (n+1)x(m+1) 
    double get(int i,int j){
        return C[i*(m+1)+j];
    }
    void AugmentCol(const int &k, int* const pred, int& zi,int & zj) {
        int U[n];
        int SV[n];
        int Uptr = -1, LUptr = -1, SUptr = -1, SVcnt=0;
        double pi[n];
        for (int i=0;i<n;i++) pi[i] = inf;
        for (int i=0;i<n;i++) U[i] = i+1;
        U[n-1] = -1;
        Uptr = 0;
        int j = k;
        while (true){
            SV[SVcnt++] = j; 
            if (varrho[j] < n && get(n,j)-v[j] == 0){
                zi = n; zj = j;
                return ;
            }
            for (int i= Uptr, prev = -1; i!=-1;prev = i, i = U[i])
                if (get(i,j) - u[i] - v[j] < pi[i]) {
                    pred[i] = j; 
                    pi[i] = get(i,j) - u[i] - v[i];
                    if (pi[i] == 0) {
                        if (rho[i] == -1 || rho[i] == m) {
                            zi = i; zj = -1;
                            return; 
                        }
                        if (Uptr == i) Uptr = U[i];
                        if (prev != -1) U[prev] = U[i];
                        U[i] = LUptr;
                        LUptr = i;
                    }
                }
            if (LUptr == -1) {
                double delta_eps = inf, delta_s = inf, delta = inf;
                int l = -1;
                for (int i = 0; i<SVcnt;i++) 
                    if (get(n, SV[i]) - v[SV[i]] < delta_eps) {
                        delta_eps = get(n, SV[i]) - v[SV[i]] ;
                        l = SV[i];
                    }
                for (int i= Uptr, nxt = U[i]; i!=-1;i = nxt, nxt = U[i])
                    delta_s = std::min(delta_s, pi[i]);
                delta = std::min(delta_s, delta_eps);
                for (int i = 0; i<SVcnt;i++) 
                    v[SV[i]] += delta;
                for (int i= LUptr; i!=-1;i = U[i])
                    u[i] -= delta;
                for (int i= SUptr; i!=-1;i = U[i])
                    u[i] -= delta;
                if (delta_eps <= delta_s) {
                    zi = n;
                    zj = l;
                    return ;
                }
                for (int i= Uptr, prev = -1; i!=-1;prev = i, i = U[i]){
                    pi[i] -= delta;
                    if (pi[i] == 0) {
                        if (rho[i] == -1 or rho[i]==m) {
                            zi = i; zj = -1; 
                            return ;
                        }
                        if (Uptr == i) Uptr = U[i];
                        if (prev != -1) U[prev] = U[i];
                        U[i] = LUptr;
                        LUptr = i;
                    }
                }
            }
            if (LUptr!=-1) {
                int tmp = U[LUptr];
                U[LUptr] = SUptr;
                SUptr = LUptr;
                LUptr = tmp;
                j = rho[SUptr];
            }
        }
    }
    public:
    Assign(const double *c, const int&n, const int &m,
            int *const rho, int * const varrho, 
            double *const u, double *const v
            ):
        n(n),m(m),rho(rho),varrho(varrho),u(u),v(v),C(c){}
    void assignCols(){
        int pred[n+1];
        for (int k=0; k<m;k++) {
            if (varrho[k] == -1) {
                int i,j,r;
                AugmentCol(k, pred, i, j);
                if (i == n) {
                    r = varrho[j]; varrho[j] = i; i = r;
                }
                else j = -1;
                while (j!=k){
                    j = pred[i];
                    rho[i] = j;
                    r = varrho[j];
                    varrho[j] = i;
                    i = r;
                }
            }
        }
    }
};
class LSAPE{
    int n, m;
    const double *C;// a array with (n+1)x(m+1) 
    double get(int i,int j){
        return C[i*(m+1)+j];
    }
    void PreProcessing(int * const rho, int * const varrho){
        u[n] = v[m] = 0;
        for (int i=0;i<n;i++) {
            u[i] = inf;
            for (int j=0;j<=m;j++) 
                if (u[i] > get(i,j)) u[i] = get(i,j);
            rho[i]=-1;
        }
        for (int j=0;j<m;j++) {
            v[j] = inf;
            for (int i=0;i<=n;i++) 
                if (v[j] > get(i,j) - u[i]) v[j] = get(i,j) - u[i];
            varrho[j] = -1;
        }
        for (int i=0;i<n;i++) {
            for (int j=0;j<m;j++) 
                if (varrho[j] < 0 && get(i,j) == u[i] + v[j]){
                    rho[i] = j;
                    varrho[j] = i;
                    break;
                }
        }
        for (int i=0;i<n;i++) 
            if (rho[i] < 0 && get(i,m) == u[i]) {
                rho[i] = m; 
            }
        for (int j = 0;j<m;j++) 
            if (varrho[j] < 0 && get(n,j) == v[j]) {
                varrho[j] = n;
            }
    }
    public:
    double *u, *v;
    LSAPE(double *const c, const int &n, const int &m):n(n),m(m),C(c){
        u = new double[n+1];
        v = new double[m+1];
    }
    ~LSAPE(){
        delete[] u;
        delete[] v;
    }
    void hungarianLSAPE(int *rho , int *varrho) {
        PreProcessing(rho, varrho);
        Assign(C, n, m, rho, varrho, u, v).assignCols();
        double *V = inv(C, n+1, m+1);
        Assign(V, m, n, varrho, rho, v, u).assignCols();
        delete[] V;
        return;
    }
};
#endif 
