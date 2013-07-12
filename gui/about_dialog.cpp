#include <QSvgRenderer>
#include <QPainter>
#include <QCoreApplication>
#include "about_dialog.h"
#include "ui_about_dialog.h"

AboutDialog::AboutDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/images/about.svg"));

  ShowInfo();

  setFixedSize(sizeHint());
}

AboutDialog::~AboutDialog() {
  delete ui;
}

void AboutDialog::ShowInfo() {
  ui->name_value->setText(QCoreApplication::applicationName());
  ui->version_value->setText(tr("version: ") + QCoreApplication::applicationVersion());
//  ui->qt_value->setText(QT_VERSION_STR);
  ui->rights_value->setText("© 2013 " + QCoreApplication::organizationName());
}
