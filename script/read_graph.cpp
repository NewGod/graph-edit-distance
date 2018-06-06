#include<tinyxml.h>
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<graph.h>
#include<sstream>
#include<unistd.h>

using namespace std;


void usage (char * s)
{
  cerr << "Usage : "<< s << " dataset " << " options"<<endl;
  cerr << "options:" << endl;
  cerr << "\t -t" << endl;
  cerr << "\t \t Specify the type of the dataset" << endl;
  cerr << "\t -o" << endl;
  cerr << "\t \t Specify the output file" << endl;
}

struct Options{
  string dataset_file = "";
  string type = "";
  string output_file = "";
};

struct Options * parseOptions(int argc, char** argv){
  struct Options * options = new struct Options();
  options->dataset_file = string(argv[1]);
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

struct EdgeAttribute{
    int from, to, type;
    EdgeAttribute(TiXmlElement *elem){
        from = stoi((elem->Attribute("from")+1));
        to = stoi((elem->Attribute("to")+1));
        auto x = elem->FirstChildElement();
        type = stoi(x->GetText());
    }
};
struct NodeAttribute{
    int id, type;
    NodeAttribute(TiXmlElement *elem){
        id = stoi((elem->Attribute("id")) + 1);
        auto x = elem->FirstChildElement();
        type = stoi(x->GetText());
    }
};
int main(int argc, char **argv){
    struct Options * options = parseOptions(argc, argv);  
    auto graph = Graph<NodeAttribute, EdgeAttribute>(options->dataset_file.c_str());
    for (auto iter: graph.node){
        printf("Node: %d type %d\n", iter->attr.id, iter->attr.type);
        auto x = iter->edges;
        while (x){
            printf("\t Edge from %d to %d type %d\n",x->attr.from, x->attr.to, x->attr.type);
        }
        printf("\n");
    }
    return 0;
}
