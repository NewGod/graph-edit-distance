#include<tinyxml.h>
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<lsape.h>
#include<sstream>
#include<unistd.h>

using namespace std;


void usage (char * s){
  cerr << "Usage : "<< s << "node_sub node_del/ins edge_sub edge_del/ins g1.gxl g2.gxl" << " options"<<endl;
  cerr << "options:" << endl;
  cerr << "\t -t" << endl;
  cerr << "\t \t Specify the type of the dataset" << endl;
  cerr << "\t -o" << endl;
  cerr << "\t \t Specify the output file" << endl;
}

struct Options{
    double NodeSubCost, NodeDelCost, EdgeSubCost, EdgeDelCost;
  string dataset1_file = "";
  string dataset2_file = "";
  string type = "";
  string output_file = "";
};

struct Options * parseOptions(int argc, char** argv){
  struct Options * options = new struct Options();
  if (argc < 7) {
      usage(argv[0]);
      exit(EXIT_FAILURE);
  }
  options->NodeSubCost = stoi(argv[1]);
  options->NodeDelCost = stoi(argv[2]);
  options->EdgeSubCost = stoi(argv[3]);
  options->EdgeDelCost = stoi(argv[4]);
  options->dataset1_file = string(argv[5]);
  options->dataset2_file = string(argv[6]);
  int opt;
  stringstream sstream;
  while ((opt = getopt(argc, argv, "o:t")) != -1) {
    switch (opt) {
    case 'o':
      options->output_file = string(optarg);
      break;
    case 't':
      options->type = string(optarg);
      break;
    default: /* '?' */
      cerr << "Options parsing failed."  << endl;
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  return options;
}

int n,m;
int main(int argc, char **argv){
    scanf("%d %d\n",&n,&m);
    double *c = new double[(n+1)*(m+1)];
    for (int i=0;i<=n;i++)
        for (int j=0;j<=m;j++) scanf("%lf\n",&c[i*(m+1)+j]);
    LSAPE solve = LSAPE(c, n, m);
    int *rho = new int[n], *varrho = new int[m];
    solve.hungarianLSAPE(rho, varrho);
    for (int i=0;i<=n;i++) printf("%lf ",solve.u[i]);
    puts("");
    for (int i=0;i<=m;i++) printf("%lf ",solve.v[i]);
    puts("");
    for (int i=0;i<n;i++) printf("%d ",rho[i]);
    puts("");
    for (int i=0;i<m;i++) printf("%d ",varrho[i]);
    return 0;
}
