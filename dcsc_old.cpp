#include<bits/stdc++.h>

using namespace std;

int v_count;
int e_count;

class Graph {
    public:
		int no_of_v;
		vector<vector<int>> original;
		vector<vector<int>> transpose;
		vector<bool> valid;
		Graph(int n)
		{
			no_of_v=n;
			original.resize(n);
			transpose.resize(n);
			valid.resize(n,false);
		}
};

vector<bool> visited;
void dfs(int v,vector<vector<int>>& edgeList,set<int>& Set) {
	//To find decendants (pred is desc of transpose graph)
	//Set is what we pass, PRED or DESC
	visited[v]=false;
	Set.insert(v);
	for(int i=0;i<edgeList[v].size();++i) {
        if(visited[edgeList[v][i]]) dfs(edgeList[v][i],edgeList,Set);
    }
}

void DCSC(Graph G) {
	bool flag;
	for(int i=0;i<v_count;++i) {
		if(G.valid[i]) {
			flag=true;
			for(auto t:G.original[i]) {
				if(G.valid[t]) {
					flag=false;
					break;
				}
			}
			if(!flag) {
				break;
			}
		}
	}
	if(flag) {
		for(int i=0;i<G.valid.size();++i) {
			if(G.valid[i])cout<<i<<"\n";
		}
		return;
	}
	
	//Random vertex - Not working (will fix)
	/*
	int k=rand()%G.no_of_v;//no_of_v is used only here
	int v=0;
	while(true) {
		while(!G.valid[v]) {
			++v;
		}
		if(k<=0)
            break;
		++v;
		--k;
	}
	*/
	
	//Choosing first available vertex instead of random
	int v=0;
	while(!G.valid[v]) {
		++v;
		if(v==v_count)
            return;
	}
	
	set<int> Pred;
	visited=G.valid;
	dfs(v,G.original,Pred);
	
	set<int> Desc;
	visited=G.valid;
	dfs(v,G.transpose,Desc);
	
	set<int> SCC;
	set_intersection(Pred.begin(),Pred.end(),Desc.begin(),Desc.end(),inserter(SCC,SCC.begin()));
	for(auto v:SCC) {
		cout<<v<<" ";
	}
	if(!SCC.empty())
        cout<<"\n";
	
	set<int> REM;
	//following lines does G-Pred-Desc = G - (Pred U Desc)
	for(auto v:Pred) {
		G.valid[v]=false;
	}
	for(auto v:Desc) {
		G.valid[v]=false;
	}
	for(int i=0;i<G.valid.size();++i) {
		if(G.valid[i]==true) {
			REM.insert(i);
		}
	}
	
	set<int> P_SCC;//Pred\SCC
	set_difference(Pred.begin(),Pred.end(),SCC.begin(),SCC.end(),inserter(P_SCC,P_SCC.begin()));

	set<int> D_SCC;//Desc\SCC
	set_difference(Desc.begin(),Desc.end(),SCC.begin(),SCC.end(),inserter(D_SCC,D_SCC.begin()));
	
	//following lines create modified graph (by changing valid nodes) for Pred\SCC and calls DCSC
	for(int i=0;i<G.valid.size();++i) {
		G.valid[i]=false;//reset validity
	}
	G.no_of_v=0;
	for(auto v:P_SCC) {
		G.valid[v]=true;
		++G.no_of_v;
	}
	DCSC(G);
	
	//following lines create modified graph (by changing valid nodes) for Desc\SCC and calls DCSC
	for(int i=0;i<G.valid.size();++i) {
		G.valid[i]=false;//reset validity
	}
	G.no_of_v=0;
	for(auto v:D_SCC) {
		G.valid[v]=true;
		++G.no_of_v;
	}
	DCSC(G);
	
	//following lines create modified graph (by changing valid nodes) for REM and calls DCSC
	for(int i=0;i<G.valid.size();++i) {
		G.valid[i]=false;//reset validity
	}
	G.no_of_v=0;
	for(auto v:REM) {
		G.valid[v]=true;
		++G.no_of_v;
	}
	DCSC(G);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	cin>>v_count>>e_count;
	Graph G(v_count);
	
	//File handling -- Doesnt work for this file (too huge?)
	ifstream fin;
	string line;
    fin.open("datasets/facebook_combined.txt");
    while (fin) {
		string a,b; 
        getline(fin, line);
        int i;
        for(i=0;line[i]!=' ';++i)
        {
        	a+=line[i];
		}
		for(++i;i<line.length();++i)
		{
			b+=line[i];
		}
		int vertex_a,vertex_b;
		vertex_a=stoi(a);
		vertex_b=stoi(b);
		G.original[vertex_a].push_back(vertex_b);
		G.transpose[vertex_b].push_back(vertex_a);
		G.valid[vertex_a]=true;
		G.valid[vertex_b]=true;
    } 
    fin.close();
    
	// for(int i=0;i<e_count;++i) {
	// 	int vertex_a,vertex_b;
	// 	cin>>vertex_a>>vertex_b;
	// 	G.original[vertex_a].push_back(vertex_b);
	// 	G.transpose[vertex_b].push_back(vertex_a);
	// 	G.valid[vertex_a]=true;
	// 	G.valid[vertex_b]=true;
	// }

	DCSC(G);
}