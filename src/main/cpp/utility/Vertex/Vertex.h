#ifndef NODE_HPP
#define NODE_HPP

#include <list>
#include "../Edge/Edge.h"

using namespace std;

template <typename IDType>
class Node {
private:
    IDType id;
    list<Edge<IDType>> incomingEdges;
    list<Edge<IDType>> outgoingEdges;

public:
    Node(const IDType& id) : id(id) {}

    const IDType& getId() const { return id; }

    void addIncomingEdge(const Edge<IDType>& edge) {
        incomingEdges.push_back(edge);
    }

    void addOutgoingEdge(const Edge<IDType>& edge) {
        outgoingEdges.push_back(edge);
    }

    void removeIncomingEdge(Node<IDType>* sourceNode) {
        incomingEdges.remove_if([sourceNode](const Edge<IDType>& edge) {
            return edge.getSource() == sourceNode;
        });
    }

    void removeOutgoingEdge(Node<IDType>* destNode) {
        outgoingEdges.remove_if([destNode](const Edge<IDType>& edge) {
            return edge.getDestination() == destNode;
        });
    }

    const list<Edge<IDType>>& getIncomingEdges() const { return incomingEdges; }
    const list<Edge<IDType>>& getOutgoingEdges() const { return outgoingEdges; }
};

#endif // NODE_HPP
