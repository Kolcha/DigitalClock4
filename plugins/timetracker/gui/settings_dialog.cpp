/*
    Digital Clock: timetracker plugin
    Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "core/settings.h"

namespace timetracker {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::AddCommonWidget(QWidget* widget)
{
  widget->layout()->setMargin(12);
  ui->tabWidget->addTab(widget, tr("Appearance"));
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  ui->hide_if_inactive->setChecked(settings.value(OPT_HIDE_INACTIVE).toBool());
  ui->pause_seq_edit->setKeySequence(QKeySequence(settings.value(OPT_PAUSE_HOTKEY).toString()));
  ui->restart_seq_edit->setKeySequence(QKeySequence(settings.value(OPT_RESTART_HOTKEY).toString()));
  ui->settings_seq_edit->setKeySequence(QKeySequence(settings.value(OPT_SETTINGS_HOTKEY).toString()));
#ifndef HAVE_QHOTKEY
  ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->hotkeys_tab));
#endif
}

void SettingsDialog::on_hide_if_inactive_clicked(bool checked)
{
  emit OptionChanged(OPT_HIDE_INACTIVE, checked);
}

void SettingsDialog::on_pause_seq_edit_editingFinished()
{
  emit OptionChanged(OPT_PAUSE_HOTKEY, ui->pause_seq_edit->keySequence().toString());
}

void SettingsDialog::on_restart_seq_edit_editingFinished()
{
  emit OptionChanged(OPT_RESTART_HOTKEY, ui->restart_seq_edit->keySequence().toString());
}

void SettingsDialog::on_settings_seq_edit_editingFinished()
{
  emit OptionChanged(OPT_SETTINGS_HOTKEY, ui->settings_seq_edit->keySequence().toString());
}

} // namespace timetracker
