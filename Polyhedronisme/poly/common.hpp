//
//  common.hpp
//  test_polygon
//
//  Created by asd on 03/09/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef common_h
#define common_h

#include <algorithm>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>

#include <simd/simd.h>

#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wmultichar"
#pragma clang diagnostic ignored "-Wfour-char-constants"

using std::vector, std::string, std::map, std::to_string, std::pair,
    std::reverse, std::min, std::max, std::tuple;

typedef simd_float3 Vertex;
typedef vector<Vertex> Vertexes;
typedef vector<int> Face;
typedef vector<Face> Faces;
typedef vector<vector<float>> VertexesFloat;

#pragma clang diagnostic ignored "-Wunused-function"

static inline string str(size_t i) { return to_string(i); }
static inline string str(int i) { return to_string(i); }
static inline string str(string s, size_t i) { return s + str(i); }
static inline string midName(int v1, int v2) {
  return v1 < v2 ? str(v1) + "_" + str(v2) : str(v2) + "_" + str(v1);
}
static inline Vertex midpoint(Vertex vec1, Vertex vec2) {
  return (vec1 + vec2) / 2.;
}
static inline Vertex unit(Vertex v) { return simd::normalize(v); }

static inline Vertex tween(Vertex &vec1, Vertex &vec2, float t) {
  return ((1.f - t) * vec1) + (t * vec2);
}
static inline Vertex oneThird(Vertex &vec1, Vertex &vec2) {
  return tween(vec1, vec2, 1 / 3.f);
}
static inline int intersect(Face &set1, Face &set2, Face &set3) {
  for (auto s1 : set1)
    for (auto s2 : set2)
      if (s1 == s2)
        for (auto s3 : set3)
          if (s1 == s3)
            return s1;
  return -1; // empty intersection
}
static Vertex calcCentroid(Vertexes vertices) {
  // running sum of vertex coords
  Vertex centroidV = 0;
  for (auto &v : vertices)
    centroidV += v;
  return centroidV / vertices.size();
}
#endif /* common_h */
