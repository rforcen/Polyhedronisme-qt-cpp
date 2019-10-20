#include "glpnt_widget.h"
#include <QDebug>

glpnt_widget::glpnt_widget(QWidget *parent) : QOpenGLWidget(parent) {}

void glpnt_widget::set_poly(Polyhedron *poly) {
  //  set (vertex, normals, color) pointers

  calc_mesh(poly);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertexes.data());
  glNormalPointer(GL_FLOAT, sizeof(Vertex), normals.data());
  glColorPointer(3, GL_FLOAT, sizeof(Vertex), colors.data());

  repaint();
}

vector<int> glpnt_widget::triangularize(
    int nSides,
    int offset) { // generate nSides polygon set of trig index coords
  vector<int> res((nSides - 2) * 3);
  for (int i = 1; i < nSides - 1;
       i++) { // 0, i, i+1 : i=1..ns-1, for quad=4: 0 1 2 0 2 3
    int vi[] = {0, i, i + 1};
    for (int j = 0; j < 3; j++)
      res[(i - 1) * 3 + j] = vi[j] + offset;
  }
  return res;
}

void glpnt_widget::calc_mesh(
    Polyhedron *poly) { // -> create trigs of vertexes, normals, colors

  vertexes.clear();
  normals.clear();
  colors.clear();

  map<int, vector<int>> trig_map; // trig map -> don't recalc trigs

  for (int iface = 0; iface < poly->faces.size(); iface++) {

    auto face = poly->faces[iface];
    int fs = int(face.size());

    if (trig_map.find(fs) == trig_map.end()) // new -> calc & use
      trig_map[fs] = triangularize(fs);

    auto color = poly->get_color(iface); // current color, normal
    auto normal = poly->get_normal(iface);

    for (auto ixv : trig_map[fs]) { // set colors & normals for face vertex
      vertexes.push_back(poly->vertexes[face[ixv]]);

      colors.push_back(color);
      normals.push_back(normal);
    }
  }
}

void glpnt_widget::initializeGL() {

  initializeOpenGLFunctions(); // for this context

  glClearColor(0, 0, 0, 1);
  glClearDepthf(1.0);
  glEnable(GL_DEPTH_TEST); // Enable depth buffer
  glDepthFunc(GL_LEQUAL);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
}

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear,
                   GLdouble zFar) {
  GLdouble fH = tan(fovY / 360. * M_PI) * zNear, fW = fH * aspect;
  glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void glpnt_widget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  perspectiveGL(45, GLdouble(w) / h, 1, 1000);
  glMatrixMode(GL_MODELVIEW);
}

void glpnt_widget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0, 0, 0, 1);
  glLoadIdentity();

  glTranslatef(0, 0, -4);

  if (vertexes.size())
    glDrawArrays(GL_TRIANGLES, 0, vertexes.size());
}
