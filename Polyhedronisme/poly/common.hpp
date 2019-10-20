//
//  common.hpp
//  test_polygon
//
//  Created by asd on 03/09/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef common_h
#define common_h

#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include <simd/simd.h>

#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wsign-compare"


using std::vector, std::string, std::map, std::to_string, std::pair, std::reverse;

typedef simd_float3 Vertex;
typedef vector<Vertex> Vertexes;
typedef vector<int>Face;
typedef vector<Face>Faces;
typedef vector<vector<float>> VertexesFloat;

#endif /* common_h */
