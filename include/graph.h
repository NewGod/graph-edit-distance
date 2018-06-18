#ifndef __GRAPH__
#define __GRAPH__

#include<algorithm>
#include<vector>
#include<map>
#include<iostream>

#include<tinyxml.h>
using namespace std;
struct EdgeAttribute{
    int from, to, type;
    EdgeAttribute(TiXmlElement *elem){
        const char *_from = elem->Attribute("from");
        const char *_to = elem->Attribute("to");
        if (_from[0] == '_') {
            _from++;_to++;
        }
        from = stoi(_from);
        to = stoi(_to);
        auto x = elem->FirstChildElement();
        type = stoi(x->FirstChild()->FirstChild()->Value());
    }
};
struct NodeAttribute{
    int id;
    string type;
    NodeAttribute(TiXmlElement *elem){
        const char *_id = elem->Attribute("id");
        if (_id[0] == '_') _id++;
        id = stoi(_id);
        auto x = elem->FirstChildElement();
        
        type = x->FirstChild()->FirstChild()->ValueStr();
    }
};
class Edge{
    public:
    int from, to; 
    Edge *next;
    EdgeAttribute attr;
    int id;
    Edge(int from, int to, Edge* next, int id, EdgeAttribute attr):
        from(from), to(to), next(next), attr(attr), id(id){};
};
class Node{
    public:
    int id; 
    NodeAttribute attr;
    Edge *edges;
    Node(int id, NodeAttribute attr): id(id), attr(attr), edges(0){}
    ~Node(){
        Edge *iter = edges;
        while (iter) {
            Edge *tmp = iter;
            iter = iter->next;
            delete tmp;
        }
    }
    Edge* addedge(int node_id, EdgeAttribute attr, int edge_id){
        return edges = new Edge(id, node_id, edges, edge_id, attr);
    }
    int unconnect(int to){
        Edge *iter, *prev;
        iter = prev = edges;
        int edge_id = -1;
        while (iter) {
            Edge *tmp = iter;
            iter = iter->next;
            if (tmp->to == to){
                if (tmp == edges) edges = iter;
                else prev->next = iter;
                edge_id = tmp->id;
                delete tmp;
            }else prev = tmp;
        }
        return edge_id;
    }
};
class Graph{
    public:
    vector<Node*> node;
    int node_num, edge_num;
    Graph(const char * filename){
        TiXmlDocument doc(filename);
        if (!doc.LoadFile()){
            cerr << "Error while loading file" << endl;
        }
        TiXmlHandle hdl(&doc);
        map<int,int> id_to_index;

        TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement("graph").FirstChildElement().Element();
        while (elem){
            if(strcmp(elem->Value(),"node") == 0){
                NodeAttribute label = NodeAttribute(elem);
                int id = label.id;

                id_to_index[id] = node_num;
                addnode(new Node(id,label));
            }else if (strcmp(elem->Value(),"edge") == 0){
                EdgeAttribute label = EdgeAttribute(elem);
                int from=label.from;
                int to=label.to;

                link(id_to_index[from], id_to_index[to], label);
            }
            elem = elem->NextSiblingElement(); // iteration
        }
    }
    void addnode(Node* x) {
        node.push_back(x);
        node_num ++;
    }
    void link(int x,int y, EdgeAttribute attr){
        node[x]->addedge(y, attr, edge_num++);
        node[y]->addedge(x, attr, edge_num++);
    }
    void shuffle(){
        int prem[node_num];
        random_shuffle(node.begin(), node.end());
        for (int i = 0; i<node_num; i++) prem[node[i]->id]=i;
        for (int i = 0; i<node_num; i++) {
            node[i]->id = i;
            auto iter = node[i]->edges;
            while (iter) {
                iter->from = prem[iter->from];
                iter->to = prem[iter->to];
            }
        }
    }
};

#endif
