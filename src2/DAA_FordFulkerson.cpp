/**
 * \file FordFulkerson.cpp
 * \brief This file contains the implementation for Ford fulkerson's algorithm for maximum network flow
 * 
 * 
 */

#include<bits/stdc++.h>
using namespace std;
#define ROOT INT_MIN
#define UNVISITED INT_MAX

/**
 * \struct Node
 * \brief Store the edge lists of the input graphs
 *	This struct stores the edge list of a node/vertex in the graph as a vector of pairs of integers. Each vertex has
 *	two types of edges namely capacity and flow. For original graph these vectors mean what the name suggests.
 *	For the residual graph, capacity represents residual capacity of the forward edge while flow represents the
 *	residual capacity of the reverse edge (not a part of original graph).
 *	Note that residual graph has no flow term hence this optimal use is encouraged.
 *	Of the pair of integers, (capacity/flow).first represents the other vertex which connects to this edge and
 *	(capacity/flow).second represents the value of (capacity/flow) passing through that edge.
 */
struct node
{
	vector<pair<int,int>> capacity;		//stores capacity in original and forward edges in residual
	vector<pair<int,int>> flow;			//stores actual flow in original and backward edges in residual
};


unsigned long long  v_count, /**< Vertex count */
                    e_count; /**< Edge count */

map<unsigned long long,unsigned long long> Map; /**< Map of vertex to its index */
/**
 * \class Graph
 * \brief Class to represent the input graph
 *	Since this is a rip off code of DAA assignment 1,this was retained. Might have as well been a struct.
 *	Will try to add outside functions into this class if I have time.
 */
class Graph
{
	public:
		/*	Each element of this vector vertex has the vector of nodes connected to it as defined by struct node.
		*	In usage it will mostly have vertex[i] kind of form denoting i'th vertex of the respective graph
		*/
		vector<node> vertex;
        /*
         * \brief Constructor for graph class
         * \param n Number of nodes in graph
         *	n vertices are initilaized in the graph
         */
		Graph(int n)
		{
			vertex.resize(n);
		}			
};

/*
 * \brief Initializes given vector with INT_MAX
 * \param parent vector of parent vertices and edge weights
 */
void init(vector<pair<int,int>>& parent)
{
	for(int i=0;i<v_count;++i)
	{
		parent[i].first=UNVISITED;
	}
}

/*
 * \brief perform plain BFS(breadth-first-search) (iterative) and push elements into queue
 * \param v vertex to perform dfs call
 *	This function looks for an S-T path in residual graph. It also stores the parent of all the nodes visited along
 *	with the edge weights in order to enable ease of finding bottleneck edge weight while backtracking once
 *	destination is reached while searching(BFS)
 */
int BFS(vector<pair<int,int>>&parent, Graph& residual, int src, int dest)
{
	int v;
	queue<int> q;
	init(parent);
	q.push(src);
	parent[src]={ROOT,INT_MAX};
	while(!q.empty())
	{
		v=q.front();
		q.pop();
		
		if(v == dest)
		{
			/* If destination is reached, backtrack and check if parent to current node's
			*  residual capacity (stored in parent[child].second is the bottleneck value of S-T path.
			*/
			int i=dest;
			int bottleneck=INT_MAX;
			while(parent[i].first!=ROOT)
			{
				if(abs(parent[i].second)<bottleneck)
				{
					bottleneck=abs(parent[i].second);
				}
				i=parent[i].first;
			}
			return bottleneck;
		}
		/* Since residual capacities of forward edges are stored in capacity vector and that of backward edges are
		*  stored in flow, we separately look for next edges in these 2 vectors in BFS.
		*  Further, note that flow (i.e backward edge) will be chosen only if no forward edge is available with
		*  positive capacity. To differentiate backward edge from forward in parent vector, parent[child].second
		*  is made negative for backward edges and positive for forward edges. Hence we use abs() function wherever necessary.
		*/
		for(auto w:residual.vertex[v].capacity) //searching for a forward edge from current node to an unvisited node
		{
			if(w.second!=0 and parent[w.first].first==UNVISITED) //w.second has residual capacity from v to w.first
			{
				q.push(w.first);
				parent[w.first].first=v;
				parent[w.first].second=w.second; //positive
			}
		}
		for(auto w:residual.vertex[v].flow) //searching for a backward edge from current node to an unvisited node
		{
			if(w.second!=0 and parent[w.first].first==UNVISITED)
			{
				q.push(w.first);
				parent[w.first].first=v;
				parent[w.first].second=(-w.second);//made negative.
				/* Note: it was made negative just for ease of coding in augment function, where this distinction 
				*		 is essential. The residual capcity is actually and always a positive value(non-negative).
				*/
			}
		}
	}
	return 0;
}

