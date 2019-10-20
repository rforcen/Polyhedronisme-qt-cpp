#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "poly/parser.hpp"
#include "poly/polyhedron.hpp"
#include "poly/seeds.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  void showEvent(QShowEvent *) override;

  void update();
private slots:
  void on_poly_trans_returnPressed();

private:
  Ui::MainWindow *ui;

  Polyhedron p;
  bool init_gl = true;
};
#endif // MAINWINDOW_H
