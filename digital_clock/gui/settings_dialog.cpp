/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <algorithm>

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QFileInfo>
#include <QTimeZone>

#include "skin_drawer.h"
#include "settings_storage.h"

#include "platform/autostart.h"
#include "core/clock_settings.h"
#include "core/clock_state.h"

#include "gui/plugin_list_item_widget.h"


#define S_OPT_LAST_TIME_FORMAT_KEY        "last_time_format"
#define S_OPT_GEOMETRY_KEY                "settings_dialog_geometry"

#ifdef Q_OS_MACOS
#define DEFAULT_TEXTURE_PATH              (QApplication::applicationDirPath() + "/../Resources/textures")
#else
#define DEFAULT_TEXTURE_PATH              (QApplication::applicationDirPath() + "/textures")
#endif

using skin_draw::SkinDrawer;

namespace digital_clock {
namespace gui {

static bool plugin_info_cmp(const QPair<TPluginInfo, bool>& a, const QPair<TPluginInfo, bool>& b)
{
  return QString::localeAwareCompare(a.first.gui_info.display_name, b.first.gui_info.display_name) < 0;
}


SettingsDialog::SettingsDialog(core::ClockSettings* config, core::ClockState* state, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog),
  config_(config), state_(state)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  ui->skin_info_btn->setVisible(false);  // temporary, not implemented
  ui->defaults_bth->setVisible(false);   // temporary, not implemented
#ifndef Q_OS_WIN
  ui->fullscreen_detect->setVisible(false);   // supported only on Windows
#endif
#ifndef Q_OS_LINUX
  ui->better_stay_on_top->setVisible(false);  // supported only on Linux
#endif
#if !defined(Q_OS_LINUX) && !defined(Q_OS_MACOS)
  ui->show_on_all_workspaces->setVisible(false);  // supported on Linux/Mac
#endif

  connect(config->GetBackend(), &SettingsStorage::reloaded, this, &SettingsDialog::InitControls);

  InitControls();
  resize(minimumSizeHint());

  connect(this, SIGNAL(accepted()), this, SLOT(SaveState()));
  connect(ui->defaults_bth, SIGNAL(clicked()), this, SIGNAL(ResetSettings()));

  ui->autostart->setChecked(IsAutoStartEnabled());      // TODO: remove
  connect(this, &SettingsDialog::accepted, [this] () {
    SetAutoStart(this->ui->autostart->isChecked());
  });

