#include<iostream>
#include<queue>
#include<vector>
using namespace std;

//using A-star algorithm to find a distance


class Nodechange{
	public:
	int cost;
	int from, to;
	NodeChange(int from_, int to_, int cost_):NodeChange(cost_),from(from_), to(to_){}
}
class opts{
	public:
	Nodechange* changelist;
	ínt numChange;
	int costs;
	int imagecost;
	int totalcost;
	opts(){}
	~opts(){
		delete Nodechange*;
	}
}

class myless {
	public:
	bool operator()(opts1,opts2) {
		return opts1.totalcost<opts2.totalcost;
	}
}

int a_star (Graph* g1, Graph* g2){
	bool found = false;
	int result = 0;//graph edit distance;
	priority_queue<opts, vector<opts>, myless> option_set;//a prioity queue that save all the options
	
	//initialize heap
	option_set.push();
	for (i=1;i<= ;++i)
		option_set.push();
	
	//find the smallest distance
	while(!found) { 
		opts bestopts= option_set.top();
		option_set.pop();
		if (bestopts.numChange=g1.numNode){
			found=true;
			result=bestopts.second;
		}
		else {
			opts newopts;
			if(newopts.numChange<g1.numNode ) {//the final graph is smaller than the operating graph
				for()
					for()
						if()
							option_set.push();
				
				option_set.push();
			}
			else {//the final graph has more points than the operating graph
				for()
					option_set.push();
			}
		}
	}
	return result;
}
