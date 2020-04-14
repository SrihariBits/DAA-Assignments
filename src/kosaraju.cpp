/**
 * \file kosaraju.cpp
 * \brief This file contains the implementation for kosaraju's algorithm
 * 
 * A strongly connected component (SCC) of a directed graph is a maximal strongly connected subgraph.
 * We can find all strongly connected components in O(V+E) time using Kosaraju’s algorithm.
 */

#include<bits/stdc++.h>

using namespace std;

/**
 * \struct Node
 * \brief Store the edge lists of the input graphs
 */
struct node
{
    vector<unsigned long long> original; /**< Actual edge list for this node*/
	vector<unsigned long long> transpose; /**< Reversed edge list for this node*/
};

unsigned long long  v_count, /**< Vertex count */
                    e_count; /**< Edge count */

map<unsigned long long,unsigned long long> Map; /**< Map of vertex to its index */
/**
 * \class Graph
 * \brief Class to represent the input graph
 */
class Graph
{
	public:
		vector<node> graph;     /**< Struct of edge lists with max 10^7 nodes */
		vector<bool> visited;   /**< Visited array for dfs */
		stack<unsigned long long> Stack;    /**< Stack to keep track for nodes */
		vector<vector<unsigned long long>> components; /**< List of detected components */
		unsigned long long no_of_components;/**< No of components found */
		
        /*
         * \brief Constructor for graph class
         * \param n Number of nodes in graph
         */
		Graph(int n)
		{
			graph.resize(n);
			visited.resize(n,false);
			components.resize(n);
			no_of_components=0;
		}
		
        /*
         * \brief perform plain DFS(depth-first-search) (recursive) and push elements into stack
         * \param v vertex to perform dfs call
         */
		void DFS_1(long long v)
		{
			visited[v]=true;
		    for(unsigned long long i=0;i<graph[v].original.size();++i)
		    {
		        if (!visited[graph[v].original[i]]) DFS_1(graph[v].original[i]);
		    }
		    Stack.push(v);
		}

        /*
         * \brief perform DFS on the transpose graph and find connected components
         * \param v vertex to perform dfs call
         */
		void DFS_2(long long v)
		{
			visited[v] = true;
		    components[no_of_components].push_back(v);//current component is components[noc]
		    for(unsigned long long i=0;i<graph[v].transpose.size();++i)
		    {
		        if(!visited[graph[v].transpose[i]]) DFS_2(graph[v].transpose[i]);
		    }
		}
};

// 325729 1497134
// 875713 5105039
// 1632803 30622564
// 4038 88235
/**
 * \brief main function to start the algorithm, takes file path as command line argument
 * \param argc no of command line arguments
 * \param argv list of command line arguments
 */
int main(int argc, char *argv[])
{
	//ios::sync_with_stdio(false);
	//cin.tie(NULL);
	//cout.tie(NULL);
	cout << "HELLO\n";
	cin>>v_count>>e_count; //should check file and enter if file handling (max. v number, no of entries)
	Graph G(v_count);

	unsigned long long index=-1;
	ofstream myfile1("./../gephi/kosaraju_edge.csv");
	if(myfile1.is_open())
	{
		myfile1 << "Source; Target; Type\n";
	}
	//file handling -- must be a list of edges, 2 vertices per line
    if(argc == 2)
    {
        ifstream fin;
        fin.open(argv[1]);
        string line;
        stringstream ss;
        unsigned long long vertex_a,vertex_b;
        while (fin) {
            getline(fin, line);
            ss << line <<"\n";
            ss >> vertex_a >> vertex_b;
			if(Map.find(vertex_a)==Map.end())
			{
				Map[vertex_a]=++index;
			}
			if(Map.find(vertex_b)==Map.end())
			{
				Map[vertex_b]=++index;
			}
            G.graph[Map[vertex_a]].original.push_back(Map[vertex_b]);
            G.graph[Map[vertex_b]].transpose.push_back(Map[vertex_a]);
			if(myfile1.is_open())
			{
				myfile1 << Map[vertex_a] << ";" << Map[vertex_b] <<"; directed\n";
			}
        }
		myfile1.close();
        fin.close();
    }
	// input from terminal if file is not given in arguments
    else
    {
        for(unsigned long long i=0;i<e_count;++i)
        {
            unsigned long long vertex_a,vertex_b;
            cin>>vertex_a>>vertex_b;
			if(Map.find(vertex_a)==Map.end())
			{
				Map[vertex_a]=++index;
			}
			if(Map.find(vertex_b)==Map.end())
			{
				Map[vertex_b]=index++;
			}
            G.graph[Map[vertex_a]].original.push_back(Map[vertex_b]);
            G.graph[Map[vertex_b]].transpose.push_back(Map[vertex_a]);
			if(myfile1.is_open())
			{
				myfile1 << Map[vertex_a] << ";" << Map[vertex_b] <<"; directed\n";
			}
        }
		myfile1.close();
    }
	// first dfs for kosaraju
	for(unsigned long long i=0;i<v_count;++i)
	{
		if(!G.visited[i])
		{
			G.DFS_1(i);
		}
	}
	// re-init the visited array to false
	for(unsigned long long i=0;i<v_count;++i)
	{
		G.visited[i]=false;
	}
	// second dfs for inverse graph
	while(!G.Stack.empty())
	{
		unsigned long long v=G.Stack.top();
		G.Stack.pop();
		if(!G.visited[v])
		{
			G.DFS_2(v);
			++G.no_of_components;
		}
	}
	ofstream outFile; // output file to store components (Nodes)
	system("pause");
	cout<<"Number of components: "<<G.no_of_components<<"\n";
	long long timestamp=0;
	// start printing each component to shell & file
	for(unsigned long long i=0;i<G.no_of_components;++i)
	{
		if(G.components[i].size()>=1) //-----------------------
		{
			cout<<"Component no. "<<i+1<<"\n";
			if(timestamp%100==0)
			{
				outFile.open("./../gephi/kosaraju_node"+to_string(timestamp/100)+".csv");
				outFile<< "Id; intervalset\n";
			}
			for(auto itr:G.components[i])
			{
				for(auto it:Map)
				{
					if(it.second==itr)
					{
						cout<<it.first<<" ";
						if (outFile.is_open())
						{
							outFile << itr << ";" << timestamp <<"\n";
						}
						break;
					}
				}
			}
			if(timestamp%100==99)
				outFile.close();
			++timestamp;
			cout<<"\n";
		}
	}
    return 0;
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
 * <b> PRED </b>:  Predecessors - set of nodes through which we can reach the current node <br>
 * <b> DESC </b>:  Descendants - set of nodes we can reach starting from the current node <br>
 *
 * \subsection Input
 * 
 * Input is given as follows:<br>
 *  - input can be given through file as command line argument
 *  - if command line args is not given, input can be given through stdin/console
 * 
 * \subsection Output
 *  - all the strongly connected components are printed in a single line
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
 *		-
 * 
 * <hr>
 * 
 */
