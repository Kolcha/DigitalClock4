/*
    Digital Clock: alarm plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "advanced_settings_dialog.h"
#include "ui_advanced_settings_dialog.h"

#include "core/alarm_settings.h"

namespace alarm_plugin {

AdvancedSettingsDialog::AdvancedSettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::AdvancedSettingsDialog)
{
  ui->setupUi(this);
  connect(ui->shortcut_clear_btn, &QToolButton::clicked, this, &AdvancedSettingsDialog::on_shortcut_edit_editingFinished);
}

AdvancedSettingsDialog::~AdvancedSettingsDialog()
{
  delete ui;
}

void AdvancedSettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  ui->shortcut_edit->setKeySequence(QKeySequence(settings.value(OPT_STOP_ALARM_SHORTCUT).toString()));
}

void AdvancedSettingsDialog::on_shortcut_edit_editingFinished()
{
  emit OptionChanged(OPT_STOP_ALARM_SHORTCUT, ui->shortcut_edit->keySequence().toString());
}

} // namespace alarm_plugin
