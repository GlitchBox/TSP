#include<bits/stdc++.h>
#define inf INT_MAX
using namespace std;

typedef pair<int, vector<int> > pair_int_arr;
typedef pair<int, pair_int_arr> pair_int_pair;

// int graph[1000][1000];
vector<vector<int> > graph(1000);

int rowReduction(int costMatrix[][1000], int nodeNo){

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

int colReduction(int costMatrix[][1000], int nodeNo){

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

int childCost(int parentNode, int parentCost, vector<vector<int> > oldCost , int childNode, priority_queue<pair_int_pair, vector<pair_int_pair>, greater<pair_int_pair> >&pq, int nodeNo){

	int newCost[1000][1000];
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
	pq.push(make_pair(cost, make_pair(childNode, newCost)));

	return cost;
}

int main(){

	priority_queue<pair_int_pair, vector<pair_int_pair>, greater<pair_int_pair> > activeNodes;
	for(int i=0;i<1000;i++)
		for(int j=0;j<1000;j++)
			graph[i][j] = inf;
	
	//take in the inputs
	ifstream myfile("input.txt");
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

	// for(int i=0;i<nodeNo;i++){
	// 	for(int j=0;j<nodeNo;j++)
	// 		cout<<graph[i][j]<<" ";
	// 	cout<<endl;
	// }
	// cout<<endl;
	
	int rootCost = rowReduction(graph, nodeNo);
	// graph[0][nodeNo-1] = 2;
	// for(int i=0;i<nodeNo;i++){
	// 	for(int j=0;j<nodeNo;j++)
	// 		cout<<graph[i][j]<<" ";
	// 	cout<<endl;
	// }
	// cout<<endl;

	rootCost += colReduction(graph, nodeNo);
	// for(int i=0;i<nodeNo;i++){
	// 	for(int j=0;j<nodeNo;j++)
	// 		cout<<graph[i][j]<<" ";
	// 	cout<<endl;
	// }
	activeNodes.push(make_pair(rootCost, make_pair(0, graph)));
	int upperBound = inf;
	

	while(!activeNodes.empty()){

		pair_int_pair topNode = activeNodes.top();
		activeNodes.pop();
		int topCost = topNode.first;
		int currentNode = topNode.second.first;
		cout<<"topCost,currentNode: "<<topCost<<","<<currentNode<<endl;

		//if topNode is a leaf node
		bool ifLeaf = false;
		for(int i=0;i<nodeNo;i++){

			if(i!=currentNode && topNode.second.second[currentNode][i]!=inf){
				if(!ifLeaf)
					ifLeaf = true;

				int childcost = childCost(currentNode, topCost, topNode.second.second, i, activeNodes, nodeNo);
				// activeNodes.push(make_pair(childcost, make_pair(i,newCost)));
				// cout<<"children: "<<endl;
				// for(int j=0;j<nodeNo;j++){
				// 	for(int k=0;k<nodeNo;k++){
				// 		if(topNode.second.second[j][k] == inf)
				// 			cout<<"inf ";
				// 		else
				// 			cout<<topNode.second.second[j][k]<<" ";
				// 	}
				// 	cout<<endl;
				// }
				// cout<<endl;
				// cout<<"childcost, i: "<<childcost<<","<<i<<endl;
				// for(int j=0;j<nodeNo;j++){
				// 		for(int k=0;k<nodeNo;k++){
				// 		if(newCost[j][k] == inf)
				// 			cout<<"inf ";
				// 		else
				// 			cout<<newCost[j][k]<<" ";
				// 	}
				// 		cout<<endl;
				// }
				// cout<<endl;
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

// void func1(int z, priority_queue<pair<int, int(*)[1000]> >&pq){

// 	int x[1000][1000];
// 	for(int i=0;i<10;i++)
// 		for(int j=0;j<10;j++)
// 			x[i][j] = z*100;
// 	pq.push(make_pair(2,x));
// }

// void func2(int z, priority_queue<pair<int, vector<vector<int> > > >&pq){

// 	vector< vector<int> > x(10);
// 	for(int i=0;i<10;i++){

// 		x[i] = vector<int>(10);
// 	}

// 	for(int i=0;i<10;i++)
// 		for(int j=0;j<10;j++)
// 			x[i][j] = z*200;
// 	pq.push(make_pair(1,x));
// }

// int main(){

// 	priority_queue<pair<int, vector<vector<int> > > > pq;
// 	for(int i=1;i<=2;i++)
// 		func2(i, pq);

// 	while(!pq.empty()){
// 		pair<int, vector<vector<int> > > topNode = pq.top();
// 		for(int i=0;i<10;i++){
// 			for(int j=0;j<10;j++)
// 				cout<<topNode.second[i][j]<<" ";
// 			cout<<endl;
// 		}
// 		cout<<endl;
// 		pq.pop();
// 	}

// }