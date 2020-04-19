/**
 * \file kosaraju.cpp
 * \brief This file contains the implementation for kosaraju's algorithm
 * 
 * A strongly connected component (SCC) of a directed graph is a maximal strongly connected subgraph.
 * We can find all strongly connected components in O(V+E) time using Kosaraju’s algorithm.
 */

#include <bits/stdc++.h>

using namespace std;

/**
 * \struct node
 * \brief store the edge lists of the input graphs
 */
struct node {
    vector<unsigned long long> original; /**< actual edge list for this node*/
	vector<unsigned long long> transpose; /**< reversed edge list for this node*/
};

unsigned long long	v_count, /**< Vertex count */
					e_count; /**< Edge count */

node graph[10000005];/*< struct of edge lists with max 10^7 nodes */
vector<bool> visited;/*< for dfs */
stack<unsigned long long> Stack;
vector<unsigned long long> components[10000005];
unsigned long long no_of_components=0;

/*
 * \brief perform plain DFS(depth-first-search) (recursive) and push elements into stack
 * \param v vertex to perform dfs call
 */
void DFS_1(long long v) {
	visited[v]=true;
    for(unsigned long long i=0;i<graph[v].original.size();++i) {
        if (!visited[graph[v].original[i]])
			DFS_1(graph[v].original[i]);
    }
    Stack.push(v);
}

/*
 * \brief perform DFS on the transpose graph
 * \param v vertex to perform dfs call
 */
void DFS_2(long long v) {
	visited[v] = true;
    components[no_of_components].push_back(v);//current component is components[noc]
    for(unsigned long long i=0;i<graph[v].transpose.size();++i) {
        if(!visited[graph[v].transpose[i]])
			DFS_2(graph[v].transpose[i]);
    }
}

/*
 * \brief main function to start the algorithm, takes file path as command line argument
 * \param argc no of command line arguments
 * \param argv list of command line arguments
 */
int main(int argc, char *argv[]) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	cin>>v_count>>e_count; // should check file and enter if file handling (max. v number, no of entries)
	visited.resize(v_count,false);
	
	// file handling -- must be a list of edges, 2 vertices per line
	if (argc == 2) {
		ifstream fin;
		string line;
		fin.open(argv[1]);
		while (fin) {
			string a,b; 
			getline(fin, line);
			int i;
			for(i=0;line[i]!=' ';++i) {
				a+=line[i];
			}
			for(++i;i<line.length();++i) {
				b+=line[i];
			}
			unsigned long long vertex_a,vertex_b;
			vertex_a=stoi(a);
			vertex_b=stoi(b);
			graph[vertex_a].original.push_back(vertex_b);
			graph[vertex_b].transpose.push_back(vertex_a);
		} 
		fin.close();
	}
	else {
		for(unsigned long long i=0;i<e_count;++i) {
			unsigned long long vertex_a,vertex_b;
			cin>>vertex_a>>vertex_b;
			graph[vertex_a].original.push_back(vertex_b);
			graph[vertex_b].transpose.push_back(vertex_a);
		}
	}
	
	for(unsigned long long i=0;i<v_count;++i) {
		if(!visited[i]) {
			DFS_1(i);
		}
	}
	for(unsigned long long i=0;i<v_count;++i) {
		visited[i]=false;
	}
	while(!Stack.empty()) {
		unsigned long long v=Stack.top();
		Stack.pop();
		if(!visited[v]) {
			DFS_2(v);
			++no_of_components;
		}
	}
	cout<<"Number of components: "<<no_of_components<<"\n";
	for(unsigned long long i=0;i<no_of_components;++i) {
		cout<<"Component no. "<<i+1<<"\n";
		for(auto itr:components[i]) {
			cout<<itr<<" ";
		}
		cout<<"\n";
	}
}

//      THIS IS THE MAIN PAGE OF DOCUMENTATION.
/**
 * \mainpage
 * 
 * \section Project-Details
 * This Project deals with finding out connected components from the input data-graphs. <br>
 * <b>How it works</b><br>
 * The first algorithm is kosaraju's algorithm which performs 2 graph traversals: <br>
 * Step 1: Perform direct dfs from any node and push node to stack <em>after<em> dfs finishes on that node <br>
 * Step 2: Perform dfs node present on top of stack, then pop it. each dfs results in a Strongly Connected Component(SCC) <br>
 * 
 * *********************************** 
 * compile with <em>g++ *.cpp</em>
 * <br>
 * <a href="../Performance_Documentation/index.html"> >>>Link to performance report<<< </a>
 * <hr>
 * 
 * \subsection Definitions
 * <b> SCC </b>:  Strongly Connected Component <br>
 * <b> DCSC </b>:  Divide and Conquer Strong Components <br>
 *
 * \subsection Input
 * 
 * Input is given as follows:<br>
 * 
 * 
 * \subsection Output
 * 
 * 
 * \subsection Assumptions
 *
 * 
 * \subsection Limitations
 *
 * 
 * <hr>
 * \subsection Team
 * This project is done by:
 *      - L Srihari (2017A7PS1670H)
 *      - Rikil Gajarla (2017A7PS0202H)
 *      - Naga Sai Bharath (2017A7PS0209H)
 *		- Raj Kashyap Mallala (2017A7PS0025H)
 * 
 * <hr>
 * 
 */