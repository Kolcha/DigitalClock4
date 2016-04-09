#include "message_widget.h"
#include "ui_message_widget.h"

#include <QInputDialog>
#include <QPainter>
#include <QMouseEvent>

namespace quick_note {

MessageWidget::MessageWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MessageWidget),
  edit_icon_(":/quick_note/edit.svg")
{
  ui->setupUi(this);
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
  if (ok) setText(str);
}

void MessageWidget::ColorizeIcon(const QColor& color)
{
  int h = 0.72 * ui->msg_label->pixmap()->height();
  QSize size = edit_icon_.actualSize(QSize(h, h));

  QImage result(size, QImage::Format_ARGB32_Premultiplied);
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
