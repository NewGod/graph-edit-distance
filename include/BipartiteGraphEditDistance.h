#ifndef __BIPARTITE_GED_H__
#define __BIPARTITE_GED_H__
#include "graph.h"
#include "lsape.h"
#include "utils.h"
class CostFunction{
    double NSubCost, NInsCost, NDelCost, ESubCost, EDelCost, EInsCost;
    public:
    CostFunction(
            double NSubCost, double NInsCost, double NDelCost,
            double ESubCost, double EDelCost, double EInsCost
            ):
        NSubCost(NSubCost), NInsCost(NInsCost), NDelCost(NDelCost),
        ESubCost(ESubCost), EDelCost(EDelCost), EInsCost(EInsCost)
    {
    }
    double NodeSubstitutionCost(Node *x, Node *y){
        if (x->attr == y->attr) return 0;
        else return NSubCost;
    }
    double NodeDeletionCost(){
        return NDelCost;
    }
    double NodeInsertionCost(){
        return NInsCost;
    }
    double EdgeInsertionCost(){
        return EInsCost;
    }
    double EdgeDeletionCost(){
        return EDelCost;
    }
    double EdgeSubstitutionCost(Edge* x, Edge* y){
        double ans = min(ESubCost, EInsCost + EDelCost);
        if (x->attr == y->attr) return 0;
        else return ans;
    }
};
double GedFromMapping(Graph* g1, Graph* g2, int* g1_to_g2, int* g2_to_g1, CostFunction* cf){
    double cost = 0, cost_edge = 0;
    int n = g1->node_num, m = g2->node_num;
    for (int i=0;i<n;i++) 
        if (g1_to_g2[i] >= m) {
            cost += cf->NodeDeletionCost();
        }else {
            cost += cf->NodeSubstitutionCost(g1->node[i], g2->node[g1_to_g2[i]]);
        }
    for (int i=0;i<m;i++) 
        if (g2_to_g1[i] >= n) {
            cost += cf->NodeInsertionCost();
        }
    for (int i = 0;i<n;i++) 
        for (auto iter = g1->node[i]->edges; iter; iter = iter->next) {
            int to = iter->to;
            int ff = g1_to_g2[i] , ft = g1_to_g2[to];
            if (ff < m && ft < m) {
                auto fe = g2->getedge(ff,ft);
                if (fe != NULL) {
                    cost_edge +=  cf->EdgeSubstitutionCost(iter, fe);
                }else {
                    cost_edge += cf->EdgeDeletionCost();
                }
            }else {
                cost_edge += cf->EdgeDeletionCost();
            }
        }
    for (int i = 0;i<m;i++) {
        for (auto iter = g2->node[i]->edges; iter; iter = iter->next) {
            int to = iter->to;
            int ff = g2_to_g1[i] , ft = g2_to_g1[to];
            if (ff < n && ft < n) {
                auto fe = g1->getedge(ff,ft);
                if (fe != NULL) {
                }else {
                    cost_edge += cf->EdgeInsertionCost();
                }
            }else {
                cost_edge += cf->EdgeInsertionCost();
            }
        }
    }
    cost_edge = cost_edge/2;
    cost += cost_edge;
    return cost;
}
class BipartiteGraphEditDistance{
    CostFunction *cf;
    double *C;
    double SubstitutionCost(Node* v1, Node* v2, Graph* g1, Graph* g2){ 
        double ans = cf->NodeSubstitutionCost(v1, v2);
        if (ans == inf) return ans;
        int n = v1->degree, m = v2->degree;
        double * local_C = new double[(n+1)*(m+1)];
        auto iter1 = v1->edges, iter2 = v2->edges;
        for (int i = 0;i<n;i++) {
            iter2 = v2->edges;
            for (int j = 0;j<m;j++) {
                local_C[i*(m+1)+j] = cf->EdgeSubstitutionCost(iter1, iter2);
                iter2 = iter2->next;
            }
            iter1 = iter1->next;
        }
        iter1 = v1->edges;
        for (int i = 0; i<n; i++) {
            local_C[i*(m+1)+m] = cf->EdgeInsertionCost();
            iter1 = iter1->next;
        }
        iter2 = v2->edges;
        for (int i = 0; i<m; i++) {
            local_C[n*(m+1)+i] = cf->EdgeDeletionCost();
            iter2 = iter2->next;
        }
        int *v1_to_v2 = new int[n+1], *v2_to_v1 = new int[m+1];
        LSAPE solver(local_C, n, m);
        solver.hungarianLSAPE(v1_to_v2, v2_to_v1);
        for (int i=0;i<=n;i++) ans += solver.u[i];
        for (int j=0;j<=m;j++) ans += solver.v[j];
        delete[] local_C;
        delete[] v1_to_v2;
        delete[] v2_to_v1;
        return ans;
    }
    double DeletionCost(Node* v1, Graph* g1){
        return v1->degree * cf->EdgeDeletionCost() + cf->NodeDeletionCost();
    }
    double InsertionCost(Node* v1, Graph* g1){
        return v1->degree * cf->EdgeInsertionCost() + cf->NodeInsertionCost();
    }
    void computeCostMatrix(Graph *g1, Graph *g2) {
        int n = g1->node_num;
        int m = g2->node_num;
        C = new double[(n+1)*(m+1)];
        for (int i = 0; i<n; i++) 
            for (int j = 0; j<m; j++) 
                C[i*(m+1) + j] = this->SubstitutionCost(g1->node[i], g2->node[j], g1, g2);
        for (int i = 0; i<n; i++) 
            C[i*(m+1)+m] = this->DeletionCost(g1->node[i], g1);
        for (int j = 0; j<m; j++)
            C[n*(m+1)+j] = this->InsertionCost(g2->node[j], g2);
        C[n*(m+1)+m] = 0;
    }
    public:
    BipartiteGraphEditDistance(CostFunction* cf):cf(cf){ 
    }
    double getOptimalMapping(Graph *g1, Graph *g2, int* g1_to_g2, int * g2_to_g1) {
        int n = g1->node_num;
        int m = g2->node_num;
        
        clock_t t = clock();
        computeCostMatrix(g1, g2);
        LSAPE(C, n, m).hungarianLSAPE(g1_to_g2, g2_to_g1);
        t = clock() - t;
        delete[] C;
        return ((float)t) / CLOCKS_PER_SEC;
    }
};
#endif
