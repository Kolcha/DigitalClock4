#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QFileInfo>
#include <QSettings>

#include "skin_drawer.h"
#include "settings_storage.h"

#include "core/autostart.h"
#include "core/clock_settings.h"

#include "gui/plugin_list_item_widget.h"


#define S_OPT_LAST_TIME_FORMAT_KEY        "state/last_time_format"
#define S_OPT_GEOMETRY_KEY                "state/settings_dialog_geometry"

#ifdef Q_OS_OSX
#include <QApplication>
#define DEFAULT_TEXTURE_PATH              (QApplication::applicationDirPath() + "/../Resources/textures")
#else
#define DEFAULT_TEXTURE_PATH              "textures"
#endif

using skin_draw::SkinDrawer;

namespace digital_clock {
namespace gui {

SettingsDialog::SettingsDialog(core::ClockSettings* config, QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog),
  config_(config)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/clock/images/settings.svg"));

  ui->skin_info_btn->setVisible(false);  // temporary, not implemented

  connect(config->GetBackend(), &SettingsStorage::reloaded, this, &SettingsDialog::InitControls);

  InitControls();



  connect(this, SIGNAL(accepted()), this, SLOT(SaveState()));
  connect(ui->defaults_bth, SIGNAL(clicked()), this, SIGNAL(ResetSettings()));

  ui->autostart->setChecked(IsAutoStartEnabled());      // TODO: remove
  connect(this, &SettingsDialog::accepted, [this] () {
    SetAutoStart(this->ui->autostart->isChecked());
  });

