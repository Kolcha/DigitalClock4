#ifndef DIGITAL_CLOCK_GUI_SETTINGS_DIALOG_H
#define DIGITAL_CLOCK_GUI_SETTINGS_DIALOG_H

#include <QDialog>

#include <QListWidgetItem>
#include <QColor>
#include <QFont>

#include "settings_keys.h"

#include "core/plugin_info.h"
#include "skin/clock_base_skin.h"

namespace digital_clock {

namespace core {
class ClockSettings;
class ClockState;
}

namespace gui {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  SettingsDialog(core::ClockSettings* config, core::ClockState* state, QWidget* parent = 0);
  ~SettingsDialog();

signals:
  void OptionChanged(Option opt, const QVariant& value);
  void PluginConfigureRequest(const QString& text);
  void PluginStateChanged(const QString& name, bool enabled);
  void ExportSettings(const QString& filename);
  void ImportSettings(const QString& filename);
  void ResetSettings();
  void CheckForUpdates();

public slots:
  void SetSkinList(const QStringList& skins);
  void DisplaySkinInfo(const core::BaseSkin::TSkinInfo& info);
  void SetPluginsList(const QList<QPair<TPluginInfo, bool> >& plugins);

protected:
  void showEvent(QShowEvent* e);

private slots:
  void InitControls();

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
  void on_use_skin_toggled(bool checked);
  void on_use_font_toggled(bool checked);
  void on_sel_font_btn_clicked();
  void on_apply_btn_clicked();
  void on_system_format_clicked();
  void on_custom_format_clicked();
  void on_enable_autoupdate_toggled(bool checked);
  void on_update_period_box_currentIndexChanged(int index);
  void on_check_for_beta_toggled(bool checked);
  void on_space_slider_valueChanged(int arg1);
  void on_export_btn_clicked();
  void on_import_btn_clicked();
  void on_cust_none_toggled(bool checked);
  void on_cust_texturing_toggled(bool checked);
  void on_cust_colorize_toggled(bool checked);
  void on_img_color_btn_clicked();
  void on_level_slider_valueChanged(int value);
  void on_align_left_rbtn_toggled(bool checked);
  void on_align_right_rbtn_toggled(bool checked);
  void on_background_enabled_toggled(bool checked);
  void on_background_color_btn_clicked();
  void on_clock_url_enabled_toggled(bool checked);
  void on_clock_url_edit_textChanged(const QString &arg1);
  void on_browse_url_file_btn_clicked();

private:
  Ui::SettingsDialog* ui;

  core::ClockSettings* config_;
  core::ClockState* state_;

  QColor last_color_;
  QString last_txd_path_;
  QStringList active_plugins_;
  QFont last_font_;
  QColor last_colorize_color_;
  QColor last_background_color_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_SETTINGS_DIALOG_H