  connect(ui->check_upd_now_btn, SIGNAL(clicked(bool)), this, SIGNAL(CheckForUpdates()));
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::SetSkinList(const QStringList& skins)
{
  ui->skin_box->clear();
  ui->skin_box->addItems(skins);
  ui->skin_box->setCurrentText(config_->GetValue(OPT_SKIN_NAME).toString());
}

void SettingsDialog::DisplaySkinInfo(const core::BaseSkin::TSkinInfo& info)
{
  using core::BaseSkin;
  if (info[BaseSkin::SI_NAME] == "Text Skin") return;
  BaseSkin::TSkinInfo l_info = info;
  for (auto iter = l_info.begin(); iter != l_info.end(); ++iter) {
    if (iter.value().isEmpty()) iter.value() = tr("unknown");
  }
  ui->skin_box->setCurrentText(l_info[BaseSkin::SI_NAME]);
  ui->skin_box->setToolTip(l_info[BaseSkin::SI_COMMENT]);
}

void SettingsDialog::SetPluginsList(const QList<QPair<TPluginInfo, bool> >& plugins)
{
  ui->plugins_list->clear();
  QList<QPair<TPluginInfo, bool> > sorted_plugins = plugins;
  std::sort(sorted_plugins.begin(), sorted_plugins.end(), plugin_info_cmp);
  for (auto& plugin : sorted_plugins) {
    QListWidgetItem* item = new QListWidgetItem();
    PluginListItemWidget* widget = new PluginListItemWidget(ui->plugins_list);
    widget->SetInfo(plugin.first);
    widget->SetConfigurable(plugin.second);
    widget->SetChecked(active_plugins_.contains(widget->GetName()));
    item->setSizeHint(widget->sizeHint());
    ui->plugins_list->addItem(item);
    ui->plugins_list->setItemWidget(item, widget);
    connect(widget, SIGNAL(StateChanged(QString,bool)),
            this, SIGNAL(PluginStateChanged(QString,bool)));
    connect(widget, SIGNAL(StateChanged(QString,bool)),
            this, SLOT(ChangePluginState(QString,bool)));
    connect(widget, SIGNAL(ConfigureRequested(QString)),
            this, SIGNAL(PluginConfigureRequest(QString)));
  }
}

void SettingsDialog::showEvent(QShowEvent* e)
{
  QDialog::showEvent(e);
  LoadState();
}

void SettingsDialog::InitControls()
{
  Q_ASSERT(config_);
  // "Appearance" tab
  ui->stay_on_top->setChecked(config_->GetValue(OPT_STAY_ON_TOP).toBool());
  ui->transp_for_input->setChecked(config_->GetValue(OPT_TRANSP_FOR_INPUT).toBool());
  ui->sep_flash->setChecked(config_->GetValue(OPT_SEPARATOR_FLASH).toBool());

  ui->opacity_slider->setValue(100 * config_->GetValue(OPT_OPACITY).toReal());
  ui->zoom_slider->setValue(100 * config_->GetValue(OPT_ZOOM).toReal());

  QString skin_name = config_->GetValue(OPT_SKIN_NAME).toString();
  ui->use_skin->setChecked(skin_name != "Text Skin");
  // TODO: load all available skins and fill combobox, set combobox text in ANY case
  if (ui->use_skin->isChecked()) ui->skin_box->setCurrentText(skin_name);
  ui->use_font->setChecked(skin_name == "Text Skin");
  last_font_ = config_->GetValue(OPT_FONT).value<QFont>();

  Customization cust = static_cast<Customization>(config_->GetValue(OPT_CUSTOMIZATION).toInt());
  ui->cust_none->setChecked(cust == Customization::C_NONE);
  ui->cust_texturing->setChecked(cust == Customization::C_TEXTURING);
  ui->cust_colorize->setChecked(cust == Customization::C_COLORIZE);

  SkinDrawer::CustomizationType txd_type = config_->GetValue(OPT_TEXTURE_TYPE).value<SkinDrawer::CustomizationType>();
  ui->type_color->setChecked(txd_type == SkinDrawer::CT_COLOR);
  ui->type_image->setChecked(txd_type == SkinDrawer::CT_TEXTURE);
  last_color_ = config_->GetValue(OPT_COLOR).value<QColor>();
  QString texture = config_->GetValue(OPT_TEXTURE).toString();
  last_txd_path_ = texture.isEmpty() ? DEFAULT_TEXTURE_PATH : QFileInfo(texture).absolutePath();

  ui->txd_per_elem->setChecked(config_->GetValue(OPT_TEXTURE_PER_ELEMENT).toBool());
  SkinDrawer::DrawMode mode = config_->GetValue(OPT_TEXTURE_DRAW_MODE).value<SkinDrawer::DrawMode>();
  ui->mode_stretch->setChecked(mode == SkinDrawer::DM_STRETCH);
  ui->mode_tile->setChecked(mode == SkinDrawer::DM_TILE);

  ui->background_enabled->setChecked(config_->GetValue(OPT_BACKGROUND_ENABLED).toBool());
  last_background_color_ = config_->GetValue(OPT_BACKGROUND_COLOR).value<QColor>();
  ui->background_color_btn->setColor(last_background_color_);

  last_colorize_color_ = config_->GetValue(OPT_COLORIZE_COLOR).value<QColor>();
  ui->img_color_btn->setColor(last_colorize_color_);
  ui->level_slider->setValue(100 * config_->GetValue(OPT_COLORIZE_LEVEL).toReal());

  // "Misc" tab
  ui->autostart->setChecked(IsAutoStartEnabled());
  ui->enable_autoupdate->setChecked(config_->GetValue(OPT_USE_AUTOUPDATE).toBool());

  QString time_format = config_->GetValue(OPT_TIME_FORMAT).toString();
  bool is_system = time_format.isEmpty();
  if (!is_system) ui->format_box->setCurrentText(time_format);
  ui->system_format->setChecked(is_system);
  ui->custom_format->setChecked(!is_system);

  CAlignment alignment = static_cast<CAlignment>(config_->GetValue(OPT_ALIGNMENT).toInt());
  ui->align_left_rbtn->setChecked(alignment == CAlignment::A_LEFT);
  ui->align_center_rbtn->setChecked(alignment == CAlignment::A_CENTER);
  ui->align_right_rbtn->setChecked(alignment == CAlignment::A_RIGHT);

  int spacing = config_->GetValue(OPT_SPACING).toInt();
  ui->space_slider->setValue(spacing);
  ui->space_slider->setToolTip(QString::number(spacing));

  ui->update_period_box->clear();
  ui->update_period_box->addItem(tr("1 day"), 1);
  ui->update_period_box->addItem(tr("3 days"), 3);
  ui->update_period_box->addItem(tr("1 week"), 7);
  ui->update_period_box->addItem(tr("2 weeks"), 14);
  int c_index = ui->update_period_box->findData(config_->GetValue(OPT_UPDATE_PERIOD).toInt());
  ui->update_period_box->setCurrentIndex(c_index);
  ui->check_for_beta->setChecked(config_->GetValue(OPT_CHECK_FOR_BETA).toBool());

  ui->clock_url_enabled->setChecked(config_->GetValue(OPT_CLOCK_URL_ENABLED).toBool());
  ui->clock_url_edit->setText(config_->GetValue(OPT_CLOCK_URL_STRING).toString());

  ui->show_hide_enable->setChecked(config_->GetValue(OPT_SHOW_HIDE_ENABLED).toBool());
  ui->export_state->setChecked(config_->GetValue(OPT_EXPORT_STATE).toBool());

  ui->change_time_zone_cbx->setChecked(!config_->GetValue(OPT_DISPLAY_LOCAL_TIME).toBool());
  ui->time_zone_box->clear();
  for (auto& tz : QTimeZone::availableTimeZoneIds())
    ui->time_zone_box->addItem(QString(tz), tz);
  ui->time_zone_box->setCurrentText(QString(config_->GetValue(OPT_TIME_ZONE).toByteArray()));

  // "Plugins" tab
  active_plugins_ = config_->GetValue(OPT_PLUGINS).toStringList();
  for (int i = 0; i < ui->plugins_list->count(); i++) {
    QListWidgetItem* item = ui->plugins_list->item(i);
    PluginListItemWidget* widget = qobject_cast<PluginListItemWidget*>(ui->plugins_list->itemWidget(item));
    Q_ASSERT(widget);
    widget->blockSignals(true);
    widget->SetChecked(active_plugins_.contains(widget->GetName()));
    widget->blockSignals(false);
  }

  // "Experimental" tab
  ui->fullscreen_detect->setChecked(config_->GetValue(OPT_FULLSCREEN_DETECT).toBool());
  ui->show_on_all_workspaces->setChecked(config_->GetValue(OPT_SHOW_ON_ALL_DESKTOPS).toBool());
  ui->better_stay_on_top->setChecked(config_->GetValue(OPT_BETTER_STAY_ON_TOP).toBool());
  ui->keep_always_visible->setChecked(config_->GetValue(OPT_KEEP_ALWAYS_VISIBLE).toBool());
}

void SettingsDialog::ChangePluginState(const QString& name, bool activated)
{
  if (activated)
    active_plugins_.append(name);
  else
    active_plugins_.removeOne(name);
  emit OptionChanged(OPT_PLUGINS, active_plugins_);
}

void SettingsDialog::SaveState()
{
  state_->SetVariable(S_OPT_LAST_TIME_FORMAT_KEY, ui->format_box->currentText());
  state_->SetVariable(S_OPT_GEOMETRY_KEY, saveGeometry());
}

void SettingsDialog::LoadState()
{
  QVariant last_format = state_->GetVariable(S_OPT_LAST_TIME_FORMAT_KEY);
  if (last_format.isValid()) ui->format_box->setCurrentText(last_format.toString());
  QVariant last_geometry = state_->GetVariable(S_OPT_GEOMETRY_KEY);
  if (last_geometry.isValid()) restoreGeometry(last_geometry.toByteArray());
}

void SettingsDialog::on_stay_on_top_clicked(bool checked)
{
  emit OptionChanged(OPT_STAY_ON_TOP, checked);
}

void SettingsDialog::on_transp_for_input_clicked(bool checked)
{
  emit OptionChanged(OPT_TRANSP_FOR_INPUT, checked);
}

void SettingsDialog::on_sep_flash_clicked(bool checked)
{
  emit OptionChanged(OPT_SEPARATOR_FLASH, checked);
}

void SettingsDialog::on_opacity_slider_valueChanged(int value)
{
  emit OptionChanged(OPT_OPACITY, value / 100.);
}

void SettingsDialog::on_zoom_slider_valueChanged(int value)
{
  emit OptionChanged(OPT_ZOOM, value / 100.);
}

void SettingsDialog::on_txd_per_elem_clicked(bool checked)
{
  emit OptionChanged(OPT_TEXTURE_PER_ELEMENT, checked);
}

void SettingsDialog::on_mode_stretch_clicked()
{
  emit OptionChanged(OPT_TEXTURE_DRAW_MODE, SkinDrawer::DM_STRETCH);
}

void SettingsDialog::on_mode_tile_clicked()
{
  emit OptionChanged(OPT_TEXTURE_DRAW_MODE, SkinDrawer::DM_TILE);
}

void SettingsDialog::on_sel_color_btn_clicked()
{
  QColor color = QColorDialog::getColor(last_color_, this);
  if (color.isValid()) {
    emit OptionChanged(OPT_COLOR, color);
    last_color_ = color;
  }
}

void SettingsDialog::on_sel_image_btn_clicked()
{
  QString texture = QFileDialog::getOpenFileName(this, tr("Open texture file"),
                    last_txd_path_,
                    tr("Images (*.bmp *.jpg *.jpeg *.png *.tiff *.xbm *.xpm)"));
  if (!texture.isEmpty()) {
    emit OptionChanged(OPT_TEXTURE, texture);
    if (last_txd_path_ == DEFAULT_TEXTURE_PATH) {
      emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_TEXTURE);
    }
    last_txd_path_ = QFileInfo(texture).absolutePath();
  }
}

void SettingsDialog::on_type_color_clicked()
{
  emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_COLOR);
}

