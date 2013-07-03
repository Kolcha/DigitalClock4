#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog {
  Q_OBJECT

public:
  explicit AboutDialog(QWidget *parent = 0);
  ~AboutDialog();

private slots:
  void TimoutHandler();

private:
  void DrawLogo();
  void ShowInfo();
  Ui::AboutDialog *ui;
  QTimer timer_;
  QColor color_;
};

#endif // ABOUT_DIALOG_H
