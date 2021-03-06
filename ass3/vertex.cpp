#include <climits>

#include "vertex.h"


#include <functional>
#include <map>
#include <string>

#include "edge.h"


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** Creates an unvisited vertex, gives it a label, and clears its
    adjacency list.
    NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label) { this->vertexLabel = label; }

/** @return  The label of this vertex. */
std::string Vertex::getLabel() const { return this->vertexLabel; }

/** Marks this vertex as visited. */
void Vertex::visit() { this->visited = true; }

/** Marks this vertex as not visited. */
void Vertex::unvisit() { this->visited = false; }

/** Returns the visited status of this vertex.
 @return  True if the vertex has been visited, otherwise
    returns false/ */
bool Vertex::isVisited() const { return this->visited; }

/** Adds an edge between this vertex and the given vertex.
    Cannot have multiple connections to the same endVertex
    Cannot connect back to itself
 @return  True if the connection is successful. */
bool Vertex::connect(const std::string& endVertex, const int edgeWeight) {

    // Find the current vertex
    std::map<std::string, Edge, std::less<std::string>>::iterator 
        currentVertex = adjacencyList.find(vertexLabel);

    // If the labels are the same or if the current vertex's edge end vertex
    // is the same as the end vertex, it means it's already connected
    if (endVertex == this->vertexLabel ||( currentVertex != adjacencyList.end() 
        &&currentVertex->second.getEndVertex()
== endVertex)){
        return false;
    }

    // Create new edge
    Edge* temp = new Edge(endVertex, edgeWeight);
    this->adjacencyList.insert({endVertex, *temp});
    return true; 
}

/** Removes the edge between this vertex and the given one.
@return  True if the removal is successful. */
bool Vertex::disconnect(const std::string& endVertex) {
    auto it = adjacencyList.find(endVertex);

    // Found the item
    if (it != adjacencyList.end()) {
        adjacencyList.erase(endVertex);
        return true;
    }
    
    // Didn't find item
    return false;
}

/** Gets the weight of the edge between this vertex and the given vertex.
 @return  The edge weight. This value is zero for an unweighted graph and
    is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string& endVertex) const { 

    // map.find iterates over the key in the adjacency list
    auto it = adjacencyList.find(endVertex);
    // Found the vertex
    if (it != adjacencyList.end()) {

        // Check if it's connected to the end vertex
        if (it->second.getEndVertex() == endVertex) {

            // It is connected therefore return the weight of the edge
            return it->second.getWeight();
        }
    }

    // Edge does not exist
    return -1;

}

/** Calculates how many neighbors this vertex has.
 @return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors() const { return adjacencyList.size(); }

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor() {
    currentNeighbor = adjacencyList.begin();
}

/** Gets this vertex's next neighbor in the adjacency list.
    Neighbors are automatically sorted alphabetically via map
    Returns the vertex label if there are no more neighbors
 @return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor() {
    int c = 0;
    for (currentNeighbor = adjacencyList.begin(); currentNeighbor != adjacencyList.end(); currentNeighbor++)
    {
        if (c == iterations)
        {
            iterations++;
            return currentNeighbor->second.getEndVertex();
        }
            c++;
    }
    iterations = 0;
    return this->vertexLabel;
   
}

void Vertex::setIterations()
{
    iterations = 0;
}

/** Sees whether this vertex is equal to another one.
    Two vertices are equal if they have the same label. */
bool Vertex::operator==(const Vertex& rightHandItem) const { 
    if (this->vertexLabel.compare(rightHandItem.vertexLabel) == 0)
    {
        return true;
    }
    return false; }

/** Sees whether this vertex is < another one.
    Compares vertexLabel. */
bool Vertex::operator<(const Vertex& rightHandItem) const {
    if (this->vertexLabel.compare(rightHandItem.vertexLabel) < 0)
    {
        return true;
    }
    return false;
}

