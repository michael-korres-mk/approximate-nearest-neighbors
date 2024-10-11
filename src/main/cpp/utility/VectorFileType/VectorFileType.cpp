//
// Created by mkorres on 10/9/2024.
//

# include "VectorFileType.h"


VectorFileType::Type VectorFileType::getFileType(const string &filename) {
    if (filename.ends_with(".fvecs")) {
        return FVECS;
    } else if (filename.ends_with(".ivecs")) {
        return IVECS;
    } else if (filename.ends_with(".bvecs")) {
        return BVECS;
    } else {
        return UNKNOWN;
    }
}
