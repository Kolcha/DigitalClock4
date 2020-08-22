/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  SettingsDialog(core::ClockSettings* config, core::ClockState* state, QWidget* parent = nullptr);
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
  void showEvent(QShowEvent* e) override;

private slots:
  void InitControls();

  void ChangePluginState(const QString& name, bool activated);
  void SaveState();
  void LoadState();

  void on_stay_on_top_clicked(bool checked);
  void on_transp_for_input_clicked(bool checked);
  void on_sep_flash_clicked(bool checked);
  void on_opacity_slider_valueChanged(int value);
  void on_zoom_slider_valueChanged(int value);
  void on_txd_per_elem_clicked(bool checked);
  void on_mode_stretch_clicked();
  void on_mode_tile_clicked();
  void on_sel_color_btn_clicked();
  void on_sel_image_btn_clicked();
  void on_type_color_clicked();
  void on_type_image_clicked();
  void on_skin_box_activated(const QString& arg1);
  void on_use_skin_clicked();
  void on_use_font_clicked();
  void on_sel_font_btn_clicked();
  void on_apply_btn_clicked();
  void on_system_format_clicked();
  void on_custom_format_clicked();
  void on_format_box_currentTextChanged(const QString& arg1);
  void on_enable_autoupdate_clicked(bool checked);
  void on_update_period_box_activated(int index);
  void on_check_for_beta_clicked(bool checked);
  void on_space_slider_valueChanged(int arg1);
  void on_export_btn_clicked();
  void on_import_btn_clicked();
  void on_cust_texturing_toggled(bool checked);
  void on_cust_none_clicked();
  void on_cust_texturing_clicked();
  void on_cust_colorize_clicked();
  void on_img_color_btn_clicked();
  void on_level_slider_valueChanged(int value);
  void on_align_left_rbtn_clicked();
  void on_align_center_rbtn_clicked();
  void on_align_right_rbtn_clicked();
  void on_background_enabled_clicked(bool checked);
  void on_background_color_btn_clicked();
  void on_clock_url_enabled_clicked(bool checked);
  void on_clock_url_edit_textEdited(const QString& arg1);
  void on_browse_url_file_btn_clicked();
  void on_show_hide_enable_clicked(bool checked);
  void on_export_state_clicked(bool checked);

  void on_show_in_fullscreen_clicked(bool checked);
  void on_show_on_all_workspaces_clicked(bool checked);
  void on_better_stay_on_top_clicked(bool checked);
  void on_keep_always_visible_clicked(bool checked);
  void on_show_on_all_monitors_clicked(bool checked);
  void on_only_one_instance_clicked(bool checked);
  void on_hover_buttons_enabled_clicked(bool checked);
  void on_transparent_on_hover_clicked(bool checked);
  void on_hide_on_mouse_hover_clicked(bool checked);
  void on_snap_to_edges_clicked(bool checked);
  void on_snap_threshold_valueChanged(int value);
  void on_refresh_interval_valueChanged(int value);

  void on_change_time_zone_cbx_clicked(bool checked);
  void on_time_zone_box_activated(int index);

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
