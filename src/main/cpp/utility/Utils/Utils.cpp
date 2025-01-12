//
// Created by micha on 09/10/2024.
//

# include "Utils.h"

using namespace std;

template <typename K>
vector<K>& Utils<K>::shuffle(vector<K>& v){
    const unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);
    std::shuffle(v.begin(), v.end(), rng);
    return v;
}

template <typename K>
int Utils<K>::flipCoin() { // Function to simulate a coin flip
   return random(0,1);

}

template <typename K>
int Utils<K>::random(int a,int b) { // Function to simulate a coin flip
    // seed engine
    random_device seed;

    // Random number engine
    mt19937 generator(seed());

    // Set the range => {a, b}
    uniform_int_distribution<int> distribution(a, b);

    // Generate a random integer
    return distribution(generator);

}

template<typename K>
void Utils<K>::printDivider() {
    cout << "-----------------------------------------------------------" << endl;
}

template<typename K>
void Utils<K>::printVec(vector<K> v) {
    const int size = v.size();

    string fmt = new char[size];

    if (is_same_v<K, int>) {
        fmt = "%d "; // Print as integer
    }
    else if (is_same<K, float>::value || is_same<K, double>::value) {
        fmt = "%f "; // Print as float/double with 2 decimal places
    }
    else if (is_same<K, char>::value) {
        fmt = "%c "; // Print as character
    }

    for(int i = 0; i < size; i++) {
       printf(fmt.c_str(),v[i]);
    }
    cout << endl;
}

template<typename K>
FILE* Utils<K>::fileopen(const char* filename, const char* columns) {

    FILE* file = fopen(filename, "r");

    if (file != NULL) {
        fclose(file);

        file = fopen(filename, "a");

        fseek(file, 0, SEEK_END);
        if(ftell(file) == 0) fprintf(file,"%s", columns);

    } else {
        printf("File just created.\n");
        file = fopen(filename, "a");
        if (file == NULL) {
            perror("Error opening file");
            exit(1);
        }
        fprintf(file,"%s", columns);
    }

    return file;
}


template class Utils<float>;
template class Utils<int>;
template class Utils<char>;
