cmake_minimum_required(VERSION 3.29)
project(approximate_nearest_neighbors)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")

add_executable(approximate_nearest_neighbors src/main/cpp/ann.cpp
        src/main/cpp/utility/DataSet/DataSet.cpp
        src/main/cpp/utility/DataSet/DataSet.h
        src/main/cpp/utility/Utils/Utils.cpp
        src/main/cpp/utility/Utils/Utils.h
        src/main/cpp/utility/Graph/Graph.cpp
        src/main/cpp/utility/Graph/Graph.h
        src/main/cpp/utility/Edge/Edge.cpp
        src/main/cpp/utility/Edge/Edge.h
        src/main/cpp/utility/VamanaContainer/VamanaContainer.cpp
        src/main/cpp/utility/VamanaContainer/VamanaContainer.h
        src/main/cpp/utility/FilterDataset/FilterDataset.cpp
        src/main/cpp/utility/FilterDataset/FilterDataset.h
        src/main/cpp/utility/DataPoint/DataPoint.cpp
        src/main/cpp/utility/DataPoint/DataPoint.h
        src/main/cpp/utility/FilterQuerySet/FilterQuerySet.cpp
        src/main/cpp/utility/FilterQuerySet/FilterQuerySet.h
        src/main/cpp/utility/Query/Query.cpp
        src/main/cpp/utility/Query/Query.h

)