void SettingsDialog::on_type_image_clicked()
{
  emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_TEXTURE);
}

void SettingsDialog::on_skin_box_activated(const QString& arg1)
{
  if (!arg1.isEmpty()) emit OptionChanged(OPT_SKIN_NAME, arg1);
}

void SettingsDialog::on_use_skin_clicked()
{
  emit OptionChanged(OPT_SKIN_NAME, ui->skin_box->currentText());
}

void SettingsDialog::on_use_font_clicked()
{
  emit OptionChanged(OPT_SKIN_NAME, "Text Skin");
}

void SettingsDialog::on_sel_font_btn_clicked()
{
  bool ok = false;
  QFont font = QFontDialog::getFont(&ok, last_font_, this);
  if (ok) {
    emit OptionChanged(OPT_FONT, font);
    last_font_ = font;
  }
}

void SettingsDialog::on_apply_btn_clicked()
{
  emit OptionChanged(OPT_TIME_FORMAT, ui->format_box->currentText());
}

void SettingsDialog::on_system_format_clicked()
{
  emit OptionChanged(OPT_TIME_FORMAT, QString());
}

void SettingsDialog::on_custom_format_clicked()
{
  emit OptionChanged(OPT_TIME_FORMAT, ui->format_box->currentText());
}

void SettingsDialog::on_enable_autoupdate_clicked(bool checked)
{
  emit OptionChanged(OPT_USE_AUTOUPDATE, checked);
}

