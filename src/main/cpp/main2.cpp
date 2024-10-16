#include <iostream>
#include <vector>
#include "utility/Graph/Graph.h"

using namespace std;

int main() {
    Graph<std::string> graph; 
    graph.insertNode("1");
    graph.insertNode("2");
    graph.insertNode("3");
    graph.insertNode("4");
    graph.insertNode("5");
    graph.insertNode("6");
    graph.insertNode("7");
    graph.insertEdge("1", "2");
    graph.insertEdge("2", "3");
    graph.insertEdge("1", "5");
    graph.insertEdge("2", "5");
    graph.insertEdge("1", "6");
    graph.insertEdge("2", "6");
    graph.insertEdge("6", "7");
    graph.insertEdge("4", "1");
    graph.insertEdge("3", "4");
    graph.printOutgoingTransactions("1");
    graph.printIncomingTransactions("2");
    graph.insertEdge("1", "4");


    graph.printGraph(std::cout);



    Graph<double> graphf; 
    graphf.insertNode(2.2);
    graphf.insertNode(3.2);
    graphf.insertNode(443.2);
    graphf.insertNode(4.2);
    graphf.insertNode(5.22);
    graphf.insertNode(6.111);
    graphf.insertNode(11);
    graphf.insertEdge(2.2, 3.2);
    graphf.insertEdge(3.2, 443.2);
    graphf.insertEdge(2.2, 5.22);
    graphf.insertEdge(3.2, 5.22);
    graphf.insertEdge(2.2, 6.111);
    graphf.insertEdge(3.2, 6.111);
    graphf.insertEdge(6.111, 11);
    graphf.insertEdge(4.2, 2.2);
    graphf.insertEdge(443.2, 4.2);
    graphf.printOutgoingTransactions(2.2);
    graphf.printIncomingTransactions(3.2);
    graphf.insertEdge(2.2, 4.2);

    graphf.printGraph(std::cout);

    
    vector<vector<int> > vec; 
    int num = 10;
    for (int i = 0; i < 5; i++) { 
        vector<int> v1; 
  
        for (int j = 0; j < 2; j++) { 
            v1.push_back(num); 
            num += 5; 
        } 
        vec.push_back(v1); 
    } 
    for (auto i = 0u; i < vec.size(); i++) {
        for (auto j = 0u; j < vec[i].size(); j++) 
            cout << vec[i][j] << " "; 
        cout << endl; 
    } 

    Graph<int> graphv(vec); 

    graphv.printGraph(std::cout);

















    vector<vector<float> > vecf; 
    float numf = 4.22;
    for (int i = 0; i < 5; i++) { 
        vector<float> v1; 
  
        for (int j = 0; j < 2; j++) { 
            v1.push_back(numf); 
            numf += 5; 
        } 
        vecf.push_back(v1); 
    } 
    for (auto i = 0u; i < vecf.size(); i++) {
        for (auto j = 0u; j < vecf[i].size(); j++) 
            cout << vecf[i][j] << " "; 
        cout << endl; 
    } 

    Graph<float> graphff(vecf); 

    graphff.insertEdge(vecf[1][1], vecf[2][1]);
    graphff.insertEdge(vecf[2][1], vecf[4][1]);
    graphff.insertEdge(vecf[2][1], vecf[2][0]);
    graphff.insertEdge(vecf[4][0], vecf[4][1]);
    graphff.insertEdge(vecf[2][0], vecf[2][0]);
    graphff.insertEdge(vecf[0][0], vecf[0][1]);


    // 4.22    9.22 
    // 14.22   19.22 
    // 24.22   29.22 
    // 34.22   39.22 
    // 44.22   49.22 


    graphff.printGraph(std::cout);


    
  

    return 0;
}
