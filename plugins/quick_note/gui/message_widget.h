#ifndef MESSAGE_WIDGET_H
#define MESSAGE_WIDGET_H

#include <QWidget>
#include <QIcon>

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

  void ColorizeIcon(const QColor& color);

signals:
  void textChanged(const QString& new_text);

public slots:
  void setText(const QString& new_text);
  void setPixmap(const QPixmap& img);

protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private slots:
  void on_edit_btn_clicked();

private:
  Ui::MessageWidget *ui;
  QString curr_text_;
  QIcon edit_icon_;
  QPoint drag_position_;
};

}  // namespace quick_note

#endif // MESSAGE_WIDGET_H
