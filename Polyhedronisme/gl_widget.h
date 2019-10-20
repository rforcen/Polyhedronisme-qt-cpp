#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QQuaternion>

#include "poly/polyhedron.hpp"

#include <math.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class gl_widget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  explicit gl_widget(QWidget *parent = nullptr);
  ~gl_widget() override;

  void mousePressEvent(QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;
  void timerEvent(QTimerEvent *e) override;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void set_poly(Polyhedron *poly);

  void calc_mesh(Polyhedron *poly);
  vector<int> triangularize(int nSides, int offset = 0);

private:
  void initShaders();

  QBasicTimer timer;
  QOpenGLShaderProgram program;

  QMatrix4x4 projection;

  QVector2D mousePressPosition;
  QVector3D rotationAxis;
  float angularSpeed = 0;
  QQuaternion rotation;

  class GLBuffers {
    vector<QOpenGLBuffer> buffs; // coords, normals, colors
    vector<string> attr_names;

  public:
    explicit GLBuffers(vector<string> attr_names) : attr_names(attr_names) {
      buffs = vector<QOpenGLBuffer>(attr_names.size());
      // Generate  VBOs
      for (auto &b : buffs)
        b.create();
    }
    ~GLBuffers() {
      for (auto &b : buffs)
        b.destroy();
    }

    void transfer(int n_vertex, Vertex *vertexes, Vertex *normals,
                  Vertex *colors) { // Transfer vertex data to VBOs
      vector<Vertex *> mv = {vertexes, normals, colors};
      for (size_t i = 0; i < buffs.size(); i++) {
        buffs[i].bind();
        buffs[i].allocate(mv[i], n_vertex * sizeof(Vertex));
      }
    }

    void attributes(QOpenGLShaderProgram *program) {
      // Tell OpenGL programmable pipeline how to locate vertex position data
      for (size_t i = 0; i < buffs.size(); i++) {
        buffs[i].bind();
        int loc = program->attributeLocation(attr_names[i].c_str());
        if (loc != -1) {
          program->enableAttributeArray(loc);
          program->setAttributeBuffer(loc, GL_FLOAT, 0, 3, sizeof(Vertex));
        }
      }
    }
  } *buffers = nullptr;
  Vertexes vertexes, colors, normals;
};

#endif // GL_WIDGET_H
