#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog {
  Q_OBJECT

public:
  explicit AboutDialog(QWidget *parent = 0);
  ~AboutDialog();

private:
  void ShowInfo();
  Ui::AboutDialog *ui;
};

#endif // ABOUT_DIALOG_H
