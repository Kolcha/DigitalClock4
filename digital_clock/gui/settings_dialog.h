#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QListWidgetItem>
#include "settings_keys.h"
#include "centered_dialog.h"
#include "../core/plugin_info.h"
#include "../skin/clock_base_skin.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public CenteredDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

signals:
  void OptionChanged(Options opt, const QVariant& value);
  void PluginInfoRequest(const QString& name);
  void PluginConfigureRequest(const QString& text);

public slots:
  void SettingsListener(Options opt, const QVariant& value);
  void SetSkinList(const QStringList& skins);
  void DisplaySkinInfo(const ClockBaseSkin::TSkinInfo& info);
  void SetPluginsList(const QList<QPair<QString,bool> >& plugins);
  void DisplayPluginInfo(const TPluginInfo& info);

protected:
  void changeEvent(QEvent* e);
  void showEvent(QShowEvent* e);

private slots:
  void ChangePluginState(const QString& name, bool activated);
  void SaveState();
  void LoadState();
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
  void on_use_skin_toggled(bool checked);
  void on_use_font_toggled(bool checked);
  void on_sel_font_btn_clicked();
  void on_use_customization_toggled(bool checked);
  void on_apply_btn_clicked();
  void on_system_format_clicked();

private:
  Ui::SettingsDialog* ui;
  QColor last_color_;
  QString last_txd_path_;
  QStringList active_plugins_;
  QFont last_font_;
  int last_customization_;
};

#endif // SETTINGS_DIALOG_H
