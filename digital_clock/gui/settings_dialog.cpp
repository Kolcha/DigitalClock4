#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QFileInfo>
#include <QSettings>
#include "skin_drawer.h"
#include "plugin_list_item_widget.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#define S_OPT_LAST_CUSTOMIZATION_KEY      "state/last_cutomization"
#define S_OPT_LAST_TIME_FORMAT_KEY        "state/last_time_format"
#define S_OPT_GEOMETRY_KEY                "state/settings_dialog_geometry"

using skin_draw::SkinDrawer;

namespace digital_clock {
namespace gui {

SettingsDialog::SettingsDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":/clock/images/settings.svg"));

  connect(this, SIGNAL(accepted()), this, SLOT(SaveState()));

  update_periods_[1] = tr("1 day");
  update_periods_[3] = tr("3 days");
  update_periods_[7] = tr("1 week");
  update_periods_[14] = tr("2 weeks");
  ui->update_period_box->clear();
  for (auto iter = update_periods_.begin(); iter != update_periods_.end(); ++iter) {
    ui->update_period_box->addItem(iter.value(), iter.key());
  }
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SettingsListener(Options opt, const QVariant& value) {
  switch (opt) {
    case OPT_OPACITY:
      ui->opacity_slider->setValue(int(value.toReal() * 100));
      break;

    case OPT_STAY_ON_TOP:
      ui->stay_on_top->setChecked(value.toBool());
      break;

    case OPT_TRANSP_FOR_INPUT:
      ui->transp_for_input->setChecked(value.toBool());
      break;

    case OPT_SEPARATOR_FLASH:
      ui->sep_flash->setChecked(value.toBool());
      break;

    case OPT_TIME_FORMAT:
    {
      QString format = value.toString();
      bool is_system = (format == GetDefaultValue(OPT_TIME_FORMAT).toString());
      if (!is_system) ui->format_box->setCurrentText(format);
      ui->system_format->setChecked(is_system);
      ui->custom_format->setChecked(!is_system);
      break;
    }

    case OPT_SKINS_PATHS:
    {
      QStringList paths = value.toStringList();
      for (auto& item : paths) ui->skins_paths_list->addItem(QDir::toNativeSeparators(item));
      break;
    }

    case OPT_PLUGINS_PATHS:
    {
      QStringList paths = value.toStringList();
      for (auto& item : paths) ui->plugins_paths_list->addItem(QDir::toNativeSeparators(item));
      break;
    }

    case OPT_SKIN_NAME:
      ui->skin_box->setCurrentText(value.toString());
      ui->use_skin->setChecked(value.toString() != "Text Skin");
      ui->use_font->setChecked(value.toString() == "Text Skin");
      break;

    case OPT_FONT:
      last_font_ = value.value<QFont>();
      break;

    case OPT_ZOOM:
      ui->zoom_slider->setValue(int(value.toReal() * 100));
      break;

    case OPT_COLOR:
      last_color_ = value.value<QColor>();
      break;

    case OPT_TEXTURE:
    {
      QString texture = value.toString();
      last_txd_path_ = texture.isEmpty() ? "textures" : QFileInfo(texture).absolutePath();
      break;
    }

    case OPT_TEXTURE_PER_ELEMENT:
      ui->txd_per_elem->setChecked(value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
    {
      SkinDrawer::DrawMode mode = (SkinDrawer::DrawMode)value.toInt();
      ui->mode_stretch->setChecked(mode == SkinDrawer::DM_STRETCH);
      ui->mode_tile->setChecked(mode == SkinDrawer::DM_TILE);
      break;
    }

    case OPT_CUSTOMIZATION:
    {
      SkinDrawer::CustomizationType type = (SkinDrawer::CustomizationType)value.toInt();
      ui->use_customization->setChecked(type != SkinDrawer::CT_NONE);
      if (type != SkinDrawer::CT_NONE) {
        ui->type_color->setChecked(type == SkinDrawer::CT_COLOR);
        ui->type_image->setChecked(type == SkinDrawer::CT_TEXTURE);
      } else {
        ui->type_color->setChecked(last_customization_ == (int)SkinDrawer::CT_COLOR);
        ui->type_image->setChecked(last_customization_ == (int)SkinDrawer::CT_TEXTURE);
      }
      break;
    }

    case OPT_SPACING:
      ui->space_value->setValue(value.toInt());
      break;

    case  OPT_PLUGINS:
      for (int i = 0; i < ui->plugins_list->count(); i++) {
        PluginListItemWidget* item = static_cast<PluginListItemWidget*>(
              ui->plugins_list->itemWidget(ui->plugins_list->item(i)));
        for (auto& plugin : value.toStringList()) {
          if (plugin == item->GetName()) {
            item->SetChecked(true);
            break;
          }
        }
      }
      break;

    case OPT_USE_AUTOUPDATE:
      ui->enable_autoupdate->setChecked(value.toBool());
      break;

    case OPT_UPDATE_PERIOD:
      ui->update_period_box->setCurrentText(update_periods_[value.value<qint64>()]);
      break;

    case OPT_CHECK_FOR_BETA:
      ui->check_for_beta->setChecked(value.toBool());
      break;
  }
}

void SettingsDialog::SetSkinList(const QStringList& skins) {
  ui->skin_box->addItems(skins);
}

void SettingsDialog::DisplaySkinInfo(const ::digital_clock::core::ClockBaseSkin::TSkinInfo& info) {
  using ::digital_clock::core::ClockBaseSkin;
  if (info[ClockBaseSkin::SI_NAME] == "Text Skin") return;
  ClockBaseSkin::TSkinInfo l_info = info;
  for (auto iter = l_info.begin(); iter != l_info.end(); ++iter) {
    if (iter.value().isEmpty()) iter.value() = tr("unknown");
  }
  ui->skin_box->setCurrentText(l_info[ClockBaseSkin::SI_NAME]);
  ui->version_value->setText(l_info[ClockBaseSkin::SI_VERSION]);
  ui->author_value->setText(l_info[ClockBaseSkin::SI_AUTHOR]);
  ui->email_value->setText(l_info[ClockBaseSkin::SI_EMAIL]);
  ui->skin_box->setToolTip(l_info[ClockBaseSkin::SI_COMMENT]);
}

void SettingsDialog::SetPluginsList(const QList<QPair<TPluginInfo, bool> >& plugins) {
  for (auto& plugin : plugins) {
    QListWidgetItem* item = new QListWidgetItem();
    PluginListItemWidget* widget = new PluginListItemWidget(ui->plugins_list);
    widget->SetInfo(plugin.first);
    widget->SetConfigurable(plugin.second);
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
  CenteredDialog::showEvent(e);
  LoadState();
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
  settings.setValue(S_OPT_LAST_CUSTOMIZATION_KEY, last_customization_);
  settings.setValue(S_OPT_LAST_TIME_FORMAT_KEY, ui->format_box->currentText());
  settings.setValue(S_OPT_GEOMETRY_KEY, saveGeometry());
}

void SettingsDialog::LoadState() {
  QSettings settings;
  last_customization_ = settings.value(S_OPT_LAST_CUSTOMIZATION_KEY,
                                       GetDefaultValue(OPT_CUSTOMIZATION)).toInt();
  ui->format_box->setCurrentText(settings.value(S_OPT_LAST_TIME_FORMAT_KEY,
                                                GetDefaultValue(OPT_TIME_FORMAT)).toString());
  restoreGeometry(settings.value(S_OPT_GEOMETRY_KEY).toByteArray());
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
    emit OptionChanged(OPT_CUSTOMIZATION, SkinDrawer::CT_COLOR);
    last_color_ = color;
    last_customization_ = SkinDrawer::CT_COLOR;
  }
}

void SettingsDialog::on_sel_image_btn_clicked() {
  QString texture = QFileDialog::getOpenFileName(this, tr("Open texture file"),
                    last_txd_path_,
                    tr("Images (*.bmp *.jpg *.jpeg *.png *.tiff *.xbm *.xpm)"));
  if (!texture.isEmpty()) {
    emit OptionChanged(OPT_TEXTURE, texture);
    emit OptionChanged(OPT_CUSTOMIZATION, SkinDrawer::CT_TEXTURE);
    last_txd_path_ = QFileInfo(texture).absolutePath();
    last_customization_ = SkinDrawer::CT_TEXTURE;
  }
}

void SettingsDialog::on_type_color_toggled(bool checked) {
  if (!checked) return;
  emit OptionChanged(OPT_CUSTOMIZATION, SkinDrawer::CT_COLOR);
  emit OptionChanged(OPT_COLOR, last_color_);
}

void SettingsDialog::on_type_image_toggled(bool checked) {
  SkinDrawer::CustomizationType current = checked ? SkinDrawer::CT_TEXTURE : SkinDrawer::CT_COLOR;
  emit OptionChanged(OPT_CUSTOMIZATION, current);
  last_customization_ = (int)current;
}

void SettingsDialog::on_skin_box_currentIndexChanged(const QString& arg1) {
  emit OptionChanged(OPT_SKIN_NAME, arg1);
}

void SettingsDialog::on_use_skin_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_SKIN_NAME, ui->skin_box->currentText());
}

