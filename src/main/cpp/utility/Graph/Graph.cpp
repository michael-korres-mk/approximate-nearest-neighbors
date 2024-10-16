#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"

template <typename IDType>
Graph<IDType>::Graph() = default;

template <typename IDType>
Graph<IDType>::~Graph() = default;

template <typename IDType>
Graph<IDType>::Graph(const vector<vector<IDType>>& vec){
    for(auto i = 0u; i < vec.size(); i++)
        for(auto j = 0u; j < vec[i].size(); j++)
            this->insertNode(vec[i][j]);
}

template <typename IDType>
bool Graph<IDType>::insertNode(const IDType& id){
    auto result = nodes.emplace(id, Node<IDType>(id));
    if(!result.second){
        cout << "Node with ID " << id << " already exists." << endl;
        return false;
    }
    return true;
}

template <typename IDType>
bool Graph<IDType>::insertEdge(const IDType& sourceId, const IDType& destId){
    auto sourceIt = nodes.find(sourceId);
    auto destIt = nodes.find(destId);

    if(sourceIt == nodes.end() || destIt == nodes.end()){
        cout << "One or both nodes not found." << endl;
        return false;
    }

    Node<IDType>* sourceNode = &(sourceIt->second);
    Node<IDType>* destNode = &(destIt->second);

    Edge<IDType> edge(sourceNode, destNode);
    sourceNode->addOutgoingEdge(edge);
    destNode->addIncomingEdge(edge);

    return true;
}

template <typename IDType>
bool Graph<IDType>::deleteEdge(const IDType& sourceId, const IDType& destId){
    auto sourceIt = nodes.find(sourceId);
    auto destIt = nodes.find(destId);

    if(sourceIt == nodes.end() || destIt == nodes.end()){
        cout << "One or both nodes not found." << endl;
        return false;
    }

    Node<IDType>* sourceNode = &(sourceIt->second);
    Node<IDType>* destNode = &(destIt->second);

    sourceNode->removeOutgoingEdge(destNode);
    destNode->removeIncomingEdge(sourceNode);

    return true;
}

template <typename IDType>
bool Graph<IDType>::printOutgoingTransactions(const IDType& id){
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()){
        cout << "Node with ID " << id << " not found." << endl;
        return false;
    }

    Node<IDType>& node = nodeIt->second;
    cout << "Outgoing edges for node " << id << ":" << endl;
    for(const Edge<IDType>& edge : node.getOutgoingEdges()){
        cout << "  To node " << edge.getDestination()->getId() << endl;
    }
    return true;
}

template <typename IDType>
bool Graph<IDType>::printIncomingTransactions(const IDType& id){
    auto nodeIt = nodes.find(id);
    if(nodeIt == nodes.end()){
        cout << "Node with ID " << id << " not found." << endl;
        return false;
    }

    Node<IDType>& node = nodeIt->second;
    cout << "Incoming edges for node " << id << ":" << endl;
    for(const Edge<IDType>& edge : node.getIncomingEdges()){
        cout << "  From node " << edge.getSource()->getId() << endl;
    }
    return true;
}

template <typename IDType>
void Graph<IDType>::printGraph(std::ostream& out){
    out << "Graph with " << nodes.size() << " nodes:" << endl;
    for(const auto& pair : nodes){
        const Node<IDType>& node = pair.second;
        out << "Node " << node.getId() << ":" << endl;
        out << "  Outgoing edges:" << endl;
        for(const Edge<IDType>& edge : node.getOutgoingEdges()){
            out << "    To node " << edge.getDestination()->getId() << endl;
        }
        out << "  Incoming edges:" << endl;
        for(const Edge<IDType>& edge : node.getIncomingEdges()){
            out << "    From node " << edge.getSource()->getId() << endl;
        }
    }
}

#endif // GRAPH_CPP
