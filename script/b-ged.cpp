#include<tinyxml.h>
#include<cstdio>
#include<iostream>
#include<algorithm>
#include"BipartiteGraphEditDistance.h"
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

int main(int argc, char **argv){
    struct Options * options = parseOptions(argc, argv);  
    CostFunction *cf = new CostFunction(options->NodeSubCost,options->NodeDelCost,options->NodeDelCost,options->EdgeSubCost, options->EdgeDelCost, options->EdgeDelCost);
    Graph *g1 = new Graph(options->dataset1_file.c_str());
    cout<<options->dataset2_file.c_str()<<endl;
    
    Graph *g2 = new Graph(options->dataset2_file.c_str());
    BipartiteGraphEditDistance *solver = new BipartiteGraphEditDistance(cf);

    int n = g1->node_num, m = g2->node_num;
    int* g1_to_g2 = new int[n+1], *g2_to_g1 = new int[m+1];
    double time = solver->getOptimalMapping(g1,g2,g1_to_g2,g2_to_g1);
    printf("%d %d\n",g1->node_num, g2->node_num);

    cout<<"2"<<endl;
    cout<<time<<endl;
    for (int i=0;i<n;i++) {
        if (g1_to_g2[i] != m) cout<<g1_to_g2[i]+1<<' ';
        else cout<<-1<<endl;
    }
    
    return 0;
}