/*
 * \brief make Residual graph from modified original graph
 * \param original Graph and residual Graph
 *	This function checks every edge of original graph (note that it has only forward edges) and makes corresponding
 *	residual graph. Could have made changes to residual graph only in the S-T path updated recently as values change
 *	only along that path. But doing it for the whole graph does not worsen the complexity of overall algorithm.
 */
void makeResidualGraph(Graph& original, Graph& residual)
{
	for(int i=0;i<v_count;++i)
	{
		for(int j=0;j<original.vertex[i].flow.size();++j)
		{
			if(original.vertex[i].flow[j].second<=original.vertex[i].capacity[j].second)
			{
				residual.vertex[i].capacity[j].second=(original.vertex[i].capacity[j].second-original.vertex[i].flow[j].second);
			}
			if(original.vertex[i].flow[j].second>0) //find the reverse edge and update it
			{
				int neighbour=original.vertex[i].flow[j].first;
				for(int k=0;k<residual.vertex[neighbour].flow.size();++k)
				{
					if(residual.vertex[neighbour].flow[k].first==i)
					{
						residual.vertex[neighbour].flow[k].second=original.vertex[i].flow[j].second;
						break;
					}
				}
			}
		}
	}
}

/*
 * \brief run the augment routine as explained in the class (and textbook).
 * \param parent a vector for S-T path, original Graph, residual Graph, bottleneck and destination index
 *	This is where having negative edge weights comes handy. In this method bottleneck is added to forward edges 
 *	and subtracted from reverse of backward edges in original graph. Note that parent was identified in residual
 *	graph while these updates are done in original graph. This is possible because both original and residual graphs
 *  have same vertex set. So j'th neighbour of i'th vertex of original will be same as that of residual(obv weights
 *  will be different).
 */
void augment(vector<pair<int,int>>& parent, Graph& residual, Graph& original, int bottleneck, int dest)
{
	int p=dest;
	while(parent[p].first!=ROOT)
	{
		if(parent[p].second>0) //forward edge since positive weight value
		{
			for(int i=0;i<original.vertex[parent[p].first].flow.size();++i)
			{
				if(original.vertex[parent[p].first].flow[i].first==p) // note forward edge from parent[p] to p
				{
					original.vertex[parent[p].first].flow[i].second+=bottleneck;
					break;
				}
			}
		}
		else //backward edge since weight with 0 will not be chosen in BFS hence weight must be -ve
		{
			for(int i=0;i<original.vertex[p].flow.size();++i)
			{
				if(original.vertex[p].flow[i].first==parent[p].first) //note reverse of backward edge hence p to parent[p]
				{
					original.vertex[p].flow[i].second-=bottleneck;
					break;
				}
			}
		}
		p=parent[p].first;
	}
}

/*
 * \brief run the Max-Flow routine as explained in the class (and textbook).
 * \param original Graph, residual Graph, source and destination index
 *	
 */
