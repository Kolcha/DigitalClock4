#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QFileInfo>
#include <QSettings>
#include "skin_drawer.h"
#include "plugin_list_item.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#define OPT_LAST_CUSTOMIZATION_KEY   "settings_dialog/last_cutomization"
#define OPT_LAST_TIME_FORMAT_KEY     "settings_dialog/last_time_format"

SettingsDialog::SettingsDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":/images/settings.svg"));

  connect(this, SIGNAL(accepted()), this, SLOT(SaveState()));

  LoadState();
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
      bool is_system = (format == QLocale::system().timeFormat());
      if (!is_system) ui->format_box->setCurrentText(format);
      ui->system_format->setChecked(is_system);
      ui->custom_format->setChecked(!is_system);
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

    case  OPT_PLUGINS:
      for (int i = 0; i < ui->plugins_list->count(); i++) {
        PluginListItem* item = static_cast<PluginListItem*>(
              ui->plugins_list->itemWidget(ui->plugins_list->item(i)));
        for (auto& plugin : value.toStringList()) {
          if (plugin == item->GetText()) {
            item->SetChecked(true);
            break;
          }
        }
      }
      break;
  }
}

void SettingsDialog::SetSkinList(const QStringList& skins) {
  ui->skin_box->addItems(skins);
}

void SettingsDialog::DisplaySkinInfo(const ISkin::TSkinInfo& info) {
  if (info[ISkin::SI_NAME] == "Text Skin") return;
  for (auto i = info.begin(); i != info.end(); ++i) {
    switch (i.key()) {
      case ISkin::SI_NAME:
        ui->skin_box->setCurrentText(i.value());
        break;

      case ISkin::SI_VERSION:
        ui->version_value->setText(i.value());
        break;

      case ISkin::SI_AUTHOR:
        ui->author_value->setText(i.value());
        break;

      case ISkin::SI_EMAIL:
        ui->email_value->setText(i.value());
        break;

      case ISkin::SI_COMMENT:
        ui->skin_box->setToolTip(i.value());
        break;
    }
  }
}

void SettingsDialog::SetPluginsList(const QList<QPair<QString, bool> >& plugins) {
  for (auto& plugin : plugins) {
    QListWidgetItem* item = new QListWidgetItem();
    PluginListItem* widget = new PluginListItem(plugin.first, false, plugin.second);
    item->setData(Qt::UserRole, plugin.first);
    item->setSizeHint(widget->sizeHint());
    ui->plugins_list->addItem(item);
    ui->plugins_list->setItemWidget(item, widget);
    connect(widget, SIGNAL(StateChanged(QString,bool)), this, SLOT(ChangePluginState(QString,bool)));
    connect(widget, SIGNAL(ConfigureRequest(QString)), this, SIGNAL(PluginConfigureRequest(QString)));
  }
}

void SettingsDialog::DisplayPluginInfo(const TPluginInfo& info) {
  ui->p_type_value->setText(info[PI_TYPE]);
  ui->p_version_value->setText(info[PI_VERSION]);
  ui->p_author_value->setText(info[PI_AUTHOR]);
  ui->p_e_mail_value->setText(info[PI_EMAIL]);
  ui->p_comment->setText(info[PI_COMMENT]);
}

void SettingsDialog::changeEvent(QEvent* e) {
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

void SettingsDialog::ChangePluginState(const QString& name, bool activated) {
  if (activated)
    active_plugins_.append(name);
  else
    active_plugins_.removeOne(name);
  emit PluginInfoRequest(name);
  emit OptionChanged(OPT_PLUGINS, active_plugins_);
}

void SettingsDialog::SaveState() {
  QSettings settings;
  settings.setValue(OPT_LAST_CUSTOMIZATION_KEY, last_customization_);
  settings.setValue(OPT_LAST_TIME_FORMAT_KEY, ui->format_box->currentText());
}

void SettingsDialog::LoadState() {
  QSettings settings;
  last_customization_ = settings.value(OPT_LAST_CUSTOMIZATION_KEY,
                                       GetDefaultValue(OPT_CUSTOMIZATION)).toInt();
  ui->format_box->setCurrentText(settings.value(OPT_LAST_TIME_FORMAT_KEY,
                                                GetDefaultValue(OPT_TIME_FORMAT)).toString());
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

void SettingsDialog::on_plugins_list_currentItemChanged(
    QListWidgetItem* current, QListWidgetItem*) {
  emit PluginInfoRequest(current->data(Qt::UserRole).toString());
}

void SettingsDialog::on_use_skin_toggled(bool checked) {
  if (checked)
    emit OptionChanged(OPT_SKIN_NAME, ui->skin_box->currentText());
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
  emit OptionChanged(OPT_TIME_FORMAT, QLocale::system().timeFormat());
}