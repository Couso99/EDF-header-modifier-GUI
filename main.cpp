#include "drop_window.h"

#include <QApplication>

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  DropWindow w;

  w.show();
  return a.exec();
}
