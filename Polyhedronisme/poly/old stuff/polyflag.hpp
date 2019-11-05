//===================================================================================================
// Polyhedron Flagset Construct
//
// A Flag is an associative triple of a face index and two adjacent vertex
// vertidxs, listed in geometric clockwise order (staring into the normal)
//
// Face_i -> V_i -> V_j
//
// They are a useful abstraction for defining topological transformations of the
// polyhedral mesh, as one can refer to vertices and faces that don't yet exist
// or haven't been traversed yet in the transformation code.
//
// A flag is similar in concept to a directed halfedge in halfedge data
// structures.
//
#ifndef polyflag_hpp
#define polyflag_hpp

#include "common.hpp"
#include "polyhedron.hpp"

class Polyflag {
  map<string, map<string, string>>
      flags; // flags[face][vertex] = next vertex of flag; symbolic triples

  class VertexIndex {
  public:
    VertexIndex() {}
    VertexIndex(Vertex vertex) : vertex(vertex), index(0) {}
    VertexIndex(Vertex vertex, int index) : vertex(vertex), index(index) {}

    VertexIndex &operator=(Vertex vertex) {
      this->vertex = vertex;
      index = 0;
      return *this;
    }

    Vertex vertex = {0, 0, 0};
    int index = 0;
  };

  map<string, VertexIndex> vertices; // XYZ coordinates

  void update_index(Vertexes &vertexes) { // fill vertexes & update index
    int ctr = 0;
    for (auto &vix : vertices) {
      VertexIndex &vi = vix.second;
      vertexes[ctr] = vi.vertex;
      vi.index = ctr++;
    }
  }

public:
  Polyflag() {}
  Polyflag(Polyhedron &poly) { add_vertexes(poly.vertexes); }

  void add_vertexes(Vertexes &vertexes, string prefix = "v") {
    for (auto i = 0; i < vertexes.size(); i++)
      new_vertex(prefix + to_string(i), unit(vertexes[i]));
  }

  // Add a new vertex named "name" with coordinates "vertex".
  void new_vertex(string vertName, Vertex vertex) {
    vertices[vertName] = vertex;
  }

  void new_flag(string faceName, string vertName1, string vertName2) {
    flags[faceName][vertName1] = vertName2;
  }

  int get_index(string vs) { return vertices[vs].index; }

  Polyhedron to_poly(string name = "unknown polyhedron") {
    Vertexes vertexes(vertices.size());
    Faces faces(flags.size());

    update_index(vertexes);

    int ctr = 0;
    for (auto &flag : flags) {

      auto &face = flag.second;          // map<string,string>
      string &v0 = face.begin()->second; // first item

      // build face out of all the edge relations in the flag assoc array

      auto v = v0; // v moves around face
      do {
        faces[ctr].push_back(vertices[v].index); // record index
        v = face[v];
      } while (v != v0);

      ctr++;
    }

    return Polyhedron(name, vertexes, faces);
  }
};

#endif // polyflag_hpp
