#include "mainwindow.h"
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
    p = Parser::parse(ui->poly_trans->text()
                          .toLocal8Bit()
                          .data()); // create the poly w/user input

    ui->statusbar->showMessage(QString::asprintf("# vertex: %ld, # faces: %ld",
                                                 p.n_vertex, p.n_faces));
    ui->poly_widget->set_poly(&p); // render it
  }
}

void MainWindow::on_poly_trans_returnPressed() { update(); }