  connect(ui->check_upd_now_btn, SIGNAL(clicked(bool)), this, SIGNAL(CheckForUpdates()));
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SetSkinList(const QStringList& skins) {
  this->blockSignals(true);
  ui->skin_box->clear();
  ui->skin_box->addItems(skins);
  ui->skin_box->setCurrentText(config_->GetValue(OPT_SKIN_NAME).toString());
  this->blockSignals(false);
}

void SettingsDialog::DisplaySkinInfo(const core::BaseSkin::TSkinInfo& info) {
  using core::BaseSkin;
  if (info[BaseSkin::SI_NAME] == "Text Skin") return;
  BaseSkin::TSkinInfo l_info = info;
  for (auto iter = l_info.begin(); iter != l_info.end(); ++iter) {
    if (iter.value().isEmpty()) iter.value() = tr("unknown");
  }
  ui->skin_box->setCurrentText(l_info[BaseSkin::SI_NAME]);
  ui->skin_box->setToolTip(l_info[BaseSkin::SI_COMMENT]);
}

void SettingsDialog::SetPluginsList(const QList<QPair<TPluginInfo, bool> >& plugins) {
  ui->plugins_list->clear();
  for (auto& plugin : plugins) {
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

void SettingsDialog::showEvent(QShowEvent* e) {
  QDialog::showEvent(e);
  LoadState();
}

void SettingsDialog::InitControls()
{
  Q_ASSERT(config_);
  this->blockSignals(true);

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

  SkinDrawer::CustomizationType txd_type = static_cast<SkinDrawer::CustomizationType>(config_->GetValue(OPT_TEXTURE_TYPE).toInt());
  ui->type_color->setChecked(txd_type == SkinDrawer::CT_COLOR);
  ui->type_image->setChecked(txd_type == SkinDrawer::CT_TEXTURE);
  last_color_ = config_->GetValue(OPT_COLOR).value<QColor>();
  QString texture = config_->GetValue(OPT_TEXTURE).toString();
  last_txd_path_ = texture.isEmpty() ? DEFAULT_TEXTURE_PATH : QFileInfo(texture).absolutePath();

  ui->txd_per_elem->setChecked(config_->GetValue(OPT_TEXTURE_PER_ELEMENT).toBool());
  SkinDrawer::DrawMode mode = static_cast<SkinDrawer::DrawMode>(config_->GetValue(OPT_TEXTURE_DRAW_MODE).toInt());
  ui->mode_stretch->setChecked(mode == SkinDrawer::DM_STRETCH);
  ui->mode_tile->setChecked(mode == SkinDrawer::DM_TILE);

  last_colorize_color_ = config_->GetValue(OPT_COLORIZE_COLOR).value<QColor>();
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
  ui->align_right_rbtn->setChecked(alignment == CAlignment::A_RIGHT);

  int spacing = config_->GetValue(OPT_SPACING).toInt();
  ui->space_slider->setValue(spacing);
  ui->space_slider->setToolTip(QString::number(spacing));

  ui->update_period_box->clear();
  // TODO: remove 'for' and 'update_periods_'
  QMap<qint64, QString> update_periods;
  update_periods[1] = tr("1 day");
  update_periods[3] = tr("3 days");
  update_periods[7] = tr("1 week");
  update_periods[14] = tr("2 weeks");
  for (auto iter = update_periods.begin(); iter != update_periods.end(); ++iter) {
    ui->update_period_box->addItem(iter.value(), iter.key());
  }
  ui->update_period_box->setCurrentText(update_periods[config_->GetValue(OPT_UPDATE_PERIOD).toInt()]);
  ui->check_for_beta->setChecked(config_->GetValue(OPT_CHECK_FOR_BETA).toBool());

  // "Plugins" tab
  active_plugins_ = config_->GetValue(OPT_PLUGINS).toStringList();

  this->blockSignals(false);
}

void SettingsDialog::ChangePluginState(const QString& name, bool activated) {
  if (activated)
    active_plugins_.append(name);
  else
    active_plugins_.removeOne(name);
  emit OptionChanged(OPT_PLUGINS, active_plugins_);
}

void SettingsDialog::SaveState() {
  QSettings settings;
  settings.setValue(S_OPT_LAST_TIME_FORMAT_KEY, ui->format_box->currentText());
  settings.setValue(S_OPT_GEOMETRY_KEY, saveGeometry());
}

void SettingsDialog::LoadState() {
  QSettings state;
  QVariant last_format = state.value(S_OPT_LAST_TIME_FORMAT_KEY);
  if (last_format.isValid()) ui->format_box->setCurrentText(last_format.toString());
  QVariant last_geometry = state.value(S_OPT_GEOMETRY_KEY);
  if (last_geometry.isValid()) restoreGeometry(last_geometry.toByteArray());
}

void SettingsDialog::on_stay_on_top_toggled(bool checked) {
  emit OptionChanged(OPT_STAY_ON_TOP, checked);
}

void SettingsDialog::on_transp_for_input_toggled(bool checked) {
  emit OptionChanged(OPT_TRANSP_FOR_INPUT, checked);
}

void SettingsDialog::on_sep_flash_toggled(bool checked) {
  emit OptionChanged(OPT_SEPARATOR_FLASH, checked);
}

void SettingsDialog::on_opacity_slider_valueChanged(int value) {
  emit OptionChanged(OPT_OPACITY, value / 100.);
}

void SettingsDialog::on_zoom_slider_valueChanged(int value) {
  emit OptionChanged(OPT_ZOOM, value / 100.);
}

void SettingsDialog::on_txd_per_elem_toggled(bool checked) {
  emit OptionChanged(OPT_TEXTURE_PER_ELEMENT, checked);
}

void SettingsDialog::on_mode_stretch_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_TEXTURE_DRAW_MODE, SkinDrawer::DM_STRETCH);
}

void SettingsDialog::on_mode_tile_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_TEXTURE_DRAW_MODE, SkinDrawer::DM_TILE);
}

void SettingsDialog::on_sel_color_btn_clicked() {
  QColor color = QColorDialog::getColor(last_color_, this);
  if (color.isValid()) {
    emit OptionChanged(OPT_COLOR, color);
    last_color_ = color;
  }
}

void SettingsDialog::on_sel_image_btn_clicked() {
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

void SettingsDialog::on_type_color_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_COLOR);
}

void SettingsDialog::on_type_image_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_TEXTURE);
}

void SettingsDialog::on_skin_box_currentIndexChanged(const QString& arg1) {
  if (!arg1.isEmpty()) emit OptionChanged(OPT_SKIN_NAME, arg1);
}

