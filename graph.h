#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>

class Graph {

    private:
    std::map<int, std::list<std::pair<int,int>>> adjList; // Adjacency list to store the graph

    public:
        // Function to add an edge between vertices u and v of
        // the graph
        void add_edge(int start, int destination, int weight)
        {
            // Add edge from start to destination
            adjList[start].push_back({destination, weight});
            // Add edge from destination to start because the graph is
            // undirected
            adjList[destination].push_back({start, weight});
        }
    
        // Read-only access, so DFS / Dijkstra can walk the graph later
        const std::map<int, std::list<std::pair<int,int>>>& getAdjList() const
        {
            return adjList;
        }
    
        int vertexCount() const { 
            return (int)adjList.size(); 
        }
    
        int edgeCount() const
        {
            int total = 0;
            for (const auto& entry : adjList)
                total += (int)entry.second.size();
            return total / 2; // each undirected edge is stored twice
        }
};

#endif



