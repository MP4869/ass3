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
Graph::Graph() {
    numberOfEdges = 0;
    numberOfVertices = 0;
}

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

    if (it == vertices.end() || it->second->connect(end, edgeWeight))
    {
        if (vertices.count(end) < 1)
        {
            Vertex* temp = new Vertex(end);
            vertices.insert({ end,temp });
            numberOfVertices++;
        }
        if (vertices.count(start) < 1)
        {
            Vertex* temp = new Vertex(start);
            vertices.insert({ start,temp });
            numberOfVertices++;
        }
        it = vertices.find(start);
        it->second->connect(end, edgeWeight);

        return true;
    }
    return false;
}



/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const {

    auto it = vertices.find(start);
    return it->second->getEdgeWeight(end);
}

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
    breadthFirstTraversalHelper(temp, visit);
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
    weight.clear();
    previous.clear();
    unvisitVertices();
    auto cmp = [&](std::string a, std::string b) { return weight[a] > weight[b]; };
    std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);

    Vertex* temp = findVertex(startLabel);
    std::string s = temp->getNextNeighbor();
    while (temp->getLabel() != s) {

        weight[s] = getEdgeWeight(temp->getLabel(), s);
        previous[s] = startLabel;
        pq.push(s);
        s = temp->getNextNeighbor();
    }

    std::set<std::string> vertexSet;
    vertexSet.insert(startLabel);
    while (!pq.empty()) {
       
      
            Vertex* v = findVertex(pq.top());
            pq.pop();
            if (v->getLabel() == startLabel) continue;

        // Vertex not found in the vertexSet
        if (vertexSet.find(v->getLabel()) == vertexSet.end()) {
            for (auto it = vertices.find(v->getNextNeighbor());
                it != vertices.end(); it++) {

                // Neighbor label
                std::string u = it->first;
                // Weight from vertex to neighbor
                int edgeWeight = getEdgeWeight(v->getLabel(), u);

                // If it's the last node, edgeweight will be -1 
                if (edgeWeight < 0) continue;
                if (u == startLabel) continue;
                //weight[neighbor] couldn't be found
                if (weight.find(u) == weight.end()) {
                    weight[u] = weight[v->getLabel()] + edgeWeight;
                    previous[u] = v->getLabel();
                    pq.push(u);
                }
                else {
                    if (weight[u] > weight[v->getLabel()] + edgeWeight) {

                        weight[u] = weight[v->getLabel()] + edgeWeight;
                        previous[u] = v->getLabel();
                        pq.push(u);
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
    startVertex->visit();
    visit(startVertex->getLabel());
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
void Graph::breadthFirstTraversalHelper(Vertex* startVertex,
    void visit(const std::string&)) {
    std::list<Vertex*> queue;
    // Mark the current node as visited and enqueue it
    startVertex->visit();
    visit(startVertex->getLabel());
    queue.push_back(startVertex);



    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        Vertex* temp = queue.front();
        std::cout << temp << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        while (true)
        {
            std::string insert = temp->getNextNeighbor();
            if (insert.compare(temp->getLabel()) == 0)
            {
                break;
            }
            Vertex* temp = vertices.at(insert);
            if (!temp->isVisited())
            {
                temp->visit();
                visit(temp->getLabel());
                queue.push_back(temp);
            }
        }
    }
}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {

    //.second refers to the vertex pointer
    for (const auto& item : vertices) {
        item.second->unvisit();
        item.second->setIterations();
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