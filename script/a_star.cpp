#include<iostream>
#include<queue>
using namespace std;

//using A-star algorithm to find a distance


class GraphChange{
	int cost;
	GraphChange(int cost_):cost(cost_){}
}
class NodeChange:public GraphChange{
	int from, to;
	NodeChange(int from_, int to_, int cost_):GraphChange(cost_),from(from_), to(to_){}
}

class EdgeChange:public GraphChange{
	int from_start, from_end, to_start, to_end;
	EdgeChange(int from_start_, int to_start_, int to_start_, int to_end_ ,int cost_):GraphChange(cost_),
	from_start(from_start_),from_end(from_end_), to_start(to_start_), to_end(to_end_){}
}

typedef pair< vector<GraphChange*>*, int> opts;

class myless {
	public:
	bool operator()(opts1,opts2) {
		return opts1.second<opts2.second;
	}
}

int a_star (){
	bool found = false;
	int result = 0;//graph edit distance;
	priority_queue<opts, vector<opts>, myless> option_set;//a prioity queue that save all the options
	
	//initialize heap
	option_set.push();
	for (i=1;i<= ;++i)
		option_set.push();
	
	//find the smallest distance
	while(!found) { 
		= option_set.top();
		option_set.pop();
		if judgesame(){
			found=true;
			result= ;
		}
		else{
			if( < ) {//the final graph is smaller than the operating graph
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
