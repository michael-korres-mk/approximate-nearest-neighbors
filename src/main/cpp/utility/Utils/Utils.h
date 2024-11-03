//
// Created by micha on 09/10/2024.
//

#ifndef ANN_UTILS_H
#define ANN_UTILS_H

# include <iostream>
#include <vector>

#define BUFFER_SIZE 1024

#define SOURCE_D "src/"
#define MAIN_D "main/"
#define RESOURCES_D "resources/"
#define RESOURCES_P "src/main/resources/"

using namespace std;

template <typename  K>
class Utils {
public:
    static vector<K>& shuffle(vector<K>& v);
    static int flipCoin();
    static int random(int a,int b);
    static void printDivider();
};

#include "Utils.cpp"


#endif //ANN_UTILS_H
