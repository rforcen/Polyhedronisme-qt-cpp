#include "mainwindow.h"
#include "Timer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showEvent(QShowEvent *) {
  if (init_gl) {
    update();
    init_gl = false;
  }
}

void MainWindow::update() {

  if (!ui->poly_trans->text().isEmpty()) {
    auto ts = Timer().chrono([this]() {
      p = Parser::parse(ui->poly_trans->text()
                            .toLocal8Bit()
                            .data()); // create the poly w/user input
    });
    ui->statusbar->showMessage(QString::asprintf(
        "# vertex: %ld, # faces: %ld, lap: %ld", p.n_vertex, p.n_faces, ts));

    ui->shader->set_poly(&p);

    ui->poly_widget->set_poly(&p); // render it
    ui->gl10->set_poly(&p);
    ui->pnt->set_poly(&p);
  }
}

void MainWindow::on_poly_trans_returnPressed() { update(); }
