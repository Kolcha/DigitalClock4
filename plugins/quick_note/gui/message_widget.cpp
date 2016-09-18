#include "message_widget.h"
#include "ui_message_widget.h"

#include <QInputDialog>
#include <QPainter>
#include <QMouseEvent>

namespace quick_note {

MessageWidget::MessageWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::MessageWidget),
  edit_icon_(":/quick_note/edit.svg")
{
  ui->setupUi(this);
  layout()->setMargin(0);
  adjustSize();
  icon_color_ = Qt::black;
}

MessageWidget::~MessageWidget()
{
  delete ui;
}

void MessageWidget::setText(const QString& new_text)
{
  curr_text_ = new_text;
  emit textChanged(new_text);
}

void MessageWidget::setPixmap(const QPixmap& img)
{
  ui->msg_label->setPixmap(img);
  ColorizeIcon(icon_color_);
}

void MessageWidget::setIconColor(const QColor& new_color)
{
  icon_color_ = new_color;
  if (!ui->msg_label->pixmap() || ui->msg_label->pixmap()->isNull()) return;
  ColorizeIcon(new_color);
}

void MessageWidget::ShowEditButton(bool show)
{
  ui->edit_btn->setVisible(show);
}

void MessageWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    drag_position_ = event->globalPos();
  }
  event->ignore();
}

void MessageWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton &&
      event->globalPos() == drag_position_) {
    ui->edit_btn->click();
  }
  event->ignore();
}

void MessageWidget::on_edit_btn_clicked()
{
  bool ok = false;
  QString str = QInputDialog::getText(ui->msg_label, tr("Edit note"), tr("text:"),
                                      QLineEdit::Normal, curr_text_, &ok);
  if (ok) {
    setText(str);
    emit textEdited(str);
  }
}

void MessageWidget::ColorizeIcon(const QColor& color)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
  qreal dpr = ui->msg_label->devicePixelRatioF();
#else
  int dpr = ui->msg_label->devicePixelRatio();
#endif
  int h = 0.72 * ui->msg_label->pixmap()->height() / dpr;
  QSize size = edit_icon_.actualSize(QSize(h, h));

  QImage result(size * dpr, QImage::Format_ARGB32_Premultiplied);
  result.setDevicePixelRatio(dpr);
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  painter.drawPixmap(0, 0, edit_icon_.pixmap(size));

  QPixmap texture(2, 2);
  texture.fill(color);
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.drawTiledPixmap(result.rect(), texture);
  painter.end();

  ui->edit_btn->setIconSize(QSize(h, h));
  ui->edit_btn->setIcon(QPixmap::fromImage(result));
}

}  // namespace quick_note
