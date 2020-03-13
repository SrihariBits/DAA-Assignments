/**
 * \file dcsc.cpp
 * \brief Implementation of the dcsc algorithm
 * 
 * This file contains the code for the DCSC algorithm specified in the paper.
 * Complexity: O(nlogn)
 */

#include<bits/stdc++.h>

using namespace std;

int v_count; /**< Vertices count */
int e_count; /**< Edges count */
map<unsigned long long,unsigned long long> Map; /**< Map of vertex to its index */
/**
 * \class Graph
 * \brief This class contains the input graph
 *
 * This class is can be used to store both the original graph and the transpose graph
 */
class Graph
{
    public:
		vector<vector<int>> original;	/**< Vector representing the original connected nodes of graph */
		vector<vector<int>> transpose;	/**< Vector representing connected nodes of transpose graph */
		vector<bool> visited;	/**< Visited array to keep track of traversed nodes */
		int no_of_v;	/**< Number of vertices in graph */
		vector<bool> valid;

		/**
		 * \brief Constructor for the graph class
		 * \param n Number of nodes in graph
		 */
		Graph(int n)
		{
			no_of_v=n;
			original.resize(n);
			transpose.resize(n);
			valid.resize(n,false);
		}

		/**
		 * \brief dfs(depth-first-search) Function to traverse the graph recursively
		 * \param v Node/vertex number to traverse from
		 * \param edgeList Edge list of graph
		 * \param Set Set of connected nodes (PRED or DESC)
		 */
		void dfs(int v,vector<vector<int>>& edgeList,set<int>& Set)
		{
			//To find decendants (pred is desc of transpose graph)
			//Set is what we pass, PRED or DESC
			visited[v]=false;
			Set.insert(v);
			for(int i=0;i<edgeList[v].size();++i)
		    {
		        if(visited[edgeList[v][i]]) dfs(edgeList[v][i],edgeList,Set);
		    }
		}
};

/**
 * \brief DCSC(Divide and Conquer Strong Components) Function to find the strongly connected components using a divide and conquer approach
 * \param G Object of Graph class
 *
 * This function first selects a random vertex and perform a dfs to find PRED and DESC set of the selected node. Then it recurses 
 * on each of PRED, DESC and REM components. It outputs the SCCs in corresponding set.
 */
void DCSC(Graph G)
{
	bool flag;
	for(int i=0;i<v_count;++i)
	{
		if(G.valid[i])
		{
			flag=true;
			for(auto t:G.original[i])
			{
				if(G.valid[t])
				{
					flag=false;
					break;
				}
			}
			if(!flag)
			{
				break;
			}
		}
	}
	if(flag)
	{
		for(int i=0;i<G.valid.size();++i)
		{
			if(G.valid[i])
				for(auto it:Map)
				{
					if(it.second==i)
					{
						cout<<it.first<<"\n";
						break;
					}
				}
		}
		return;
	}
	
	srand(time(0));
	unsigned long long k;
	if(G.no_of_v==0) k=0;
	else
	k=rand()%G.no_of_v;
	unsigned long long v=0;
	while(k)
	{
		while(!G.valid[v])
		{
			++v;
		}
		++v;
		--k;
	}
	
	//Choosing first available vertex instead of random
	while(!G.valid[v])
	{
		++v;
		if(v==v_count) return;
	}
	
	set<int> Pred;
	G.visited=G.valid;
	G.dfs(v,G.original,Pred);
	
	set<int> Desc;
	G.visited=G.valid;
	G.dfs(v,G.transpose,Desc);
	
	set<int> SCC;
	set_intersection(Pred.begin(),Pred.end(),Desc.begin(),Desc.end(),inserter(SCC,SCC.begin()));
	for(auto v:SCC)
	{
		for(auto it:Map)
		{
			if(it.second==v)
			{
				cout<<it.first<<" ";
				break;
			}
		}
	}
	if(!SCC.empty()) cout<<"\n";
	
	set<int> REM;
	//following lines does G-Pred-Desc = G - (Pred U Desc)
	for(auto v:Pred)
	{
		G.valid[v]=false;
	}
	for(auto v:Desc)
	{
		G.valid[v]=false;
	}
	for(int i=0;i<G.valid.size();++i)
	{
		if(G.valid[i]==true)
		{
			REM.insert(i);
		}
	}
	
	set<int> P_SCC;//Pred\SCC
	set_difference(Pred.begin(),Pred.end(),SCC.begin(),SCC.end(),inserter(P_SCC,P_SCC.begin()));

	set<int> D_SCC;//Desc\SCC
	set_difference(Desc.begin(),Desc.end(),SCC.begin(),SCC.end(),inserter(D_SCC,D_SCC.begin()));
	
	//following lines create modified graph (by changing valid nodes) for Pred\SCC and calls DCSC
	for(int i=0;i<G.valid.size();++i)
	{
		G.valid[i]=false;//reset validity
	}
	G.no_of_v=0;
	for(auto v:P_SCC)
	{
		G.valid[v]=true;
		++G.no_of_v;
	}
	DCSC(G);
	
	//following lines create modified graph (by changing valid nodes) for Desc\SCC and calls DCSC
	for(int i=0;i<G.valid.size();++i)
	{
		G.valid[i]=false;//reset validity
	}
	G.no_of_v=0;
	for(auto v:D_SCC)
	{
		G.valid[v]=true;
		++G.no_of_v;
	}
	DCSC(G);
	
	//following lines create modified graph (by changing valid nodes) for REM and calls DCSC
	for(int i=0;i<G.valid.size();++i)
	{
		G.valid[i]=false;//reset validity
	}
	G.no_of_v=0;
	for(auto v:REM)
	{
		G.valid[v]=true;
		++G.no_of_v;
	}
	DCSC(G);
}

/**
 * \brief main function to start the algorithm, takes file path as command line argument
 * \param argc no of command line arguments
 * \param argv list of command line arguments
 */
int main(int argc, char *argv[])
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	cin>>v_count>>e_count;
	Graph G(v_count);
	unsigned long long index=0;

	//File handling -- Doesnt work for this file (too huge?)
	if(argc == 2)
    {
        ifstream fin;
        string line;
        fin.open(argv[1]);
        stringstream ss;
            int vertex_a,vertex_b;
        while (fin) {
            getline(fin, line);
			ss << line <<"\n";
            ss >> vertex_a >> vertex_b;
			if(Map.find(vertex_a)==Map.end())
			{
				Map[vertex_a]=index++;
			}
			if(Map.find(vertex_b)==Map.end())
			{
				Map[vertex_b]=index++;
			}
            G.original[Map[vertex_a]].push_back(Map[vertex_b]);
            G.transpose[Map[vertex_b]].push_back(Map[vertex_a]);
            G.valid[Map[vertex_a]]=true;
            G.valid[Map[vertex_b]]=true;
        } 
        fin.close();
    }
    else
    {
        for(int i=0;i<e_count;++i)
        {
            int vertex_a,vertex_b;
            cin>>vertex_a>>vertex_b;
			if(Map.find(vertex_a)==Map.end())
			{
				Map[vertex_a]=index++;
			}
			if(Map.find(vertex_b)==Map.end())
			{
				Map[vertex_b]=index++;
			}
            G.original[Map[vertex_a]].push_back(Map[vertex_b]);
            G.transpose[Map[vertex_b]].push_back(Map[vertex_a]);
            G.valid[Map[vertex_a]]=true;
            G.valid[Map[vertex_b]]=true;
        }
    }

    // start of DCSC algo
	DCSC(G);
}