#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>

using namespace std;

template <typename IDType>
class Node; // Προκαταρκτική δήλωση της κλάσης Node

template <typename IDType>
class Edge{
private:
    Node<IDType>* source;      // Κόμβος πηγής
    Node<IDType>* destination; // Κόμβος προορισμού

public:
    Edge(Node<IDType>* source, Node<IDType>* destination) : source(source), destination(destination) {}

    Edge(const Edge& other) = default;            // Προεπιλεγμένος copy constructor
    Edge& operator=(const Edge& other) = default; // Προεπιλεγμένος operator=

    Node<IDType>* getSource() const { return source; }
    Node<IDType>* getDestination() const { return destination; }
};

#endif // EDGE_HPP
