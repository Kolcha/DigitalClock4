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

  DrawLogo();
  ShowInfo();

  setFixedSize(sizeHint());
}

AboutDialog::~AboutDialog() {
  delete ui;
}

void AboutDialog::DrawLogo() {
  QSvgRenderer renderer(QString(":/images/clock_logo.svg"));
  QImage result(128, 128, QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  painter.end();
  ui->logo_label->setPixmap(QPixmap::fromImage(result));
}

void AboutDialog::ShowInfo() {
  ui->name_value->setText(QCoreApplication::applicationName());
  ui->version_value->setText(QCoreApplication::applicationVersion());
  ui->qt_value->setText(QT_VERSION_STR);
  ui->rights_value->setText("© 2013 " + QCoreApplication::organizationName());
}
