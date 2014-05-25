#ifndef DIGITAL_CLOCK_GUI_SETTINGS_DIALOG_H
#define DIGITAL_CLOCK_GUI_SETTINGS_DIALOG_H

#include <QListWidgetItem>
#include <QDir>
#include "settings_keys.h"
#include "centered_dialog.h"
#include "../core/plugin_info.h"
#include "../skin/clock_base_skin.h"

namespace digital_clock {
namespace gui {

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
  void PluginConfigureRequest(const QString& text);
  void PluginStateChanged(const QString& name, bool enabled);
  void ExportSettings(const QString& filename);
  void ImportSettings(const QString& filename);
  void ResetSettings();

public slots:
  void SettingsListener(Options opt, const QVariant& value);
  void SetSkinList(const QStringList& skins);
  void DisplaySkinInfo(const ::digital_clock::core::ClockBaseSkin::TSkinInfo& info);
  void SetPluginsList(const QList<QPair<TPluginInfo, bool> >& plugins);

protected:
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
  void on_use_skin_toggled(bool checked);
  void on_use_font_toggled(bool checked);
  void on_sel_font_btn_clicked();
  void on_use_customization_toggled(bool checked);
  void on_apply_btn_clicked();
  void on_system_format_clicked();
  void on_enable_autoupdate_toggled(bool checked);
  void on_update_period_box_currentIndexChanged(int index);
  void on_check_for_beta_toggled(bool checked);
  void on_space_value_valueChanged(int arg1);
  void on_export_btn_clicked();
  void on_import_btn_clicked();
  void on_add_sp_btn_clicked();
  void on_del_sp_btn_clicked();
  void on_add_pp_btn_clicked();
  void on_del_pp_btn_clicked();

private:
  Ui::SettingsDialog* ui;
  QColor last_color_;
  QString last_txd_path_;
  QStringList active_plugins_;
  QFont last_font_;
  int last_customization_;
  QMap<qint64, QString> update_periods_;
  QStringList skins_paths_;
  QStringList plugins_paths_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_SETTINGS_DIALOG_H
