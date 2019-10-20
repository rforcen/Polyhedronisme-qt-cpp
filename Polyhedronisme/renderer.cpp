#include "renderer.h"

Renderer::Renderer(QWidget *parent) : QOpenGLWidget(parent) { _init(); }
void Renderer::_init() { setFPS(15); }
void Renderer::setFPS(float fps) {
  QTimer *timer = new QTimer(this);
  if (fps != 0.f) {
    timer->start(int(fps != 0.f ? 1000.f / fps : 0.f));
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
  } else
    timer->stop();
}
void Renderer::repaint() { update(); }
void Renderer::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear,
                             GLdouble zFar) {
  GLdouble fH = tan(fovY / 360. * M_PI) * zNear, fW = fH * aspect;
  glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}
void Renderer::initializeGL() { // Set up the rendering context, load shaders
                                // and other resources, etc.:
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  init(); // init gl items here
}
void Renderer::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  perspectiveGL(45, GLdouble(w) / h, 1, 1000);
  glMatrixMode(GL_MODELVIEW);
}
void Renderer::mousePressEvent(QMouseEvent *event) {
  m_lastPos = event->pos();
  emit clicked(event);
}
void Renderer::mouseDoubleClickEvent(QMouseEvent *event) {
  emit doubleClicked(event);
}

void Renderer::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - m_lastPos.x();
  int dy = event->y() - m_lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(m_xRot + 8 * dy);
    setYRotation(m_yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(m_xRot + 8 * dy);
    setZRotation(m_zRot + 8 * dx);
  }
  m_lastPos = event->pos();
}
static void qNormalizeAngle(int &angle) {
  while (angle < 0)
    angle += 360 * 16;
  while (angle > 360 * 16)
    angle -= 360 * 16;
}
void Renderer::setXRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_xRot) {
    m_xRot = angle;
    emit xRotationChanged(angle);
    update();
  }
}
void Renderer::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_yRot) {
    m_yRot = angle;
    emit yRotationChanged(angle);
    update();
  }
}
void Renderer::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != m_zRot) {
    m_zRot = angle;
    emit zRotationChanged(angle);
    update();
  }
}

void Renderer::paintGL() { // Draw the scene:
  if (doRotate) {
    glClearColor(0, 0, 0, 1);
    glLoadIdentity();
    glTranslatef(0, 0, zoom);

    glRotatef(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    glRotatef(m_yRot / 16.0f, 0, 1, 0);
    glRotatef(m_zRot / 16.0f, 0, 0, 1);
    glRotatef(rotAngle, 1, 1, 1);
  }
  draw();
}

void Renderer::sceneInit(
    QColor color) { // works nice for golden solid colors (requires normals)
  float lmodel_ambient[] = {0, 0, 0, 0};
  float lmodel_twoside[] = {GL_FALSE};
  float light0_ambient[] = {.2f, .2f, .2f, 1}; //{0.1f, 0.1f, 0.1f, 1.0f};
  float light0_diffuse[] = {.7f, .7f, .7f, 0}; //{1.0f, 1.0f, 1.0f, 0.0f};
  float light0_position[] = {1, 0.5, 1, 0};
  float light1_position[] = {-1, 0.5, -1, 0};
  float light0_specular[] = {1, 1, 1, 0};
  float bevel_mat_ambient[] = {0, 0, 0, 1};
  float bevel_mat_shininess[] = {40};
  float bevel_mat_specular[] = {1, 1, 1, 0};
  float bevel_mat_diffuse[] = {1, 0, 0, 0};

  glClearColor(float(color.redF()), float(color.greenF()), float(color.blueF()),
               1);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT1, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glEnable(GL_LIGHT1);

  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glEnable(GL_LIGHTING);

  glMaterialfv(GL_FRONT, GL_AMBIENT, bevel_mat_ambient);
  glMaterialfv(GL_FRONT, GL_SHININESS, bevel_mat_shininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bevel_mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, bevel_mat_diffuse);

  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}