void Max_Flow(Graph& original,Graph& residual,int src,int dest)
{
	int bottleneck;
	vector<pair<int,int>> parent(v_count);
	init(parent);
	cout<<"Max-Flow\n";
	while(bottleneck=BFS(parent,residual,src,dest))
	{
		augment(parent,residual,original,bottleneck,dest);
		/*for(int i=0;i<v_count;++i)
		{
			for(auto x:original.vertex[i].flow)
			{
				cout<<i<<","<<x.second<<","<<x.first<<" ";
			}
			cout<<"\n";
		}*/
		makeResidualGraph(original,residual);
		/*for(int i=0;i<v_count;++i)
		{
			for(auto x:residual.vertex[i].capacity)
			{
				cout<<i<<","<<x.second<<","<<x.first<<" ";
			}
			cout<<"\n";
		}
		cout<<"\n";
		for(int i=0;i<v_count;++i)
		{
			for(auto x:residual.vertex[i].flow)
			{
				cout<<i<<","<<x.second<<","<<x.first<<" ";
			}
			cout<<"\n";
		}*/
		init(parent); // initialize parent b4 every BFS call
	}
}

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
	Graph original(v_count);
	Graph residual(v_count);
	
	vector<bool>isSource(v_count,true);
	unsigned long long index=-1;
	//file handling -- must be a list of edges, 2 vertices per line
    if(argc == 2)
    {
        ifstream fin;
        fin.open(argv[1]);
        string line;
        stringstream ss;
        unsigned long long vertex_a,vertex_b,cap;
        while (fin) 
		{
            getline(fin, line);
            ss << line <<"\n";
            ss >> vertex_a >> vertex_b >>cap;
			if(Map.find(vertex_a)==Map.end())
			{
				Map[vertex_a]=++index;
			}
			if(Map.find(vertex_b)==Map.end())
			{
				Map[vertex_b]=++index;
			}
            original.vertex[Map[vertex_a]].capacity.push_back({Map[vertex_b],cap});
            original.vertex[Map[vertex_a]].flow.push_back({Map[vertex_b],0});
            
            /*Note: Everytime a forward edge is added, corresponding backward edge is initialized for possible
            *		future use. Also (i,j)th node in original will be same as (i,j)th node in residual.(not wrt weghts)
            */
            residual.vertex[Map[vertex_a]].capacity.push_back({Map[vertex_b],cap});
            residual.vertex[Map[vertex_b]].flow.push_back({Map[vertex_a],0}); //for backward edges
            
            isSource[Map[vertex_b]]=false;
        }
        fin.close();
    }
	// input from terminal if file is not given in arguments
    else
    {
        for(unsigned long long i=0;i<e_count;++i)
        {
            unsigned long long vertex_a,vertex_b,cap;
            cin>>vertex_a>>vertex_b>>cap;
			if(Map.find(vertex_a)==Map.end())
			{
				Map[vertex_a]=++index;
			}
			if(Map.find(vertex_b)==Map.end())
			{
				Map[vertex_b]=++index;
			}
            original.vertex[Map[vertex_a]].capacity.push_back({Map[vertex_b],cap});
            original.vertex[Map[vertex_a]].flow.push_back({Map[vertex_b],0});
            /*Note: Everytime a forward edge is added, corresponding backward edge is initialized for possible
            *		future use. Also (i,j)th node in original will be same as (i,j)th node in residual.(not wrt weghts)
            */
            residual.vertex[Map[vertex_a]].capacity.push_back({Map[vertex_b],cap});
            residual.vertex[Map[vertex_b]].flow.push_back({Map[vertex_a],0});//for backward edge
            
            isSource[Map[vertex_b]]=false;
        }
	}
		int src,dest;
		for(int i=0;i<v_count;++i) // Destination has no outgoing edges
		{
			if(original.vertex[i].capacity.size()==0)
			{
				dest=i;
				break;
			}
		}
		for(int i=0;i<v_count;++i) // Source has no incoming edges
		{
			if(isSource[i]==true)
			{
				src=i;
				break;
			}
		}
		
		Max_Flow(original,residual,src,dest);
		int max_flow=0;
		for(auto v:original.vertex[src].flow) // Sum all outgoing edges from source in final graph for max_flow
		{
			cout<<v.second<<" ";
			max_flow+=v.second;
		}
		cout<<"\nMAX FLOW :"<<max_flow;
    
    return 0;
}

