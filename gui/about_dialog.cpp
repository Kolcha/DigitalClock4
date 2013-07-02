#include "about_dialog.h"
#include "ui_about_dialog.h"

AboutDialog::AboutDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/images/about.svg"));
}

AboutDialog::~AboutDialog() {
  delete ui;
}
