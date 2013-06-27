#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include "../core/settings_keys.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

signals:
  void OptionChanged(Options opt, const QVariant& value);

public slots:
  void SettingsListener(Options opt, const QVariant& value);
  void SetSkinList(const QStringList& skins);

protected:
  void changeEvent(QEvent* e);

private slots:
  void on_stay_on_top_toggled(bool checked);
  void on_transp_for_input_toggled(bool checked);
  void on_sep_flash_toggled(bool checked);
  void on_opacity_slider_valueChanged(int value);
  void on_zoom_slider_valueChanged(int value);

private:
  Ui::SettingsDialog* ui;
};

#endif // SETTINGS_DIALOG_H