void SettingsDialog::on_use_skin_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_SKIN_NAME, ui->skin_box->currentText());
}

void SettingsDialog::on_use_font_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_SKIN_NAME, "Text Skin");
}

void SettingsDialog::on_sel_font_btn_clicked() {
  bool ok = false;
  QFont font = QFontDialog::getFont(&ok, last_font_, this);
  if (ok) {
    emit OptionChanged(OPT_FONT, font);
    last_font_ = font;
  }
}

void SettingsDialog::on_apply_btn_clicked() {
  emit OptionChanged(OPT_TIME_FORMAT, ui->format_box->currentText());
}

void SettingsDialog::on_system_format_clicked() {
  emit OptionChanged(OPT_TIME_FORMAT, QString());
}

void SettingsDialog::on_custom_format_clicked()
{
  emit OptionChanged(OPT_TIME_FORMAT, ui->format_box->currentText());
}

void SettingsDialog::on_enable_autoupdate_toggled(bool checked) {
  emit OptionChanged(OPT_USE_AUTOUPDATE, checked);
}

void SettingsDialog::on_update_period_box_currentIndexChanged(int index) {
  emit OptionChanged(OPT_UPDATE_PERIOD, ui->update_period_box->itemData(index));
}

void SettingsDialog::on_check_for_beta_toggled(bool checked) {
  emit OptionChanged(OPT_CHECK_FOR_BETA, checked);
}

void SettingsDialog::on_space_slider_valueChanged(int arg1) {
  ui->space_slider->setToolTip(QString::number(arg1));
  emit OptionChanged(OPT_SPACING, arg1);
}

void SettingsDialog::on_export_btn_clicked() {
  QString filename = QFileDialog::getSaveFileName(this, tr("Export settings to ..."),
                     QDir::homePath(),
                     tr("Digital Clock settings files (*.dcs)"));
  if (!filename.isEmpty()) emit ExportSettings(filename);
}

void SettingsDialog::on_import_btn_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Import settings from ..."),
                     QDir::homePath(),
                     tr("Digital Clock settings files (*.dcs)"));
  if (!filename.isEmpty()) emit ImportSettings(filename);
}

} // namespace gui
} // namespace digital_clock

void digital_clock::gui::SettingsDialog::on_cust_none_toggled(bool checked) {
  if (!checked) return;
  ui->image_group->setDisabled(true);
  emit OptionChanged(OPT_CUSTOMIZATION, static_cast<int>(Customization::C_NONE));
}

void digital_clock::gui::SettingsDialog::on_cust_texturing_toggled(bool checked) {
  if (checked) {
    emit OptionChanged(OPT_CUSTOMIZATION, static_cast<int>(Customization::C_TEXTURING));
    if (ui->type_color->isChecked()) emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_COLOR);
    if (ui->type_image->isChecked()) emit OptionChanged(OPT_TEXTURE_TYPE, SkinDrawer::CT_TEXTURE);
    ui->image_group->setEnabled(ui->type_image->isChecked());
  } else {
    ui->image_group->setDisabled(true);
  }
}

void digital_clock::gui::SettingsDialog::on_cust_colorize_toggled(bool checked) {
  if (!checked) return;
  ui->image_group->setDisabled(true);
  emit OptionChanged(OPT_CUSTOMIZATION, static_cast<int>(Customization::C_COLORIZE));
}

void digital_clock::gui::SettingsDialog::on_img_color_btn_clicked() {
  QColor color = QColorDialog::getColor(last_colorize_color_, this);
  if (color.isValid()) {
    emit OptionChanged(OPT_COLORIZE_COLOR, color);
    last_colorize_color_ = color;
  }
}

void digital_clock::gui::SettingsDialog::on_level_slider_valueChanged(int value) {
  emit OptionChanged(OPT_COLORIZE_LEVEL, value / 100.);
}

void digital_clock::gui::SettingsDialog::on_align_left_rbtn_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(CAlignment::A_LEFT));
}

void digital_clock::gui::SettingsDialog::on_align_right_rbtn_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(CAlignment::A_RIGHT));
}
