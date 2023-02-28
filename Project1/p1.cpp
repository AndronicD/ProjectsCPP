#include <bits/stdc++.h>
using namespace std;
int nrElemCiclu[10002];
int ciclu[10002];
int vizitat[10002];

class Graph
{
    int V;
    list<int> *adj;
public:
    bool isCyclicUtil(int v, bool visited[], bool *rs);
    void isCyclic(int numComponenta[]);
    void DFSUtil(int v, int nodComponenta[], int componenta);
    void print(int n);
    Graph(int V);
    void addEdge(int v, int w);
};
  
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V + 1];
}

void Graph::print(int n){
    list<int>::iterator j;
    for(int i = 1; i <= n; i++){
        cout<<i<<": ";
        for (j = adj[i].begin(); j != adj[i].end(); ++j){
            cout<<*j<<" ";
        }
        cout<<endl;
    }
}
  
void Graph::DFSUtil(int v, int nodComponenta[10002], int componenta)
{
    nodComponenta[v] = componenta;
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i){
        if (!nodComponenta[*i])
            DFSUtil(*i, nodComponenta, componenta);
    }        
}
  
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}

int moreComponents(int nodComponenta[10002], int n){
    for(int i = 1; i <= n ; i++){
        if(nodComponenta[i] == 0){
            return 1;
        }
    }
    return 0;
}

bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack)
{
    if(visited[v] == false)
    {
        visited[v] = true;
        recStack[v] = true;
 
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) )
                return true;
            else if (recStack[*i])
                return true;
        }
 
    }
    recStack[v] = false;
    return false;
}

int findNumberElem(int numComponenta[10002], int comp, int nrNod){
    int fin = 0;
    for(int i = 1; i <= nrNod; i++){
        if(numComponenta[i] == comp){
            fin++;
        }
    }
    return fin;
}

void Graph::isCyclic(int numComponenta[10002])
{
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 1; i <= V; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }
    int sum = 0;
    int compcurr = 1;
    for(int i = 1; i <= V; i++){
        if ( !visited[i] && isCyclicUtil(i, visited, recStack)){
            ciclu[vizitat[i]] = 1;
        }
    }
}
 
int main()
{
    int n, m;
    ifstream fin("p1.in");
    fin >> n >> m;
    Graph g(n);
    Graph g2(n);
    int u, v;
    for(int i = 0; i < m; i++){
        fin >> u >> v;
        g.addEdge(u, v);
        g2.addEdge(u, v);
        g2.addEdge(v, u);
    }

    fin.close();        

    int componenta = 1;
    int nodComponenta[10002];
    
    for(int i = 1; i <= n; i++){
        nodComponenta[i] = -1;
    }

    for(int i = 1; i <= n; i++){
        nodComponenta[i] = 0;
    }

    while(moreComponents(nodComponenta, n)){
        for(int i = 1; i <= n; i++){
            if(nodComponenta[i] == 0){
                g2.DFSUtil(i, nodComponenta, componenta);
                componenta++;
            }
        }
    }

    memcpy(vizitat, nodComponenta, 10002);

    int compcurr = 1;
    for(int i = 1; i <= 10002; i++){
        nrElemCiclu[i] = findNumberElem(nodComponenta, compcurr, n);
        compcurr++;
    }

    int sum = 0;
    g.isCyclic(nodComponenta);
   
    for(int i = 1; i <= componenta - 1; i++){
       if(ciclu[i] == 1){
           sum += nrElemCiclu[i]; 
       }
       else{
           sum += nrElemCiclu[i] - 1;
       }
    }
    ofstream fout("p1.out");
    fout << sum;
    fout.close();
    return 0;
}