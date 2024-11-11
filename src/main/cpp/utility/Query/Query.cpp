//
// Created by mkorres on 11/11/2024.
//

#include "Query.h"
#include "../Utils/Utils.h"

template<typename K>
Query<K>::Query() {}

template<typename K>
Query<K>::Query(int queryType, int v, float l, float r, vector<K> vec): queryType(queryType), v(v), l(l), r(r), vec(vec) {}

template<typename K>
void Query<K>::print() {

    int temp = (FILTER_QUERY_SET_DIMENSION - 4) - 1;

    cout << "queryType = " << queryType << endl;
    cout << "v = " << v << endl;
    cout << "l = " << l << endl;
    cout << "r = " << r << endl;
    for(int j = 0; j < temp; j++) {
        cout << vec[j] << " ";
    }
    cout << setw(4) << fixed << setprecision(4) << vec[temp] << endl;

}

// explicit template instantiations
template class Query<float>;
template class Query<int>;
template class Query<char>;