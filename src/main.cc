#include <QApplication>

#include "view/mainwindow/view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::view w;
  w.show();
  return a.exec();
}
