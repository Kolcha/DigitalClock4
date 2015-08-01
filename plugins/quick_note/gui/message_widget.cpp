#include "message_widget.h"
#include "ui_message_widget.h"

#include <QInputDialog>

namespace quick_note {

MessageWidget::MessageWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MessageWidget)
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

void MessageWidget::on_edit_btn_clicked()
{
  bool ok = false;
  QString str = QInputDialog::getText(ui->msg_label, tr("Edit note"), tr("text:"),
                                      QLineEdit::Normal, curr_text_, &ok);
  if (ok) setText(str);
}

}  // namespace quick_note
