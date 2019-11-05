#include "glpnt_widget.h"
#include <QDebug>

glpnt_widget::glpnt_widget(QWidget *parent) : Renderer(parent) {}

void glpnt_widget::set_poly(Polyhedron *poly) {

  mesh.calc(poly);

  update();
}

void glpnt_widget::init() {
  sceneInit();
  setZoom(-4);
}

void glpnt_widget::draw() {
  if (mesh.n_triangles) {

    enableClient();

    // set data pointers
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), mesh.get_vertex_data());
    glNormalPointer(GL_FLOAT, sizeof(Vertex), mesh.get_normal_data());
    glColorPointer(3, GL_FLOAT, sizeof(Vertex), mesh.get_color_data());

    // draw
    glDrawArrays(GL_TRIANGLES, 0, mesh.n_triangles);

    disableClient();
  }
}
