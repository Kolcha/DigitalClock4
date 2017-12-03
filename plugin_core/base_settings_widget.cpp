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

namespace plugin {

BaseSettingsWidget::BaseSettingsWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::BaseSettingsWidget)
{
  ui->setupUi(this);
  layout()->setMargin(0);
}

BaseSettingsWidget::~BaseSettingsWidget()
{
  delete ui;
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
        ui->leave_as_is_rbtn->setChecked(mode == ZoomMode::ZM_NOT_ZOOM);
        ui->clock_size_percent_rbtn->setChecked(mode == ZoomMode::ZM_AUTOSIZE);
        break;
      }

      case OPT_SPACE_PERCENT:
        ui->percent_box->setValue(iter.value().toInt());
        break;

      case OPT_WIDGET_LOCATION:
      {
        WidgetLocation w_loc = static_cast<WidgetLocation>(iter.value().toInt());
        ui->pos_bottom_rbtn->setChecked(w_loc == WidgetLocation::WL_BOTTOM);
        ui->pos_right_rbtn->setChecked(w_loc == WidgetLocation::WL_RIGHT);
        break;
      }

      case OPT_ALIGNMENT:
      {
        Qt::Alignment c_align = static_cast<Qt::Alignment>(iter.value().toInt());
        ui->align_top_left_rbtn->setChecked(c_align == (Qt::AlignLeft | Qt::AlignTop));
        ui->align_top_rbtn->setChecked(c_align == (Qt::AlignHCenter | Qt::AlignTop));
        ui->align_top_right_rbtn->setChecked(c_align == (Qt::AlignRight | Qt::AlignTop));
        ui->align_left_rbtn->setChecked(c_align == (Qt::AlignLeft | Qt::AlignVCenter));
        ui->align_center_rbtn->setChecked(c_align == Qt::AlignCenter);
        ui->align_right_rbtn->setChecked(c_align == (Qt::AlignRight | Qt::AlignVCenter));
        ui->align_bottom_left_rbtn->setChecked(c_align == (Qt::AlignLeft | Qt::AlignBottom));
        ui->align_bottom_rbtn->setChecked(c_align == (Qt::AlignHCenter | Qt::AlignBottom));
        ui->align_bottom_right_rbtn->setChecked(c_align == (Qt::AlignRight | Qt::AlignBottom));
        break;
      }

      case OPT_USE_CUSTOM_COLOR:
        ui->use_custom_color->setChecked(iter.value().toBool());
        break;

      case OPT_CUSTOM_COLOR:
        last_color_ = iter.value().value<QColor>();
        break;

      case OPT_USE_CLOCK_SKIN:
        ui->use_clock_skin->setChecked(iter.value().toBool());
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

void BaseSettingsWidget::on_leave_as_is_rbtn_clicked()
{
  emit OptionChanged(OPT_ZOOM_MODE, ZoomMode::ZM_NOT_ZOOM);
}

void BaseSettingsWidget::on_clock_size_percent_rbtn_clicked()
{
  emit OptionChanged(OPT_ZOOM_MODE, ZoomMode::ZM_AUTOSIZE);
}

void BaseSettingsWidget::on_percent_box_valueChanged(int arg1)
{
  emit OptionChanged(OPT_SPACE_PERCENT, arg1);
}

void BaseSettingsWidget::on_pos_bottom_rbtn_clicked()
{
  emit OptionChanged(OPT_WIDGET_LOCATION, WidgetLocation::WL_BOTTOM);
}

void BaseSettingsWidget::on_pos_right_rbtn_clicked()
{
  emit OptionChanged(OPT_WIDGET_LOCATION, WidgetLocation::WL_RIGHT);
}

void BaseSettingsWidget::on_align_top_left_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignLeft | Qt::AlignTop));
}

void BaseSettingsWidget::on_align_top_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignHCenter | Qt::AlignTop));
}

void BaseSettingsWidget::on_align_top_right_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignRight | Qt::AlignTop));
}

void BaseSettingsWidget::on_align_left_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter));
}

void BaseSettingsWidget::on_align_center_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignCenter));
}

void BaseSettingsWidget::on_align_right_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignRight | Qt::AlignVCenter));
}

void BaseSettingsWidget::on_align_bottom_left_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignLeft | Qt::AlignBottom));
}

void BaseSettingsWidget::on_align_bottom_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignHCenter | Qt::AlignBottom));
}

void BaseSettingsWidget::on_align_bottom_right_rbtn_clicked()
{
  emit OptionChanged(OPT_ALIGNMENT, static_cast<int>(Qt::AlignRight | Qt::AlignBottom));
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

void BaseSettingsWidget::on_use_clock_skin_clicked(bool checked)
{
  emit OptionChanged(OPT_USE_CLOCK_SKIN, checked);
}

} // namespace plugin
