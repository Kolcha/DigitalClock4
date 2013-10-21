#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}
// TODO: inherit from CenteredDialog
class AboutDialog : public QDialog {
  Q_OBJECT

public:
  explicit AboutDialog(QWidget *parent = 0);
  ~AboutDialog();

protected:
  void showEvent(QShowEvent* e);

private:
  Ui::AboutDialog *ui;
};

#endif // ABOUT_DIALOG_H
