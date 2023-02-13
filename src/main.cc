#include <QApplication>

#include "controller.h"
#include "main_window.h"
#include "model.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Model model;
  Controller controller(&model);
  MainWindow window(&controller);
  window.show();

  return app.exec();
}
