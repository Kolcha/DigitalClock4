#include "../skin/skin_drawer.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SettingsListener(Options opt, const QVariant& value) {
  switch (opt) {
    case OPT_OPACITY:
    {
      qreal opacity = value.toReal();
      ui->opacity_slider->setValue(int(opacity > 0.1 ? opacity * 100 : 75));
      break;
    }

    case OPT_STAY_ON_TOP:
      ui->stay_on_top->setChecked(value.toBool());
      break;

    case OPT_TRANSP_FOR_INPUT:
      ui->transp_for_input->setChecked(value.toBool());
      break;

    case OPT_SEPARATOR_FLASH:
      ui->sep_flash->setChecked(value.toBool());
      break;

    case OPT_SKIN_NAME:
    {
      QString skin_name = value.toString();
      ui->skin_box->setCurrentText(skin_name.isEmpty() ? "Comic Sans" : skin_name);
      break;
    }

    case OPT_ZOOM:
    {
      qreal zoom = value.toReal();
      ui->zoom_slider->setValue(int((zoom > 0.1) && (zoom <= 4) ? zoom * 100 : 125));
      break;
    }

    case OPT_COLOR:
    {
//      QColor color = value.value<QColor>();
//      ui->type_color->setChecked(color.isValid());
      break;
    }

    case OPT_TEXTURE:
    {
//      QString texture = value.toString();
//      ui->type_image->setChecked(!texture.isEmpty());
      break;
    }

    case OPT_TEXTURE_PER_ELEMENT:
      ui->txd_per_elem->setChecked(value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
    {
      SkinDrawer::DrawMode mode = (SkinDrawer::DrawMode)value.toInt();
      switch (mode) {
        case SkinDrawer::DM_STRETCH:
          ui->mode_stretch->setChecked(true);
          break;

        case SkinDrawer::DM_TILE:
          ui->mode_tile->setChecked(true);
          break;
      }
      break;
    }

    case OPT_USE_TEXTURE:
      ui->type_color->setChecked(!value.toBool());
      ui->type_image->setChecked(value.toBool());
      break;
  }
}

void SettingsDialog::SetSkinList(const QStringList& skins) {
  ui->skin_box->addItems(skins);
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
