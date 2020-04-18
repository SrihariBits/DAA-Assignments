/**
 * \file FordFulkerson.cpp
 * \brief This file contains the implementation for Ford fulkerson's algorithm for maximum network flow
 * 
 * 
 */

#include<bits/stdc++.h>
#include "FordFulkerson.h"
using namespace std;
vector<int> partA,partB;
unordered_map<int,bool> visited;
/**
 * \brief does dfs to find the all the nodes which are reachable from source
 * \param residual Graph after Ford Fulkerson Algorith is completed, current vertex index at which dfs is running
 */

void minCut(Graph& residual,int current)
{
    cout<<current<<endl;
    visited[current]=true;
    vector<pair<int,int>> forwards=residual.vertex[current].capacity;
    vector<pair<int,int>> backwards=residual.vertex[current].flow;
    for(int i=0;i<forwards.size();i++)
    {
        if(!visited[forwards[i].first] && forwards[i].second>0)	// if there is a forward edge from current vertex and it is not visited previously
            minCut(residual,forwards[i].first);					// then take path along that vertex
    }

    for(int i=0;i<backwards.size();i++)
    {
        if(!visited[backwards[i].first] && backwards[i].second>0)	// if there is a backward edge from current vertex and it is not visited previously
            minCut(residual,backwards[i].first);					// then take path along that vertex
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
        for(int i=0;i<v_count;i++)
            visited[i]=false;
        minCut(residual,src);
        cout<<"minCut function exited"<<endl;
        for(int i=0;i<v_count;i++)	//this is for getting the original vertices from the indices we have in 'visited' 
        {							//and also partitioning them into partA and partB
            auto it=Map.begin();
            while(it!=Map.end())
            {
                if(it->second==i)
                {
                    if(visited[i]==true)
                        partA.push_back(it->first);	//vertices which are visited in dfs from src will be in partA
                    else
                        partB.push_back(it->first);	//vertices which are not visited in dfs from src will be in partB
                }
                it++;
            }
        }
        cout<<"mincut partA"<<endl;
        for(int x:partA)
            cout<<x<<" ";
        cout<<endl;
        cout<<"mincut partB"<<endl;
        for(int x:partB)
            cout<<x<<" ";
        cout<<endl;
		int max_flow=0;
		for(auto v:original.vertex[src].flow) // Sum all outgoing edges from source in final graph for max_flow
		{
			cout<<v.second<<" ";
			max_flow+=v.second;
		}
		cout<<"\nMAX FLOW :"<<max_flow;
    }
    return 0;
}

