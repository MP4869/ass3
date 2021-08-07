#include <string>

#include "edge.h"


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


Edge::Edge() {}

/** constructor with label and weight */
Edge::Edge(const std::string& end, int weight) {
	this->endVertex = end;
	this->edgeWeight = weight;
}

/** return the vertex this edge connects to */
std::string Edge::getEndVertex() const { return endVertex; }

/** return the weight/cost of travlleing via this edge */
int Edge::getWeight() const { return edgeWeight; }
