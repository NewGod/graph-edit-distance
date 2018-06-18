#ifdef __LSAPE__
#define __LSAPE__
// x[cols][rows]
#include "utils.h"
class Assign{
    int n, m;
    int *rho, *varrho;
    double *u, *v;
    const double *C;// a array with (n+1)x(m+1) 
    double get(i,j){
        return C[i*(m+1)+j];
    }
    int AugmentCol(const int &k, const int* pred, int& zi,int & zj) {
        int U[n];
        int SV[n];
        int Uptr = 0, LUptr = -1, *SUptr = -1, SVcnt=0;
        double pi[n];
        for (int i=0;i<n;i++) pi[n] = inf;
        for (int i=0;i<n;i++) U[i] = i+1;
        U[n-1] = -1;
        Uptr = U
        int j = k;
        while (true){
            SV[SVcnt++] = j; 
            if (varrho[j] < n && get(n,j-v[j] == 0)){
                zi = n; zj = j;
                return ;
            }
            for (int i= Uptr, nxt = U[i]; i!=-1;i = nxt, nxt = U[i])
                if (get(i,j) - u[i] - v[j] < pi[i]) {
                    pred[i] = j; 
                    pi[i] = get(i,j) - u[i] - v[i];
                    if (pi[i] == 0) {
                        if (rho[i] == -1 || rho[i] == m) {
                            zi = i; zj = -1;
                            return; 
                        }
                        if (Uptr == i) Uptr = U[i];
                        U[i] = LUptr;
                        LUptr = i;
                    }
                }
            if (LUptr == -1) {
                double delta_eps = inf, delta_s = inf, delta = inf;
                int l = -1;
                for (int i = 0; i<SVcnt;i++) 
                    if (get(n+1, SV[i]) - v[SV[i]] < delta_eps) {
                        delta_eps = get(n+1, SV[i]) - v[SV[i]] ;
                        l = SV[i];
                    }
                for (int i= Uptr, nxt = U[i]; i!=-1;i = nxt, nxt = U[i])
                    delta_s = min(delta_s, pi[i]);
                delta = min(delta_s, delta_eps);
                for (int i = 0; i<SVcnt;i++) 
                    v[SV[i]] += delta;
                for (int i= LUptr, nxt = U[i]; i!=-1;i = nxt, nxt = U[i])
                    u[i] -= delta;
                for (int i= SUptr, nxt = U[i]; i!=-1;i = nxt, nxt = U[i])
                    u[i] -= delta;
                if (delta_eps <= delta_s) {
                    zi = n;
                    zj = l;
                    return ;
                }
                for (int i= Uptr, nxt = U[i]; i!=-1;i = nxt, nxt = U[i]){
                    pi[i] -= delta;
                    if (pi[i] == 0) 
                        if (rho[i] != -1) {
                            zi = i; zj = -1; 
                            return ;
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
    Augment(const double *c, const int&n, const int &m,
            const int *rho, const int *varrho, 
            const double *u, const double *v
            ):
        n(n),m(m),rho(rho),varrho(varrho),u(u),v(v){}
    void assignCols(){
        int *pred = int[n+1];
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
}
class LSAPE{
    int n, m;
    int *rho, *varrho;
    double *u, *v;
    const double *C;// a array with (n+1)x(m+1) 
    lsape(const double *c, const int &n, const int &m):n(n),m(m),c(c){
        rho = new int[n];
        varrho = new int[m];
        u = new double[n+1];
        v = new double[m+1];
    }
    double get(i,j){
        return C[i*(m+1)+j];
    }
    void PreProcessing(){// remeber that u[n] and v[m] is undefine
        const int n = rows-1, m = cols-1;
        for (int i=0;i<n;i++) {
            u[i] = inf;
            for (int j=0;j<cols;j++) 
                if (u[i] > get(i,j)) u[i] = get(i,j);
            rho[i]=-1;
        }
        for (int j=0;j<m;j++) {
            for (int i=0;i<rows;i++) 
                if (v[j] > get(i,j) - u[i]) v[i] = get(i,j) - u[i];
            varrho[j] = -1;
        }
        for (int i=0;i<n;i++) 
            for (int j=0;j<m;j++) 
                if (rho[i] < 0 && varrho[j] < 0 && get(i,j) == u[i] + v[j]){
                    rho[i] = j;
                    varrho[j] = i;
                    break;
                }
        for (int i = 0; i<n; i++)
            if (rho[i] < 0 && get(i,m) == u[i]) {
                rho[i] = m; 
            }
        for (int j = 0;j<m;j++) 
            if (varrho[i] < 0 && get(n,j) == v[j]) {
                varrho[j] = n;
            }
    }
    public:
    lsape(const double *c, const int &n, const int &m):n(n),m(m),c(c){
        rho = new int[n];
        varrho = new int[m];
        u = new double[n+1];
        v = new double[m+1];
    }
    void hungarianLSAPE(int *rho, int *varrho) {
        int nass = 0;
        int mass = 0;
        pre-process();
        Assign(C, n, m, rho, varrho, u, v).assignCols();
        Assign(inv(C, V+1, m+1), m, n, varrho, rho, v, u).assignCols();
    }
}
#endif 
