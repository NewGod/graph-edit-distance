#include<iostream>
#include<queue>
using namespace std;

//using A-star algorithm to find a distance

int a_star (){
	bool found = false;
	int result = 0;//graph edit distance;
	priority_queue<> option_set;//a prioity queue that save all the options
	
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
