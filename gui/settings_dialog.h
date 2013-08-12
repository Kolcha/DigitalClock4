#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "../core/settings_keys.h"
#include "../core/iclock_plugin.h"
#include "../skin/iclock_skin.h"

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
  void PluginInfoRequest(const QString& name);

public slots:
  void SettingsListener(Options opt, const QVariant& value);
  void SetSkinList(const QStringList& skins);
  void DisplaySkinInfo(const TSkinInfo& info);
  void SetPluginsList(const QList<QPair<QString,bool> >& plugins);
  void DisplayPluginInfo(const TPluginInfo& info);

protected:
  void changeEvent(QEvent* e);

private slots:
  void ChangePluginState(const QString& name, bool activated);
  void on_stay_on_top_toggled(bool checked);
  void on_transp_for_input_toggled(bool checked);
  void on_sep_flash_toggled(bool checked);
  void on_opacity_slider_valueChanged(int value);
  void on_zoom_slider_valueChanged(int value);
  void on_txd_per_elem_toggled(bool checked);
  void on_mode_stretch_toggled(bool checked);
  void on_mode_tile_toggled(bool checked);
  void on_sel_color_btn_clicked();
  void on_sel_image_btn_clicked();
  void on_type_color_toggled(bool checked);
  void on_type_image_toggled(bool checked);
  void on_skin_box_currentIndexChanged(const QString& arg1);
  void on_plugins_list_currentItemChanged(QListWidgetItem* current, QListWidgetItem*);

private:
  Ui::SettingsDialog* ui;
  QColor last_color_;
  QString last_txd_path_;
  QStringList active_plugins_;
};

#endif // SETTINGS_DIALOG_H