void SettingsDialog::on_use_font_toggled(bool checked) {
  if (checked) {
    emit OptionChanged(OPT_SKIN_NAME, "Text Skin");
    ui->use_customization->setChecked(true);
  }
}

void SettingsDialog::on_sel_font_btn_clicked() {
  bool ok = false;
  QFont font = QFontDialog::getFont(&ok, last_font_, this);
  if (ok) {
    emit OptionChanged(OPT_FONT, font);
    last_font_ = font;
  }
}

void SettingsDialog::on_use_customization_toggled(bool checked) {
  emit OptionChanged(OPT_CUSTOMIZATION, checked ? last_customization_ : SkinDrawer::CT_NONE);
}

void SettingsDialog::on_apply_btn_clicked() {
  emit OptionChanged(OPT_TIME_FORMAT, ui->format_box->currentText());
}

void SettingsDialog::on_system_format_clicked() {
  emit OptionChanged(OPT_TIME_FORMAT, GetDefaultValue(OPT_TIME_FORMAT).toString());
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

void SettingsDialog::on_space_value_valueChanged(int arg1) {
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

void digital_clock::gui::SettingsDialog::on_add_sp_btn_clicked() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Add skins directory"),
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (dir.isEmpty()) return;
  ui->skins_paths_list->addItem(QDir::toNativeSeparators(dir));
}

void digital_clock::gui::SettingsDialog::on_del_sp_btn_clicked() {
  qDeleteAll(ui->skins_paths_list->selectedItems());
}

void digital_clock::gui::SettingsDialog::on_add_pp_btn_clicked() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Add plugins directory"),
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (dir.isEmpty()) return;
  ui->plugins_paths_list->addItem(QDir::toNativeSeparators(dir));
}

void digital_clock::gui::SettingsDialog::on_del_pp_btn_clicked() {
  qDeleteAll(ui->plugins_paths_list->selectedItems());
}
