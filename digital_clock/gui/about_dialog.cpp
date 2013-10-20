#include <QCoreApplication>
#include "about_dialog.h"
#include "ui_about_dialog.h"

AboutDialog::AboutDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/images/about.svg"));

  ui->name_value->setText(QCoreApplication::applicationName());
  ui->version_value->setText(tr("version: ") + QCoreApplication::applicationVersion());
  ui->rights_value->setText("© 2013 " + QCoreApplication::organizationName());
  ui->qt_value->setText(QString("Qt ") + QT_VERSION_STR);

  setFixedSize(sizeHint());
}

AboutDialog::~AboutDialog() {
  delete ui;
}
