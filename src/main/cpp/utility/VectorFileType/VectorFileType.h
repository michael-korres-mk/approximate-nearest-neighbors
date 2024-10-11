//
// Created by mkorres on 10/9/2024.
//

#ifndef VECTORFILEDATATYPE_H
#define VECTORFILEDATATYPE_H
# include <string>
using namespace std;

class VectorFileType {
public:
    enum Type {
        FVECS, // For .fvecs files
        IVECS, // For .ivecs files
        BVECS, // For .bvecs files
        UNKNOWN // For unknown types
    };

    static Type getFileType(const string& filename);
};
#endif //VECTORFILEDATATYPE_H
