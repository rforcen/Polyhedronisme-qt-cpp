#include "gl_widget.h"

// GLGS programs

static const char *vertexSource = R"(

         uniform mat4 mvp_matrix;
         attribute vec4 a_position, a_normal, a_color;
         varying vec4 v_color, v_normal, fragPos; // -> fragment

         void main()
         {
             // Calculate vertex position in screen space
             fragPos = gl_Position = mvp_matrix * a_position;

             v_normal = a_normal;
             v_color = a_color;
         }
    )";

static const char *fragmentSource = R"(

        varying vec4 v_color, v_normal, fragPos;

        vec4 calc_light() { // using v_color, v_normal
           float l=0.9;
           vec4 lightColor = vec4(l, l, l, 1);
           vec4 lightPos = vec4(-1, 0.5, -1, 1);

           // ambient
           vec4 ambient = lightColor;

           // Diffuse
           vec4 lightDir = normalize(lightPos - fragPos);
           float diff = max(dot(v_normal, lightDir), 0.0);
           vec4 diffuse = diff * lightColor;

           return (ambient + diffuse) * v_color;
        }

        void main()
        {
           gl_FragColor =  calc_light(); //v_color; //
        }
    )";

gl_widget::gl_widget(QWidget *parent) : QOpenGLWidget(parent) {}

gl_widget::~gl_widget() {
  // Make sure the context is current when deleting
  makeCurrent();
  delete buffers;
  doneCurrent();
}

void gl_widget::mousePressEvent(QMouseEvent *e) {
  mousePressPosition = QVector2D(e->localPos());
}

void gl_widget::mouseReleaseEvent(QMouseEvent *e) {
  QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
  QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
  auto acc = diff.length() / 100.0f;
  rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();
  angularSpeed += acc;
}

void gl_widget::timerEvent(QTimerEvent *) {
  angularSpeed *= 0.99f;

  if (angularSpeed < 0.01f) {
    angularSpeed = 0.0;
  } else {
    rotation =
        QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    update();
  }
}

void gl_widget::initializeGL() {

  initializeOpenGLFunctions(); // for this context

  glClearColor(0, 0, 0, 1);
  glClearDepthf(1.0);
  glEnable(GL_DEPTH_TEST); // Enable depth buffer
  glDepthFunc(GL_LEQUAL);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

  timer.start(12, this);

  initShaders();
  buffers = new GLBuffers({"a_position", "a_normal", "a_color"});
}

void gl_widget::initShaders() {
  // Compile vertex shader
  if (program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSource))
    // Compile fragment shader
    if (program.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                        fragmentSource))
      // Link shader pipeline
      if (program.link())
        // Bind shader pipeline for use
        program.bind();
}

void gl_widget::resizeGL(int w, int h) {
  float aspect = float(w) / float(h ? h : 1);
  const float zNear = 1, zFar = 10, fov = 45;

  projection.setToIdentity();
  projection.perspective(fov, aspect, zNear, zFar);
}

void gl_widget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (buffers) {
    QMatrix4x4 mvp_matrix;
    mvp_matrix.translate(0, 0, -4);
    mvp_matrix.rotate(rotation);

    program.setUniformValue("mvp_matrix", projection * mvp_matrix);

    // draw mesh in buffers
    glDrawArrays(GL_TRIANGLES, 0, mesh.n_triangles);
  }
}

void gl_widget::set_poly(Polyhedron *poly) {

  buffers->attributes(&program);      // set mesh shader attributes
  buffers->transfer(mesh.calc(poly)); // transfer

  update();
}
