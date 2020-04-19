/**
 * \file bipartiteMatching.cpp
 * \brief This file contains the implementation for finding solution to the maximum bipartite matching problem
 * 
 * 
 */

#include<bits/stdc++.h>
#include "FordFulkerson.h"
using namespace std;

unsigned long long v_count1,v_count2;

map<unsigned long long,unsigned long long> Map1; /**< Map of vertex to its index */
map<unsigned long long,unsigned long long> Map2;

/**
 * \brief main function to start the algorithm, takes file path as command line argument
 * \param argc no of command line arguments
 * \param argv list of command line arguments
 */
int main(int argc, char *argv[])
{
	cout << "HELLO\n";
	cin>>v_count1>>v_count2>>e_count;	// v_count is no of vertices in part A and v_count is no of vertices in part B of the bipartitie graph. And e_count is no of edges
	v_count=v_count1+v_count2+2;		// because we are adding source and destination vertex
	Graph original(v_count);
	Graph residual(v_count);
	
	unsigned long long index=0;
	int src=0,dest=v_count-1;
	//file handling -- must be a list of edges, 2 vertices per line
    if(argc == 2)
    {
        ifstream fin;
        fin.open(argv[1]);
		cout<<"file open";
        string line;
        stringstream ss;
        unsigned long long vertex_a,vertex_b;
        while (fin) 
		{
            getline(fin, line);
            ss << line <<"\n";
            ss >> vertex_a >> vertex_b;
			if(Map1.find(vertex_a)==Map1.end())
			{
				Map1[vertex_a]=++index;
				// For every vertex in part A, there is a directed edge from source vertex to that vertex with cap 1 in original graph
				original.vertex[src].capacity.push_back({Map1[vertex_a],1});
				original.vertex[src].flow.push_back({Map1[vertex_a],0});

				residual.vertex[src].capacity.push_back({Map1[vertex_a],1});
				residual.vertex[Map1[vertex_a]].flow.push_back({src,0});
			}
			if(Map2.find(vertex_b)==Map2.end())
			{
				Map2[vertex_b]=++index;
				// For every vertex in part B, there is a directed edge from that vertex to the dest vertex with cap 1 in original graph
				original.vertex[Map2[vertex_b]].capacity.push_back({dest,1});
				original.vertex[Map2[vertex_b]].flow.push_back({dest,0});

				residual.vertex[Map2[vertex_b]].capacity.push_back({dest,1});
				residual.vertex[dest].capacity.push_back({Map2[vertex_b],0});
			}
            original.vertex[Map1[vertex_a]].capacity.push_back({Map2[vertex_b],1});
            original.vertex[Map1[vertex_a]].flow.push_back({Map2[vertex_b],0});
            
            /*Note: Everytime a forward edge is added, corresponding backward edge is initialized for possible
            *		future use. Also (i,j)th node in original will be same as (i,j)th node in residual.(not wrt weghts)
            */
            residual.vertex[Map1[vertex_a]].capacity.push_back({Map2[vertex_b],1});
            residual.vertex[Map2[vertex_b]].flow.push_back({Map1[vertex_a],0}); //for backward edges			
       }
        fin.close();
    }
	// input from terminal if file is not given in arguments
    else
    {
        for(unsigned long long i=0;i<e_count;++i)
        {
            unsigned long long vertex_a,vertex_b;
            cin>>vertex_a>>vertex_b;
			if(Map1.find(vertex_a)==Map1.end())
			{
				Map1[vertex_a]=++index;
				// For every vertex in part A, there is a directed edge from source vertex to that vertex with cap 1 in original graph
				original.vertex[src].capacity.push_back({Map1[vertex_a],1});
				original.vertex[src].flow.push_back({Map1[vertex_a],0});

				residual.vertex[src].capacity.push_back({Map1[vertex_a],1});
				residual.vertex[Map1[vertex_a]].flow.push_back({src,0});
			}
			if(Map2.find(vertex_b)==Map2.end())
			{
				Map2[vertex_b]=++index;
				// For every vertex in part B, there is a directed edge from that vertex to the dest vertex with cap 1 in original graph
				original.vertex[Map2[vertex_b]].capacity.push_back({dest,1});	// 
				original.vertex[Map2[vertex_b]].flow.push_back({dest,0});

				residual.vertex[Map2[vertex_b]].capacity.push_back({dest,1});
				residual.vertex[dest].capacity.push_back({Map2[vertex_b],0});
			}
            original.vertex[Map1[vertex_a]].capacity.push_back({Map2[vertex_b],1});
            original.vertex[Map1[vertex_a]].flow.push_back({Map2[vertex_b],0});
            /*Note: Everytime a forward edge is added, corresponding backward edge is initialized for possible
            *		future use. Also (i,j)th node in original will be same as (i,j)th node in residual.(not wrt weghts)
            */
            residual.vertex[Map1[vertex_a]].capacity.push_back({Map2[vertex_b],1});
            residual.vertex[Map2[vertex_b]].flow.push_back({Map1[vertex_a],0});//for backward edge
        }
	}
	
	Max_Flow(original,residual,src,dest);
	vector<pair<int,int>> maximumMatching;
	vector<pair<int,int>> maxMatch;
	vector<pair<int,int>> leftNodes=original.vertex[src].capacity;
	for(int i=0;i<leftNodes.size();i++)	// This is for finding edges who have positive flow in it
	{
		int left=leftNodes[i].first;
		vector<pair<int,int>> rightneighbours=original.vertex[left].flow;
		for(int j=0;j<rightneighbours.size();j++)
		{
			if(rightneighbours[j].second>0)
			{
				maximumMatching.push_back({left,rightneighbours[j].first});
			}
		}
	}

	for(int i=0;i<maximumMatching.size();i++)	// This is for finding the original vertices from the indices we have
	{
		int a=maximumMatching[i].first,b=maximumMatching[i].second;
		int aa,bb;
		for(int i=0;i<v_count1;i++) 
		{							
			auto it=Map1.begin();
			while(it!=Map1.end())
			{
				if(it->second==a)
				{
					aa=it->first;
					break;
				}
				it++;
			}
		}

		for(int i=0;i<v_count2;i++) 
		{							
			auto it=Map2.begin();
			while(it!=Map2.end())
			{
				if(it->second==b)
				{
					bb=it->first;
					break;
				}
				it++;
			}
		}
		
		maxMatch.push_back({aa,bb});
	}

	cout<<"Maximum Matching"<<endl;
	for(int i=0;i<maxMatch.size();i++)
		cout<<maxMatch[i].first<< " "<<maxMatch[i].second<<endl;
        
    return 0;
}
