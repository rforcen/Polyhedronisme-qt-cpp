#ifndef FASTFLAGS_H
#define FASTFLAGS_H

#include "common.hpp"
#include "polyhedron.hpp"

class FastFlags {
public:
  typedef pair<int, Vertex> VertexIndex;
  typedef tuple<int, int, int, int> Int4;
  typedef map<int, VertexIndex> IntVertex;

  FastFlags() {}
  FastFlags(Vertexes vertexes) : vertexes(vertexes) {}

  int add_vertex(Vertex v) {
    vertexes.push_back(v);
    return vertexes.size() - 1;
  }

  VertexIndex vtx_index(Vertex v) { return VertexIndex(0, v); }

  static inline Int4 to_int4(int v) { return Int4(v + 1, 0, 0, 0); }
  static inline Int4 to_int4(int v1, int v2) {
    return Int4(v1 + 1, v2 + 1, 0, 0);
  }
  static inline Int4 to_int4(int v1, int v2, int v3) {
    return Int4(v1 + 1, v2 + 1, v3 + 1, 0);
  }
  static inline Int4 to_int4(int v1, int v2, int v3, int v4) {
    return Int4(v1 + 1, v2 + 1, v3 + 1, v4 + 1);
  }

  Polyhedron to_poly(string name) { // mapi4i4i4, vi4v

    index_vertexes();

    faces.clear();

    for (auto &m0 : map4) {
      Int4 v0 = m0.second.begin()->second, v = v0;
      Face face;

      do {
        face.push_back(vi4v[v].first); // vertex index
        v = m0.second[v];
      } while (v != v0);

      faces.push_back(face);
    }

    vi4v.clear();
    map4.clear();

    return Polyhedron(name, vertexes, faces);
  }

  Vertexes vertexes;
  Faces faces;

  map<Int4, FastFlags::VertexIndex> vi4v;
  map<Int4, map<Int4, Int4>> map4;

  void index_vertexes() { // vi4v
    int i = 0;            // numerate vertexes index & create vertexes[]
    vertexes.clear();
    for (auto &v : vi4v) {
      v.second.first = i++;
      vertexes.push_back(v.second.second);
    }
  }

  void add_face(vector<Int4> vi4) {
    Face face;
    for (auto &i : vi4)
      if (vi4v.find(i) == vi4v.end())
        printf("add_face: index not found (%d, %d, %d, %d)\n", std::get<0>(i),
               std::get<1>(i), std::get<2>(i), std::get<3>(i));
      else
        face.push_back(vi4v[i].first);
    faces.push_back(face);
  }

  int add_vertexes(Vertexes &vertexes) { // vi4v << vertexes
    for (int i = 0; i < vertexes.size(); i++)
      vi4v[to_int4(i)] = vtx_index(vertexes[i]);
    return vertexes.size();
  }
};

static inline FastFlags::Int4 i4(int v1) { return FastFlags::to_int4(v1); }
static inline FastFlags::Int4 i4(int v1, int v2) {
  return FastFlags::to_int4(v1, v2);
}
static inline FastFlags::Int4 i4(int v1, int v2, int v3) {
  return FastFlags::to_int4(v1, v2, v3);
}
static inline FastFlags::Int4 i4(int v1, int v2, int v3, int v4) {
  return FastFlags::to_int4(v1, v2, v3, v4);
}

static inline FastFlags::Int4 i4_min(int v1, int v2) {
  return v1 < v2 ? i4(v1, v2) : i4(v2, v1);
}
static inline FastFlags::Int4 i4_min(int i, int v1, int v2) {
  return v1 < v2 ? i4(i, v1, v2) : i4(i, v2, v1);
}
#endif // FASTFLAGS_H
