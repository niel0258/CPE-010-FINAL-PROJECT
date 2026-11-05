#include <list>
#include <map>

class Graph {

    private:
    std::map<int, std::list<std::pair<int,int>>> adjList; // Adjacency list to store the graph

    public:
        // Function to add an edge between vertices u and v of
        // the graph
        void add_edge(int start, int destination,int weight)
        {
            // Add edge from start to destination
            adjList[start].push_back({destination,weight});
            // Add edge from destination to start because the graph is
            // undirected
            adjList[destination].push_back({start,weight});
        }
};



