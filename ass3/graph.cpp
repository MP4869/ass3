#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include "graph.h"

/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** constructor, empty graph */
Graph::Graph() {}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph() { 
    
    std::map<std::string, Vertex*>::iterator it;
    for (it = vertices.begin(); it != vertices.end(); it++)
    {
        delete it->second;
        it->second = nullptr;
    }
}

/** return number of vertices */
int Graph::getNumVertices() const { return this->numberOfVertices; }

/** return number of edges */
int Graph::getNumEdges() const { return this->numberOfEdges; }

/** add a new edge between start and end vertex
    if the vertices do not exist, create them
    calls Vertex::connect
    a vertex cannot connect to itself
    or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight) {
    std::cout << start + " " + end + " " + std::to_string(edgeWeight) <<
        std::endl;
    auto it = vertices.find(start);
    
    if (it == vertices.end() ||it->second->connect(end, edgeWeight))
    {
        if (vertices.count(end) < 1)
        {
            Vertex* temp = new Vertex(end);
            vertices.insert({ end,temp });
            numberOfVertices++;
        }
        return true;
    }
    return false;
}
    


/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const { 
    
    auto it = vertices.find(start);
    return it->second->getEdgeWeight(end); }

/** read edges from file
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cout << "File could not be opened" << std::endl;
        return;
    }

    std::string start, end;
    int edgeWeight;

    infile >> numberOfEdges;

    for (;;) {

        // Grab start vertex, end vertex, weight of the edge
        infile >> start >> end >> edgeWeight;
     
        add(start, end, edgeWeight);
        
     
        if (infile.eof()) break;
    }
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
                                void visit(const std::string&)) {
    unvisitVertices();
    Vertex* temp = vertices.at(startLabel);
    depthFirstTraversalHelper(temp, visit);
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
    void visit(const std::string&)) {
    unvisitVertices();
    Vertex* temp = vertices.at(startLabel);
    breadthFirstTraversalHelper(temp,  visit);
}

/** find the lowest cost from startLabel to all vertices that can be reached
    using Djikstra's shortest-path algorithm
    record costs in the given map weight
    weight["F"] = 10 indicates the cost to get to "F" is 10
    record the shortest path to each vertex using given map previous
    previous["F"] = "C" indicates get to "F" via "C"

    cpplint gives warning to use pointer instead of a non-const map
    which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(
    std::string startLabel,
    std::map<std::string, int>& weight,
    std::map<std::string, std::string>& previous) {

    unvisitVertices();
    
    std::priority_queue<Vertex*> pq;
    Vertex* vertex = findVertex(startLabel);
    // Insert into pq
    for (auto it = vertices.find(vertex->getLabel()); it != vertices.end(); it++) {
        std::string neighbor = it->first;
        weight[neighbor] = getEdgeWeight(it->first, neighbor);
        previous[neighbor] = vertex->getLabel();
        pq.push(it->second);
    }

    // Ran out of neighbors
    std::set<Vertex*> vertexSet;
    vertexSet.insert(vertex);
    while (!pq.empty()){
        Vertex* v = pq.top();
        
        // V not in vertex set
        if (vertexSet.find(v) == vertexSet.end()) {
            for (auto it = vertices.find(v->getLabel()); it != vertices.end(); 
                it++) {
                // Cost from vertex to it
                int v2ucost = getEdgeWeight(v->getLabel(), it->first);

                // If there is no weight[u]
                if (weight.find(it->first) == weight.end()) {
                    weight[it->first] = weight[it->first] + v2ucost;
                    previous[it->first] = v->getLabel();
                }
                else {
                    if (weight[it->first] > weight[v->getLabel()]) {
                        weight[it->first] = weight[v->getLabel()] + v2ucost;
                        previous[it->first] = v->getLabel();
                        pq.push(it->second);
                    }
                    else {
                        continue;
                    }
                }

            }
        }
    }

}

/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,
                                      void visit(const std::string&)) {
    std::cout << startVertex->getLabel() << " ";
    startVertex->visit();
    // Recur for all the vertices adjacent
    // to this vertex
    while (true)
    {
        std::string insert = startVertex->getNextNeighbor();
        if (insert.compare(startVertex->getLabel()) == 0)
        {
            break;
        }
        Vertex* temp = vertices.at(insert);
        if (!temp->isVisited())
        {
            depthFirstTraversalHelper(temp, visit);
        }  
    }
  
}

/** helper for breadthFirstTraversal */
void Graph::breadthFirstTraversalHelper(Vertex*startVertex,
                                        void visit(const std::string&)) {
    std::list<Vertex*> queue;

    // Mark the current node as visited and enqueue it
    startVertex->visit();
    queue.push_back(startVertex);

 

    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        Vertex*temp = queue.front();
        std::cout << temp << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
      while(true)
        {
          std::string insert = temp->getNextNeighbor();
          if (insert.compare(temp->getLabel())==0)
          {
              break;
          }
          Vertex* temp = vertices.at(insert);
            if (!temp->isVisited())
            {
                temp->visit();
                queue.push_back(temp);
            }
        }
    }
}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {

    //.second refers to the vertex pointer
    for (const auto& item: vertices) {
        item.second->unvisit();
    }
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const {

    auto it = vertices.find(vertexLabel);

    // Found vertex
    if (it != vertices.end()) {
        // it->second returns the pointer for the vertex
        return it->second;
    }
    return nullptr;
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) { 
    auto it = vertices.find(vertexLabel);
    //abc

    // Found vertex
    if (it != vertices.end()) {
        // it->second returns the pointer for the vertex
        return it->second;
    }

    // Create new vertex and return it
    Vertex* v = new Vertex(vertexLabel);
    return v;
}
