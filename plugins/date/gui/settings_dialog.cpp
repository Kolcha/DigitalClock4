#include <QFontDialog>
#include "../date_settings.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace date {

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDialog) {
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);

  date_formats_[Qt::TextDate] = tr("default");
  date_formats_[Qt::ISODate] = QString("ISO 8601");
  date_formats_[Qt::SystemLocaleShortDate] = tr("short");
  date_formats_[Qt::SystemLocaleLongDate] = tr("long");
  ui->int_type_box->clear();
  for (auto iter = date_formats_.begin(); iter != date_formats_.end(); ++iter) {
    ui->int_type_box->addItem(iter.value(), iter.key());
  }
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings) {
  for (auto iter = settings.constBegin(); iter != settings.constEnd(); ++iter) {
    this->SettingsListener(iter.key(), iter.value());
  }
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_DATE_FORMAT_TYPE) {
    ui->int_type_button->setChecked((FormatType)value.toInt() == FormatType::FT_INT);
    ui->str_type_button->setChecked((FormatType)value.toInt() == FormatType::FT_STR);
  }
  if (key == OPT_DATE_FORMAT_INT) {
    ui->int_type_box->setCurrentText(date_formats_[(Qt::DateFormat)value.toInt()]);
  }
  if (key == OPT_DATE_FORMAT_STR) {
    ui->str_type_box->setCurrentText(value.toString());
  }
  if (key == OPT_USE_CLOCK_FONT) {
    ui->clock_font_button->setChecked(value.toBool());
    ui->custom_font_button->setChecked(!value.toBool());
  }
  if (key == OPT_DATE_FONT) {
    last_font_ = value.value<QFont>();
  }
  if (key == OPT_ZOOM_MODE) {
    ZoomMode mode = (ZoomMode)value.toInt();
    ui->not_zoom->setChecked(mode == ZoomMode::ZM_NOT_ZOOM);
    ui->font_autosize->setChecked(mode == ZoomMode::ZM_AUTOSIZE);
    ui->clock_zoom->setChecked(mode == ZoomMode::ZM_CLOCK_ZOOM);
  }
}

void SettingsDialog::on_int_type_button_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_DATE_FORMAT_TYPE, (int)FormatType::FT_INT);
}

void SettingsDialog::on_str_type_button_toggled(bool checked) {
  if (checked) emit OptionChanged(OPT_DATE_FORMAT_TYPE, (int)FormatType::FT_STR);
}

void SettingsDialog::on_int_type_box_currentIndexChanged(int index) {
  emit OptionChanged(OPT_DATE_FORMAT_INT, ui->int_type_box->itemData(index));
}

void SettingsDialog::on_str_type_box_currentTextChanged(const QString& arg1) {
  emit OptionChanged(OPT_DATE_FORMAT_STR, arg1);
}

void SettingsDialog::on_clock_font_button_toggled(bool checked) {
  emit OptionChanged(OPT_USE_CLOCK_FONT, checked);
}

void SettingsDialog::on_choose_font_button_clicked() {
  bool ok = false;
  QFont font = QFontDialog::getFont(&ok, last_font_, this);
  if (ok) {
    emit OptionChanged(OPT_DATE_FONT, font);
    last_font_ = font;
  }
}

void SettingsDialog::on_not_zoom_clicked() {
  emit OptionChanged(OPT_ZOOM_MODE, (int)ZoomMode::ZM_NOT_ZOOM);
}

void SettingsDialog::on_font_autosize_clicked() {
  emit OptionChanged(OPT_ZOOM_MODE, (int)ZoomMode::ZM_AUTOSIZE);
}

void SettingsDialog::on_clock_zoom_clicked() {
  emit OptionChanged(OPT_ZOOM_MODE, (int)ZoomMode::ZM_CLOCK_ZOOM);
}

} // namespace date
