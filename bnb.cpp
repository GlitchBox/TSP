#include<bits/stdc++.h>
#define inf INT_MAX
using namespace std;

typedef pair<int, vector<vector<int> > > pair_int_arr;
typedef pair<int, pair_int_arr> pair_int_pair;

vector<vector<int> > graph(1000);

int rowReduction(vector<vector<int> > &costMatrix, int nodeNo){

	int totalReduction = 0;
	for(int i=0;i<nodeNo;i++){

		int minCost = costMatrix[i][0];
		for(int j=1;j<nodeNo;j++){
			if(minCost>costMatrix[i][j])
				minCost = costMatrix[i][j];
		}

		if(minCost>0 && minCost<inf){
			
			totalReduction += minCost;
			for(int j=0;j<nodeNo;j++){
				if(costMatrix[i][j]<inf)
					costMatrix[i][j] -= minCost;
			}
		}

	}

	return totalReduction;
}

int colReduction(vector<vector<int> > &costMatrix, int nodeNo){

	int totalReduction = 0;
	for(int j=0;j<nodeNo;j++){

		int minCost = costMatrix[0][j];
		for(int i=1;i<nodeNo;i++){
			if(costMatrix[i][j]<minCost)
				minCost = costMatrix[i][j];
		}

		if(minCost>0 && minCost<inf){

			totalReduction += minCost;
			for(int i=0;i<nodeNo;i++){
				if(costMatrix[i][j]<inf)
					costMatrix[i][j] -= minCost;
			}
		}
	}

	return totalReduction;
}

int childCost(int parentNode, int parentCost, vector<vector<int> > &oldCost , int childNode, vector<vector<int> > &newCost, int nodeNo){

	int cost = oldCost[parentNode][childNode] + parentCost;
	for(int i=0;i<nodeNo;i++){
		for(int j=0;j<nodeNo;j++)
			newCost[i][j] = oldCost[i][j];
	}

	//inf-ing row of the parent
	for(int j=0;j<nodeNo;j++)
		newCost[parentNode][j] = inf;

	//inf-ing col of the child
	for(int i=0;i<nodeNo;i++)
		newCost[i][childNode] = inf;

	//inf the edge from child to 0(1st node)
	newCost[childNode][0] = inf;

	cost += rowReduction(newCost, nodeNo);
	cost += colReduction(newCost, nodeNo);

	return cost;

}

int main(){

	for(int i=0;i<1000;i++)
		graph[i] = vector<int>(1000);
	for(int i=0;i<1000;i++)
		for(int j=0;j<1000;j++)
			graph[i][j] = inf;
	priority_queue<pair_int_pair, vector<pair_int_pair>, greater<pair_int_pair> > activeNodes;	
	
	//take in the inputs
	ifstream myfile("input1.txt");
	string line;
	int nodeNo;

	getline(myfile, line);
	istringstream is(line);
	is>>nodeNo;

	while(!myfile.eof()){
		getline(myfile, line);
		int i,j,cost;
		istringstream is1(line);
		is1>>i>>j>>cost;

		graph[i][j] = cost;
	}
	myfile.close();
	
	int rootCost = rowReduction(graph, nodeNo);
	rootCost += colReduction(graph, nodeNo);

	activeNodes.push(make_pair(rootCost, make_pair(0, graph)));
	int upperBound = inf;
	

	while(!activeNodes.empty()){

		pair_int_pair topNode = activeNodes.top();
		activeNodes.pop();
		int topCost = topNode.first;
		int currentNode = topNode.second.first;

		//if topNode is a leaf node
		bool ifLeaf = false;
		for(int i=0;i<nodeNo;i++){

			if(i!=currentNode && topNode.second.second[currentNode][i]!=inf){
				if(!ifLeaf)
					ifLeaf = true;

				vector<vector<int> > newCost(1000);
				for(int j=0;j<1000;j++)
					newCost[j] = vector<int>(1000);

				int childcost = childCost(currentNode, topCost, topNode.second.second, i, newCost, nodeNo);
				activeNodes.push(make_pair(childcost, make_pair(i,newCost)));
			}
		}

		//if we reached a leaf node
		if(!ifLeaf){

			if(topCost<upperBound)
				upperBound = topCost;

			while(!activeNodes.empty()){
				if(activeNodes.top().first < upperBound)
					break;
				activeNodes.pop();
			}
		}

	}
	cout<<upperBound<<endl;

}
