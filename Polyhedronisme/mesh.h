#ifndef MESH_H
#define MESH_H

#include <poly/common.hpp>
#include <poly/polyhedron.hpp>

class Mesh {
  enum { e_vertex, e_normal, e_color };

public:
  vector<Vertexes> mesh = vector<Vertexes>(3); // v,n,c
  int n_triangles = 0;

  void init() {
    mesh.clear();
    mesh = vector<Vertexes>(3);
    n_triangles = 0;
  }

  vector<int> triangularize(
      int nSides,
      int offset = 0) { // generate nSides polygon set of trig index coords
    vector<int> res((nSides - 2) * 3);
    for (int i = 1; i < nSides - 1;
         i++) { // 0, i, i+1 : i=1..ns-1, for quad=4: 0 1 2 0 2 3
      int vi[] = {0, i, i + 1};
      for (int j = 0; j < 3; j++)
        res[(i - 1) * 3 + j] = vi[j] + offset;
    }
    return res;
  }

  map<int, vector<int>> gen_trigs_map(Polyhedron *poly) {
    map<int, vector<int>> tm;
    for (auto &face : poly->faces)
      if (tm.find(face.size()) == tm.end()) // not found->generate
        tm[face.size()] = triangularize(face.size());
    return tm;
  }

  Mesh &calc(Polyhedron *poly) { // -> create trigs of vertexes, normals, colors

    init();

    auto trig_map = gen_trigs_map(poly);

    for (int iface = 0; iface < poly->faces.size(); iface++) {

      auto &face = poly->faces[iface];
      int fs = face.size();

      auto color = poly->get_color(iface); // current color, normal
      auto normal = poly->get_normal(iface);

      for (auto ixv : trig_map[fs]) { // set colors & normals for face vertex
        mesh[e_vertex].push_back(poly->vertexes[face[ixv]]);
        mesh[e_color].push_back(color);
        mesh[e_normal].push_back(normal);
      }
    }

    n_triangles = mesh[e_vertex].size();

    return *this;
  }

  void *get_vertex_data() { return mesh[e_vertex].data(); }
  void *get_normal_data() { return mesh[e_normal].data(); }
  void *get_color_data() { return mesh[e_color].data(); }

  void *get_data(int i) { return mesh[i].data(); }

  int get_size() { return n_triangles * sizeof(Vertex); }
};
#endif // MESH_H