void SettingsDialog::on_update_period_box_activated(int index)
{
  emit OptionChanged(OPT_UPDATE_PERIOD, ui->update_period_box->itemData(index));
}

void SettingsDialog::on_check_for_beta_clicked(bool checked)
{
  emit OptionChanged(OPT_CHECK_FOR_BETA, checked);
}

void SettingsDialog::on_space_slider_valueChanged(int arg1)
{
  ui->space_slider->setToolTip(QString::number(arg1));
  emit OptionChanged(OPT_SPACING, arg1);
}

void SettingsDialog::on_export_btn_clicked()
{
  QString filename = QFileDialog::getSaveFileName(this, tr("Export settings to ..."),
                     QDir::home().absoluteFilePath("clock_settings.dcs"),
                     tr("Digital Clock settings files (*.dcs)"));
  if (!filename.isEmpty()) emit ExportSettings(filename);
}

void SettingsDialog::on_import_btn_clicked()
{
  QString filename = QFileDialog::getOpenFileName(this, tr("Import settings from ..."),
                     QDir::home().absoluteFilePath("clock_settings.dcs"),
                     tr("Digital Clock settings files (*.dcs)"));
  if (!filename.isEmpty()) emit ImportSettings(filename);
}

} // namespace gui
} // namespace digital_clock

