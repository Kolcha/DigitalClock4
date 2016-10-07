/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_PLUGIN_INFO_DIALOG_H
#define DIGITAL_CLOCK_GUI_PLUGIN_INFO_DIALOG_H

#include <QDialog>
#include "core/plugin_info.h"

namespace digital_clock {
namespace gui {

namespace Ui {
class PluginInfoDialog;
}

class PluginInfoDialog : public QDialog
{
  Q_OBJECT

public:
  PluginInfoDialog(QWidget* parent = 0);
  ~PluginInfoDialog();

public slots:
  void SetInfo(const TPluginInfo& info);

private:
  Ui::PluginInfoDialog* ui;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_PLUGIN_INFO_DIALOG_H
