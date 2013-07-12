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

  color_ = Qt::red;
  DrawLogo();
  ShowInfo();

  setFixedSize(sizeHint());

  connect(&timer_, SIGNAL(timeout()), this, SLOT(TimoutHandler()));
  timer_.start(15);
}

AboutDialog::~AboutDialog() {
  timer_.stop();
  delete ui;
}

void AboutDialog::TimoutHandler() {
  int r = color_.red();
  int g = color_.green();
  int b = color_.blue();
  int i = 1;

  if (r == 255 && g  < 255 && b ==  0 ) { g += i; }
  if (r  >  0  && g == 255 && b ==  0 ) { r -= i; }
  if (r ==  0  && g == 255 && b  < 255) { b += i; }
  if (r ==  0  && g  >  0  && b == 255) { g -= i; }
  if (r  < 255 && g ==  0  && b == 255) { r += i; }
  if (r == 255 && g ==  0  && b  >  0 ) { b -= i; }

  color_.setRgb(r, g, b);
  DrawLogo();
}

void AboutDialog::DrawLogo() {
  // resize and render svg logo
  QSvgRenderer renderer(QString(":/images/clock_logo.svg"));
  QImage result(64, 64, QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  // aplly color
  QPixmap color_txd(8, 8);
  color_txd.fill(color_);
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.drawTiledPixmap(result.rect(), color_txd);
  painter.end();
//  ui->logo_label->setPixmap(QPixmap::fromImage(result));
}

void AboutDialog::ShowInfo() {
  ui->name_value->setText(QCoreApplication::applicationName());
  ui->version_value->setText(tr("version: ") + QCoreApplication::applicationVersion());
//  ui->qt_value->setText(QT_VERSION_STR);
  ui->rights_value->setText("© 2013 " + QCoreApplication::organizationName());
}
