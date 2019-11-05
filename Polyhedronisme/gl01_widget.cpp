#include "gl01_widget.h"

gl01_widget::gl01_widget(QWidget *parent) : Renderer(parent) {}

void gl01_widget::init() {
  sceneInit();
  setZoom(-4);
}

void gl01_widget::draw() {
  if (poly)
    glCallList(1);
}

void gl01_widget::draw_poly() {
  if (poly) {
    int ixf = 0;
    for (auto face : poly->faces) {
      glBegin(GL_POLYGON);

      auto color = poly->get_color(ixf);
      glColor3f(color.r, color.g, color.b);

      auto normal = poly->get_normal(ixf);
      glNormal3f(normal.x, normal.y, normal.z);

      for (auto ix_coord : face) {
        auto v = poly->vertexes[size_t(ix_coord)];
        glVertex3f(v.x, v.y, v.z);
      }
      glEnd();
      ixf++;
    }
  }
}

void gl01_widget::draw_lines() {
    for (auto face : poly->faces) {
      glBegin(GL_LINES);
      glColor3f(0,0,0);

      for (auto ix_coord : face) {
        auto v = poly->vertexes[size_t(ix_coord)];
        glVertex3f(v.x, v.y, v.z);
      }
      glEnd();
    }
}

void gl01_widget::set_poly(Polyhedron *poly) {
  this->poly = poly;

  glNewList(1, GL_COMPILE);

  draw_poly();
  draw_lines();

  glEndList();

  update();
}
