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

#include "mesh.h"

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

private:
  Mesh mesh;
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
    Mesh mesh; // local copy of mesh, never use disposable data copy

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

    void transfer(Mesh &mesh) { // Transfer vertex data to VBOs (use local mesh
                                // -> don't use original vector pointers!!)

      const int sz = mesh.get_size();
      this->mesh = mesh; // transfer to local copy (deep copy)

      for (size_t i = 0; i < buffs.size(); i++) {
        buffs[i].bind();
        buffs[i].allocate(this->mesh.get_data(i), sz);
      }
    }

    void attributes(
        QOpenGLShaderProgram *program) { // Tell OpenGL programmable pipeline
                                         // how to locate vertex position data
      for (size_t i = 0; i < buffs.size(); i++) {
        buffs[i].bind();

        int att_loc = program->attributeLocation(attr_names[i].c_str());
        if (att_loc != -1) {
          program->enableAttributeArray(att_loc);
          program->setAttributeBuffer(att_loc, GL_FLOAT, 0, 3, sizeof(Vertex));
        }
      }
    }
  } *buffers = nullptr;
};

#endif // GL_WIDGET_H
