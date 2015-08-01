#ifndef MESSAGE_WIDGET_H
#define MESSAGE_WIDGET_H

#include <QWidget>

namespace quick_note {

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MessageWidget(QWidget *parent = 0);
  ~MessageWidget();

signals:
  void textChanged(const QString& new_text);

public slots:
  void setText(const QString& new_text);
  void setPixmap(const QPixmap& img);

private slots:
  void on_edit_btn_clicked();

private:
  Ui::MessageWidget *ui;
  QString curr_text_;
};

}  // namespace quick_note

#endif // MESSAGE_WIDGET_H