void digital_clock::gui::SettingsDialog::on_cust_texturing_toggled(bool checked)
{
  ui->image_group->setEnabled(checked ? ui->type_image->isChecked() : false);
}

void digital_clock::gui::SettingsDialog::on_cust_none_clicked()
{
  ui->image_group->setDisabled(true);
  emit OptionChanged(OPT_CUSTOMIZATION, static_cast<int>(Customization::C_NONE));
}

void digital_clock::gui::SettingsDialog::on_cust_texturing_clicked()
{
  emit OptionChanged(OPT_CUSTOMIZATION, static_cast<int>(Customization::C_TEXTURING));
  if (ui->type_color->isChecked()) emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_COLOR);
  if (ui->type_image->isChecked()) emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_TEXTURE);
}

void digital_clock::gui::SettingsDialog::on_cust_colorize_clicked()
{
  ui->image_group->setDisabled(true);
  emit OptionChanged(OPT_CUSTOMIZATION, static_cast<int>(Customization::C_COLORIZE));
}

void digital_clock::gui::SettingsDialog::on_img_color_btn_clicked()
{
  QColor color = QColorDialog::getColor(last_colorize_color_, this);
  if (color.isValid()) {
    ui->img_color_btn->setColor(color);
    emit OptionChanged(OPT_COLORIZE_COLOR, color);
    last_colorize_color_ = color;
  }
}

void digital_clock::gui::SettingsDialog::on_level_slider_valueChanged(int value)
{
  emit OptionChanged(OPT_COLORIZE_LEVEL, value / 100.);
}

void digital_clock::gui::SettingsDialog::on_align_left_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(CAlignment::A_LEFT));
}

void digital_clock::gui::SettingsDialog::on_align_center_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(CAlignment::A_CENTER));
}

void digital_clock::gui::SettingsDialog::on_align_right_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(CAlignment::A_RIGHT));
}

void digital_clock::gui::SettingsDialog::on_background_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_BACKGROUND_ENABLED, checked);
}

void digital_clock::gui::SettingsDialog::on_background_color_btn_clicked()
{
  QColor color = QColorDialog::getColor(last_background_color_, this);
  if (color.isValid()) {
    ui->background_color_btn->setColor(color);
    emit OptionChanged(OPT_BACKGROUND_COLOR, color);
    last_background_color_ = color;
  }
}

void digital_clock::gui::SettingsDialog::on_clock_url_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_CLOCK_URL_ENABLED, checked);
}

void digital_clock::gui::SettingsDialog::on_clock_url_edit_textEdited(const QString& arg1)
{
  emit OptionChanged(OPT_CLOCK_URL_STRING, arg1);
}

void digital_clock::gui::SettingsDialog::on_browse_url_file_btn_clicked()
{
  QUrl url = QFileDialog::getOpenFileUrl(this);
  if (url.isValid()) ui->clock_url_edit->setText(url.toString());
}

void digital_clock::gui::SettingsDialog::on_show_hide_enable_clicked(bool checked)
{
  emit OptionChanged(OPT_SHOW_HIDE_ENABLED, checked);
}

void digital_clock::gui::SettingsDialog::on_export_state_clicked(bool checked)
{
  emit OptionChanged(OPT_EXPORT_STATE, checked);
}

void digital_clock::gui::SettingsDialog::on_fullscreen_detect_clicked(bool checked)
{
  emit OptionChanged(OPT_FULLSCREEN_DETECT, checked);
}

void digital_clock::gui::SettingsDialog::on_show_on_all_workspaces_clicked(bool checked)
{
  emit OptionChanged(OPT_SHOW_ON_ALL_DESKTOPS, checked);
}

void digital_clock::gui::SettingsDialog::on_better_stay_on_top_clicked(bool checked)
{
  emit OptionChanged(OPT_BETTER_STAY_ON_TOP, checked);
}

void digital_clock::gui::SettingsDialog::on_change_time_zone_cbx_clicked(bool checked)
{
  emit OptionChanged(OPT_DISPLAY_LOCAL_TIME, !checked);
}

void digital_clock::gui::SettingsDialog::on_keep_always_visible_clicked(bool checked)
{
  emit OptionChanged(OPT_KEEP_ALWAYS_VISIBLE, checked);
}

void digital_clock::gui::SettingsDialog::on_time_zone_box_activated(int index)
{
  emit OptionChanged(OPT_TIME_ZONE, ui->time_zone_box->itemData(index, Qt::UserRole).toByteArray());
}
