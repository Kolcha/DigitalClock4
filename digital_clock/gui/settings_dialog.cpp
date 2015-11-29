#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QFileInfo>
#include <QSettings>
#include "../core/autostart.h"
#include "skin_drawer.h"
#include "plugin_list_item_widget.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#define S_OPT_LAST_TIME_FORMAT_KEY        "state/last_time_format"
#define S_OPT_GEOMETRY_KEY                "state/settings_dialog_geometry"

#ifdef Q_OS_OSX
#define DEFAULT_TEXTURE_PATH              "../textures"
#else
#define DEFAULT_TEXTURE_PATH              "textures"
#endif

using skin_draw::SkinDrawer;

namespace digital_clock {
namespace gui {

SettingsDialog::SettingsDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  ui->skin_info_btn->setVisible(false);  // temporary, not implemented
  setWindowIcon(QIcon(":/clock/images/settings.svg"));

  connect(this, SIGNAL(accepted()), this, SLOT(SaveState()));
  connect(ui->defaults_bth, SIGNAL(clicked()), this, SIGNAL(ResetSettings()));

  update_periods_[1] = tr("1 day");
  update_periods_[3] = tr("3 days");
  update_periods_[7] = tr("1 week");
  update_periods_[14] = tr("2 weeks");
  ui->update_period_box->clear();
  for (auto iter = update_periods_.begin(); iter != update_periods_.end(); ++iter) {
    ui->update_period_box->addItem(iter.value(), iter.key());
  }
  is_loading_ = false;

  ui->autostart->setChecked(IsAutoStartEnabled());
  connect(this, &SettingsDialog::accepted, [this] () {
    SetAutoStart(this->ui->autostart->isChecked());
  });
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SetCurrentSettings(const QMap<Options, QVariant>& settings) {
  is_loading_ = true;
  for (auto iter = settings.begin(); iter != settings.end(); iter++) {
    const Options& opt = iter.key();
    const QVariant& value = iter.value();
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

      case OPT_ALIGNMENT:
      {
        CAlignment alignment = static_cast<CAlignment>(value.toInt());
        ui->align_left_rbtn->setChecked(alignment == CAlignment::A_LEFT);
        ui->align_right_rbtn->setChecked(alignment == CAlignment::A_RIGHT);
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
        last_txd_path_ = texture.isEmpty() ? DEFAULT_TEXTURE_PATH : QFileInfo(texture).absolutePath();
        break;
      }

      case OPT_TEXTURE_TYPE:
      {
        SkinDrawer::CustomizationType type = static_cast<SkinDrawer::CustomizationType>(value.toInt());
        ui->type_color->setChecked(type == SkinDrawer::CT_COLOR);
        ui->type_image->setChecked(type == SkinDrawer::CT_TEXTURE);
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
        Customization cust = static_cast<Customization>(value.toInt());
        ui->cust_none->setChecked(cust == Customization::C_NONE);
        ui->cust_texturing->setChecked(cust == Customization::C_TEXTURING);
        ui->cust_colorize->setChecked(cust == Customization::C_COLORIZE);
        break;
      }

      case OPT_SPACING:
        ui->space_slider->setValue(value.toInt());
        break;

      case OPT_COLORIZE_COLOR:
        last_colorize_color_ = value.value<QColor>();
        break;

      case OPT_COLORIZE_LEVEL:
        ui->level_slider->setValue(static_cast<int>(value.toReal() * 100));
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
        active_plugins_ = value.toStringList();
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
  is_loading_ = false;
}

void SettingsDialog::SetSkinList(const QStringList& skins) {
  is_loading_ = true;
  QString current_skin;
  if (ui->skin_box->count()) current_skin = ui->skin_box->currentText();
  ui->skin_box->clear();
  ui->skin_box->addItems(skins);
  if (!current_skin.isEmpty()) ui->skin_box->setCurrentText(current_skin);
  is_loading_ = false;
}

void SettingsDialog::DisplaySkinInfo(const ::digital_clock::core::BaseSkin::TSkinInfo& info) {
  using ::digital_clock::core::BaseSkin;
  if (info[BaseSkin::SI_NAME] == "Text Skin") return;
  BaseSkin::TSkinInfo l_info = info;
  for (auto iter = l_info.begin(); iter != l_info.end(); ++iter) {
    if (iter.value().isEmpty()) iter.value() = tr("unknown");
  }
  ui->skin_box->setCurrentText(l_info[BaseSkin::SI_NAME]);
  ui->skin_box->setToolTip(l_info[BaseSkin::SI_COMMENT]);
}

void SettingsDialog::SetPluginsList(const QList<QPair<TPluginInfo, bool> >& plugins) {
  is_loading_ = true;
  ui->plugins_list->clear();
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
  is_loading_ = false;
}

void SettingsDialog::showEvent(QShowEvent* e) {
  CenteredDialog::showEvent(e);
  LoadState();
}

void SettingsDialog::ChangePluginState(const QString& name, bool activated) {
  if (is_loading_) return;
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
  QSettings settings;
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
  if (!arg1.isEmpty() && !is_loading_) emit OptionChanged(OPT_SKIN_NAME, arg1);
}

void SettingsDialog::on_use_skin_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_SKIN_NAME, ui->skin_box->currentText());
}

void SettingsDialog::on_use_font_toggled(bool checked) {
  if (checked) {
    emit OptionChanged(OPT_SKIN_NAME, "Text Skin");
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

void SettingsDialog::on_space_slider_valueChanged(int arg1) {
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
