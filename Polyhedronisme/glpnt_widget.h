//
// gl pointer version

#ifndef GLPNT_WIDGET_H
#define GLPNT_WIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <poly/polyhedron.hpp>

class glpnt_widget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  vector<int> triangularize(int nSides, int offset=0);
  void calc_mesh(Polyhedron *poly);

public:
  glpnt_widget(QWidget *parent = nullptr);

  void set_poly(Polyhedron *poly);

  Vertexes vertexes, normals, colors;
};

#endif // GLPNT_WIDGET_H
