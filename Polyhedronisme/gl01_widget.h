// gl 1.0 version glBegin-glEnd
//
#ifndef GL01_WIDGET_H
#define GL01_WIDGET_H

#include "renderer.h"
#include <poly/polyhedron.hpp>

class gl01_widget : public Renderer {
  Q_OBJECT

private:
  Polyhedron *poly = nullptr;

public:
  gl01_widget(QWidget *parent);
  void draw_poly();
  void draw_lines();

  void draw() override;
  void init() override;

  void set_poly(Polyhedron *poly);
};

#endif // GL01_WIDGET_H
