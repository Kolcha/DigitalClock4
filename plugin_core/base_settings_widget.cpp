/*
    Digital Clock: plugin core library
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "base_settings_widget.h"
#include "ui_base_settings_widget.h"

#include <QFontDialog>
#include <QColorDialog>
#include <QTranslator>
#include <QApplication>
#include <QLocale>

namespace plugin {

BaseSettingsWidget::BaseSettingsWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::BaseSettingsWidget)
{
  gui_translator_ = new QTranslator();
  QStringList ui_languages = QLocale::system().uiLanguages();
  QString prefix(":/plugin_core/lang/plugin_core_");
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (gui_translator_->load(prefix + locale)) {
      QApplication::installTranslator(gui_translator_);
      break;
    } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
      // use built-in
      break;
    } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
      // use built-in
      break;
    } else if (locale.contains("ua", Qt::CaseInsensitive)) { /* Ukrainian, use russian */
      if (gui_translator_->load(prefix + "ru"))
        QApplication::installTranslator(gui_translator_);
      break;
    }
  }

  ui->setupUi(this);
  layout()->setMargin(0);
}

BaseSettingsWidget::~BaseSettingsWidget()
{
  delete ui;
  QApplication::removeTranslator(gui_translator_);
  delete gui_translator_;
}

void BaseSettingsWidget::InitWidgets(const QMap<WidgetPluginOption, QVariant>& cur_values)
{
  for (auto iter = cur_values.constBegin(); iter != cur_values.constEnd(); ++iter) {
    switch (iter.key()) {
      case OPT_USE_CLOCK_FONT:
        ui->clock_font_button->setChecked(iter.value().toBool());
        ui->custom_font_button->setChecked(!iter.value().toBool());
        break;

      case OPT_CUSTOM_FONT:
        last_font_ = iter.value().value<QFont>();
        break;

      case OPT_ZOOM_MODE:
      {
        ZoomMode mode = static_cast<ZoomMode>(iter.value().toInt());
        ui->not_zoom->setChecked(mode == ZoomMode::ZM_NOT_ZOOM);
        ui->font_autosize->setChecked(mode == ZoomMode::ZM_AUTOSIZE);
        ui->clock_zoom->setChecked(mode == ZoomMode::ZM_CLOCK_ZOOM);
        break;
      }

      case OPT_USE_CUSTOM_COLOR:
        ui->use_custom_color->setChecked(iter.value().toBool());
        break;

      case OPT_CUSTOM_COLOR:
        last_color_ = iter.value().value<QColor>();
        break;
    }
  }
}

void BaseSettingsWidget::on_clock_font_button_toggled(bool checked)
{
  emit OptionChanged(OPT_USE_CLOCK_FONT, checked);
}

void BaseSettingsWidget::on_choose_font_button_clicked()
{
  bool ok = false;
  QFont font = QFontDialog::getFont(&ok, last_font_, this);
  if (ok) {
    emit OptionChanged(OPT_CUSTOM_FONT, font);
    last_font_ = font;
  }
}

void BaseSettingsWidget::on_not_zoom_clicked()
{
  emit OptionChanged(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_NOT_ZOOM));
}

void BaseSettingsWidget::on_font_autosize_clicked()
{
  emit OptionChanged(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_AUTOSIZE));
}

void BaseSettingsWidget::on_clock_zoom_clicked()
{
  emit OptionChanged(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_CLOCK_ZOOM));
}

void BaseSettingsWidget::on_use_custom_color_clicked(bool checked)
{
  emit OptionChanged(OPT_USE_CUSTOM_COLOR, checked);
}

void BaseSettingsWidget::on_custom_color_button_clicked()
{
  QColor color = QColorDialog::getColor(last_color_, this);
  if (!color.isValid()) return;
  last_color_ = color;
  emit OptionChanged(OPT_CUSTOM_COLOR, color);
}

} // namespace plugin
