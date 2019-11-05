#ifndef RENDERER_H
#define RENDERER_H

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>

#include <math.h>
#include <random>

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  explicit Renderer(QWidget *parent = nullptr);
  float rotAngle = 0;
  void setZoom(float zoom) { this->zoom = zoom; }
  void rotate(float ang) { rotAngle += ang; }
  void setFPS(float fps);
  void setRotate(bool doRotate = false) { this->doRotate = doRotate; }
  void sceneInit(QColor = QColor());
  bool doRotate = true;

  std::vector<GLenum> array_type = {GL_VERTEX_ARRAY, GL_NORMAL_ARRAY,
                                    GL_COLOR_ARRAY};

  void enableClient() {
    for (auto &s : array_type) // enable
      glEnableClientState(s);
  }
  void disableClient() {
    for (auto &s : array_type) // disable
      glDisableClientState(s);
  }

private slots:
  void repaint();
public slots:
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);
signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);
  void clicked(QMouseEvent *event);
  void doubleClicked(QMouseEvent *event);

private:
  void _init();
  void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear,
                     GLdouble zFar);
  float angle = 0, zoom = -5;
  QPoint m_lastPos;
  int m_xRot = 0, m_yRot = 0, m_zRot = 0;

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  virtual void draw() = 0;
  virtual void init() = 0;
};

#endif
