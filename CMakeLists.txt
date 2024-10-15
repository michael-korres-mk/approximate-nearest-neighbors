cmake_minimum_required(VERSION 3.29)
project(approximate_nearest_neighbors)

set(CMAKE_CXX_STANDARD 20)

add_executable(approximate_nearest_neighbors src/main/cpp/main.cpp
        src/main/cpp/utility/DataSet/DataSet.cpp
        src/main/cpp/utility/DataSet/DataSet.h
        src/main/cpp/utility/Utils/Utils.cpp
        src/main/cpp/utility/Utils/Utils.h
        src/main/cpp/utility/VectorFileType/VectorFileType.cpp
        src/main/cpp/utility/VectorFileType/VectorFileType.h
        src/main/cpp/utility/Graph/Graph.cpp
        src/main/cpp/utility/Graph/Graph.h
        src/main/cpp/utility/Vertex/Vertex.cpp
        src/main/cpp/utility/Vertex/Vertex.h
        src/main/cpp/utility/Edge/Edge.cpp
        src/main/cpp/utility/Edge/Edge.h
)


